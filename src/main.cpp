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
#include "DeferredRenderer.h"
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

int anisotropicFiltering = 8;

float light0QuadraticAttenuation = 0.01f;

Ezr::Camera* cam;
Ezr::Scene* scene;
Ezr::Timer* timer;

DeferredRenderer* deferred;

float fov = 90.0f;
float nearPlane = 0.01f;
float farPlane = 10.0;

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
//void load();
//void loadImages();
//void reloadShaders();

//void drawPass(const Matrix4f& modelView, const float nearPlane, const float fov);
//float nearPlaneSize(const float nearPlaneDist, const float fov);

void display(void){    
 
	cam->UpdateCamPos(timer->GetFrameInterval(), w, s, a, d);
	timer->CalculateFrameRate();
	
	deferred->draw();

    stringstream stream;
    stream << timer->GetFramesPerSecond();
    font->renderText(stream.str());
   
	glutSwapBuffers();
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

    cam = new Ezr::Camera(wndWidth, wndHeight, fov, nearPlane, farPlane);
	cam->PositionCamera( 1, 0, 0,   0, 0, -1,   0, 1, 0);

	//load();
	scene = new Ezr::Scene(cam);
}

//void load()
//{
//	loadImages();
//	loadShaders(lightDirection,
//				fbo->getColorAttachment(color3_depth1Name),
//				fbo->getColorAttachment(normal2Name));
//}

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

