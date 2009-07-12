#include <iostream>
#include "OpenGL.h"
#include "Camera.h"
#include "Fbo.h"
#include "Scene.h"
#include "Timer.h"
#include "Utilities.h"
#include "gl/GlBindShader.h"
#include "Viewport.h"
#include "Texture.h"
#include "Image.h"
#include "MyMath.h"
#include <algorithm>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include "shader/Shader.h"
#include "shader/DeferredDrawShader.h"
#include "shader/DeferredDirectionalLighting.h"
#include "shader/DeferredPointLighting.h"
#include <sstream>

#include "font/Font.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

using namespace Eigen;
using namespace Ezr;


//non 2^n sizes make FBOs VERY slow
int wndWidth = 512;
int wndHeight = 512;

float fov = 90.0f;
float nearPlane = 0.01f;
float farPlane = 10.0;


int anisotropicFiltering = 8;

float light0QuadraticAttenuation = 0.01f;

Ezr::Camera* cam;
Ezr::Scene* scene;
Ezr::Timer* timer;

DeferredRenderer* deferred;



/**
 * current Viewport we're using
 */
Ezr::Viewport* window;
int _x, _y;

bool leftButtonDown, leftButtonJustDown, useFbo, useShader, w, s, a, d = false;

bool usePointLight = true;
bool useDirectionalLight = true;
bool drawLightGeometry = false;

Vector3f* lightDirection = new Vector3f(0, -2, 1);

//Vector3f* lightPosition = new Vector3f(2, 1, 0);
Vector3f* lightPosition = new Vector3f(0, 4, -1);
float attenuation = 0.01f;
float lightRadius = 5.0f;

Ezr::Font* font;

void init();
void load();
void loadImages();
void reloadShaders();

void drawPass(const Matrix4f& modelView, const float nearPlane, const float fov);
float nearPlaneSize(const float nearPlaneDist, const float fov);

void display(void){    
 
	cam->UpdateCamPos(timer->GetFrameInterval(), w, s, a, d);
	timer->CalculateFrameRate();
	
	deferred->draw();

    stringstream stream;
    stream << timer->GetFramesPerSecond();
    font->renderText(stream.str());
   
	glutSwapBuffers();
}

/**
 * calculate smallest distance from center of the nearplane to the side
 */
float nearPlaneSize(const float nearPlaneDist, const float fov)
{
	float halfFov = fov / 2.0f;
	return nearPlaneDist * tan(halfFov / 360.0f * 2.0f * MyMath::PI);
}

/**
 * draw a screensize rectangle with the fbo rendered scene as texture on it
 *
 * @param modelView Camera modelview matrix so we can position the quad before the camera
 */
void drawPass(const Matrix4f& modelView, const float nearPlane, const float fov)
{
	//nearplane, but with a small offset to ensure we don't get clipped
	float nearP = nearPlane + 0.01f;

	float size = nearPlaneSize(nearPlane + 0.01f, fov);

	Vector4f quad[] = {
		Vector4f(-size, -size, -nearP, 1),
		Vector4f( size, -size, -nearP, 1),
		Vector4f( size,  size, -nearP, 1),
		Vector4f(-size,  size, -nearP, 1)
	};

	Matrix4f modelViewInverse = modelView.inverse();

	//we don't mess with any state, but draw the rectangle perfectly in front of the camera
	for (int i = 0; i < 4; ++i) {
		quad[i] = (modelViewInverse * quad[i]);
	}

	glDisable(GL_DEPTH_TEST);
		
	Vector4f p;
	glBegin (GL_QUADS); 
		glTexCoord2f(0.0, 0.0);
		p = quad[0];
        glVertex3f(p.x(), p.y(), p.z());

        glTexCoord2f(1.0, 0.0);
		p = quad[1];
        glVertex3f(p.x(), p.y(), p.z());

        glTexCoord2f(1.0, 1.0);
		p = quad[2];
        glVertex3f(p.x(), p.y(), p.z());

        glTexCoord2f(0.0, 1.0);
		p = quad[3];
        glVertex3f(p.x(), p.y(), p.z());
	glEnd ();

	glEnable(GL_DEPTH_TEST);
	
}

void reshape (int w, int h){
	Vector2i windowSize(w, h);
	window->setWindowSize(windowSize);
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float)(w)/h , nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}


