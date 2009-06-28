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
#include "Eigen/Geometry"
#include <Eigen/LU>

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

using namespace Eigen;
using namespace Ezr;


//non 2^n sizes make FBOs VERY slow
int wndWidth = 512;
int wndHeight = 512;

GLdouble gNear = 0.1;
GLdouble gFar = 100.0;

int anisotropicFiltering = 8;

float fullscreenQuadSize = 1.0;

float light0QuadraticAttenuation = 0.01f;

Ezr::Camera* cam;
Ezr::Fbo* fbo;
Ezr::Scene* scene;
Ezr::Timer* timer;

/**
 * current Viewport we're using
 */
Ezr::Viewport* window;
int _x, _y;

bool leftButtonDown, leftButtonJustDown, useFbo, useShader, w, s, a, d = false;

GLfloat light_position[] = {3.0, 3.0, 2.0, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {1, 1, 1.0, 1.0};
GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };

//static const std::string deferredVertexShaderPath("res/shaders/deferred/basic.vert");
//static const std::string deferredFragmentShaderPath("res/shaders/deferred/basic.frag");
static Ezr::GlBindShader* deferredShader;
static const std::string deferredVertexShaderPath("res/shaders/deferred/basic.vert");
static const std::string deferredFragmentShaderPath("res/shaders/deferred/basic.frag");

static Ezr::GlBindShader* deferredDirectionalLightShader;
static const std::string deferredDirectionalVertexShaderPath("res/shaders/deferred/lightDirectional.vert");
static const std::string deferredDirectionalFragmentShaderPath("res/shaders/deferred/lightDirectional.frag");

static const std::string colorMapPath("res/textures/lava.tga");
static const std::string normalMapPath("res/textures/lava-normal.tga");
//static const std::string normalMapPath("res/textures/defaultnormals.tga");
Ezr::Texture* colormap;
Ezr::Texture* normalmap;



void init();
void load();
void loadImages();
void reloadShaders();

void drawPass();


void display(void){    
 
	cam->UpdateCamPos(timer->GetFrameInterval(), w, s, a, d);
	timer->CalculateFrameRate();
	
	if(useFbo)
	{
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, wndWidth, wndHeight);
		OpenGl::printGlError("after FBO binding");

		fbo->bind();
		std::string b1("color3_depth1");
		fbo->clearColorAttachment(b1, 0, 0, 0, 1);

		std::string b2("normal2");
		fbo->clearColorAttachment(b2, 0, 0, 1, 1);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		OpenGl::printGlError("after FBO depth buffer clear");
	}
	else
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();

	cam->CamLookAt();
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
    glActiveTexture(GL_TEXTURE0);
	colormap->bind();
	glEnable(GL_TEXTURE_2D);
	
	if (useShader)
	{
		deferredShader->bind();

		// //multitexturing
		glActiveTexture(GL_TEXTURE1);
		normalmap->bind();
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		//tell the shader about uniforms
		GLint program = deferredShader->getProgram();
        GLint colorMapL = glGetUniformLocation(program, "colorMap");
		glUniform1i(colorMapL, 0);
        GLint normalMapL = glGetUniformLocationARB(program, "normalMap");
		glUniform1i(normalMapL, 1);
        GLint invRadiusL = glGetUniformLocationARB(program, "invRadius");
		glUniform1f(invRadiusL, light0QuadraticAttenuation);

		OpenGl::printGlError("after shader binding");
	}

	
	/*GLfloat m[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, m);
	Matrix4f modelViewMatrix(m);*/

	

	//draw geometry
	glutSolidTeapot(1);
	//scene->drawScene();
	

	if (useShader)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		deferredShader->unbind();
		OpenGl::printGlError("after shader unbinding");
		
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	Ezr::OpenGl::printGlError("before fbo unbinding");
	
	if(useFbo)
	{
		fbo->unbindFbo(); 
		
		glPopAttrib();
		glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

		if (useShader) {
			std::string att0("color3_depth1");
			std::string att1("normal2");

			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			fbo->getColorAttachment(att0)->bind();
			OpenGl::printGlError("fbo/shader texture0 bind");

			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			fbo->getColorAttachment(att1)->bind();
			glActiveTexture(GL_TEXTURE0);
			OpenGl::printGlError("fbo/shader texture1 bind");
			
			
			//tell the shader about uniforms
			deferredDirectionalLightShader->bind();
			GLhandleARB program = deferredDirectionalLightShader->getProgram();

			GLint att0L = glGetUniformLocation(program, att0.c_str());
			OpenGl::printGlError("fbo/shader texture0 get");
			glUniform1i(att0L, 0);
			OpenGl::printGlError("fbo/shader texture0 set");
			
			GLint att1L = glGetUniformLocation(program, att1.c_str());
			OpenGl::printGlError("fbo/shader texture1 get");
			glUniform1i(att1L, 1);
			OpenGl::printGlError("fbo/shader texture1 set");

			GLint eyeL = glGetUniformLocation(program, "eye");
			OpenGl::printGlError("fbo/shader eye get");
			Vector3f eye = cam->GetCamPos();
			glUniform3f(eyeL, eye.x(), eye.y(), eye.z());
			OpenGl::printGlError("fbo/shader eye set");

			//Vector3f lightDirection(-1, 1, 0);
			Vector3f lightDirection(1, 1, 0);
			lightDirection.normalize();
			GLint lightL = glGetUniformLocation(program, "lightdir");
			OpenGl::printGlError("fbo/shader lightdir get");
			glUniform3f(lightL, lightDirection.x(), lightDirection.y(), lightDirection.z());
			OpenGl::printGlError("fbo/shader lightdir set");
			
			GLint modelViewMatrixL = glGetUniformLocation(program, "modelViewMatrix");
			glUniform1fv(modelViewMatrixL, 16, m);
		}
		else
		{
			std::string bla("color3_depth1");
			//std::string bla("normal2");
			fbo->getColorAttachment(bla)->bind();
		}

		glEnable(GL_TEXTURE_2D);

		drawPass();

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (useShader)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			
			deferredDirectionalLightShader->unbind();
		}		
	}
//	std::cout << timer->GetFramesPerSecond() << std::endl;
	glutSwapBuffers();
}

void drawPass()
{
		//draw a screensize rectangle with the fbo rendered scene as texture on it
		glMatrixMode(GL_MODELVIEW);	
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode (GL_PROJECTION); 
		glPushMatrix (); 
		glLoadIdentity ();
		float orthoSize = 1.0 / fullscreenQuadSize;
		glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, -1.0, 1.0);

		// glTranslatef(0, 0, -0.1f);
		// float scale = 0.1f * tan(30.0f / 360.0f * 2.0f * MyMath::PI);
		// glScalef(scale, scale, scale);

		//glMatrixMode(GL_PROJECTION);

		glBegin (GL_QUADS); 
		//glNormal3f( 0.0f, 0.0f, 1.0);
		glTexCoord2f(0.0, 0.0);
        glVertex2f(-1.0, -1.0);
        glTexCoord2f(1.0, 0.0);
        glVertex2f( 1.0, -1.0);
        glTexCoord2f(1.0, 1.0);
        glVertex2f( 1.0,  1.0);
        glTexCoord2f(0.0, 1.0);
        glVertex2f(-1.0,  1.0);
		glEnd ();

		glPopMatrix();

		glMatrixMode (GL_MODELVIEW); 
		glPopMatrix();
}

void reshape (int w, int h){
	Vector2i windowSize(w, h);
	window->setWindowSize(windowSize);
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(w)/h , 0.1f, 100.0f); // FOV, AspectRatio, NearClip, FarClip

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
	case '[':
		fullscreenQuadSize += 0.1;
		break;
	case ']':
		fullscreenQuadSize -= 0.1;
		break;
	case '\'':
		anisotropicFiltering = (anisotropicFiltering == 0) ? 8 : 0;
		colormap->setAnisotropicFiltering(anisotropicFiltering);
		std::cout << "Anisotropic Filtering: " << anisotropicFiltering << std::endl;
		break;
	case 'o':
		reloadShaders();
		std::cout << "reloading shaders: " << std::endl;
		break;
	case 'i':
		std::cout << "reloading images... ";
		loadImages();
		std::cout << "finished." << std::endl;

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
			//std::cout << "up!" << std::endl;
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
		//std::cout << x << "," << y << std::endl;

		Vector2i half = window->getWindowSize() / 2;
		/** @todo 2009-06-20 23:19 hrehfeld    move somewhere central */
		glutWarpPointer(half.x(), half.y());

		//ignore first down
		if (leftButtonJustDown)
		{
			leftButtonJustDown = false;
			//std::cout << "first  press!" << std::endl;
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
	glClearDepth (0.0f);
	glClearDepth (gFar);
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
	//wglSwapIntervalEXT(0);

	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	//glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
    fbo = new Ezr::Fbo(wndWidth, wndHeight, Ezr::Fbo::Depth);
    fbo->attachColorbuffer("color3_depth1");
	fbo->attachColorbuffer("normal2");
	glDrawBuffer(GL_NONE);
	
	fbo->checkFbo();
	fbo->unbindFbo();


	glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	

	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, 24);


	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	//set light like in the phong shader..
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light0QuadraticAttenuation);
		
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	timer = new Ezr::Timer();
	
	glViewport(0, 0, wndWidth, wndHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)wndWidth/(GLfloat)wndHeight, 0.1 ,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	cam = new Ezr::Camera(wndWidth, wndHeight);
	cam->PositionCamera( -7, 0, 0,   1, 0, 0,   0, 1, 0);

	load();
	//scene = new Ezr::Scene();
}

void loadShaders() {
	deferredShader = new Ezr::GlBindShader(Ezr::Utilities::loadFile(deferredVertexShaderPath),
	 									   Ezr::Utilities::loadFile(deferredFragmentShaderPath));

	deferredDirectionalLightShader
		= new Ezr::GlBindShader(Ezr::Utilities::loadFile(deferredDirectionalVertexShaderPath),
								Ezr::Utilities::loadFile(deferredDirectionalFragmentShaderPath));
}

void reloadShaders() {
	delete deferredShader;
	delete deferredDirectionalLightShader;
	loadShaders();
}

void load()
{
	loadShaders();
	loadImages();
}

void loadImages()
{
	if (colormap != NULL)
	{
		delete colormap;
	}
	Ezr::Image colorImage(colorMapPath);
	colormap = new Ezr::Texture(colorImage);

	if (normalmap != NULL)
	{
		delete normalmap;
	}
	Ezr::Image normalImage(normalMapPath);
	normalmap = new Ezr::Texture(normalImage);
}

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