//keyboard functions
void keyboard(unsigned char key, int x, int y)
{
	switch(key){
		//escape key pressed
		case 27:
			exit(0);
			break;
		case 'f':
			useFbo = !useFbo;
			std::cout << "fbo: " << useFbo << std::endl;
			break;
		case 'g':
			useShader = !useShader;
			std::cout << "shader: " << useShader << std::endl;
			break;
		case 'w' :
			w=true;
			break;
		case 's' :
			s=true;
			break;
		case 'a' :
			a=true;
			break;
		case 'd' :
			d=true;
			break;
	case '1':
		useDirectionalLight = !useDirectionalLight;
		std::cout << "Directional light: " << useDirectionalLight << std::endl;
		break;
	case '2':
		usePointLight = !usePointLight;
		std::cout << "Point light: " << usePointLight << std::endl;
		break;
	case 'h':
		drawLightGeometry = !drawLightGeometry;
		std::cout << "drawing light geometry: " << drawLightGeometry << std::endl;
		break;
		
	}
}

void releaseKey(unsigned char key, int x, int y)
{
	switch(key){
		case 'w' :
			w=false;
			break;
		case 's' :
			s=false;
			break;
		case 'a' :
			a=false;
			break;
		case 'd' :
			d=false;
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			leftButtonDown = true;
			leftButtonJustDown = true;
		}
		else
		{
			leftButtonDown = false;
			leftButtonJustDown = false;
		}

		// hide cursor on move
		glutSetCursor(state ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
	}
}

void mouseMotion(int x, int y)
{
	if (leftButtonDown) {
		Vector2i half = window->getWindowSize() / 2;
		/** @todo 2009-06-20 23:19 hrehfeld    move somewhere central */
		glutWarpPointer(half.x(), half.y());

		//ignore first down
		if (leftButtonJustDown)
		{
			leftButtonJustDown = false;
			return;
		}
			
		Vector2i mousePosition(x,y);
		Vector2f relativeMousePos = window->getMousePosition(mousePosition);
		cam->rotateView(relativeMousePos);
		
		display();
	}
}


//init openGL
void init(void)
{
	glShadeModel (GL_SMOOTH);				
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);	
	glClearDepth (farPlane);
	glEnable (GL_DEPTH_TEST);				
    glDepthFunc (GL_LEQUAL);	

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
	{
		std::cerr << "Ready for OpenGL 2.0\n" << std::endl;; 
	}
	else{
		std::cerr << "OpenGL 2.0 not supported\n" << std::endl;
		exit(1);
	}
	
	//disable vsync
	wglSwapIntervalEXT(0);

	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	//glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	

	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, 24);

    font = new Font("res/fonts/arial.glf", window);	

	timer = new Ezr::Timer();
	
	glViewport(0, 0, wndWidth, wndHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)wndWidth/(GLfloat)wndHeight, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	cam = new Ezr::Camera(wndWidth, wndHeight);
	cam->PositionCamera( 1, 0, 0,   0, 0, -1,   0, 1, 0);

	load();
	scene = new Ezr::Scene(cam);
}

void load()
{
	loadImages();
	loadShaders(lightDirection,
				fbo->getColorAttachment(color3_depth1Name),
				fbo->getColorAttachment(normal2Name));
}

// void loadImages()
// {
// 	if (colormap != NULL)
// 	{
// 		delete colormap;
// 	}
// 	Ezr::Image colorImage(colorMapPath);
// 	colormap = new Ezr::Texture(colorImage);

// 	if (normalmap != NULL)
// 	{
// 		delete normalmap;
// 	}
// 	Ezr::Image normalImage(normalMapPath);
// 	normalmap = new Ezr::Texture(normalImage);
// }

int main(int argc, char* argv[])
{
	Vector2i windowSize(wndWidth, wndHeight);
	window = new Ezr::Viewport(windowSize);

	//setup a GLUT window
	glutInit(&argc, argv);   
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);    
    glutInitWindowSize(wndWidth, wndHeight);

	glutCreateWindow("Demo");
  	
	init();

	//registering callback functions
	glutReshapeFunc(reshape);	
    glutDisplayFunc(display);
    glutIdleFunc(display);   
	glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(releaseKey);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	
	glutMainLoop();
		
	return 0;
}

