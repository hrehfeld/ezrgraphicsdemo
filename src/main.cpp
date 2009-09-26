#include <iostream>
#include "OpenGL.h"
#include "Camera.h"
#include "Scene.h"
#include "Timer.h"
#include "Utilities.h"
#include "Viewport.h"
#include <algorithm>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <sstream>
#include "DeferredRenderer.h"
#include "font/Font.h"

using namespace Eigen;
using namespace Ezr;
using namespace std;

//non 2^n sizes make FBOs VERY slow
int wndWidth = 512;
int wndHeight = 512;

float fov = 60.0f;
float nearPlane = 0.01f;
float farPlane = 1000.0;
int anisotropicFiltering = 8;
float light0QuadraticAttenuation = 0.01f;

Camera* _cam;
DeferredRenderer* _deferredRenderer;
Scene* _scene;
Timer* _timer;
Viewport* _viewport;

int _x, _y;
bool leftButtonDown, leftButtonJustDown, w, s, a, d = false;

Font* font;

void init();

void display(void){    
 
	_cam->UpdateCamPos(_timer->GetFrameInterval(), w, s, a, d);
	_timer->CalculateFrameRate();
	
    _deferredRenderer->render();

    stringstream stream;
    stream << _timer->GetFramesPerSecond();
    font->renderText(stream.str());
   
	glutSwapBuffers();
}

void reshape (int w, int h){
	Vector2i viewportSize(w, h);
	_viewport->setViewportSize(viewportSize);
	
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float)(w)/h , nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key){
		//escape key pressed
		case 27:
			exit(0);
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
        case '1' :
			_deferredRenderer->toggleDirectionalLight();
			break;
        case '2' :
		    _deferredRenderer->togglePointLight();
		    break;
        case 'g' :
		    _deferredRenderer->toggleLightGeometry();
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
		Vector2i half = _viewport->getViewportSize() / 2;
		/** @todo 2009-06-20 23:19 hrehfeld    move somewhere central */
		glutWarpPointer(half.x(), half.y());

		//ignore first down
		if (leftButtonJustDown)
		{
			leftButtonJustDown = false;
			return;
		}
			
		Vector2i mousePosition(x,y);
		Vector2f relativeMousePos = _viewport->getMousePosition(mousePosition);
		_cam->rotateView(relativeMousePos);

        display();
	}
}


void init(void)
{
	glShadeModel (GL_SMOOTH);				
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);	
	glClearDepth (farPlane);
	glEnable (GL_DEPTH_TEST);				
    glDepthFunc (GL_LEQUAL);	
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

    //check for ogl 2.0
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
	glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_COLOR_MATERIAL);
    // set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);	

	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMateriali(GL_FRONT, GL_SHININESS, 24);

    font = new Font("res/fonts/arial.glf", _viewport);	

	//set light like in the phong shader..
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light0QuadraticAttenuation);
		
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	_timer = new Timer();
	
	glViewport(0, 0, wndWidth, wndHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)wndWidth/(GLfloat)wndHeight, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	_cam = new Camera(wndWidth, wndHeight);
	_cam->PositionCamera( 1, 0, 0,   0, 0, -1,   0, 1, 0);

    Vector3f lightDirection(0, -2, 1);
    Vector3f lightPosition(0, 4, -1);
    float attenuation = 0.01f;
    float lightRadius = 5.0f;
    
    DeferredDirectionalLight::DirectionalLightPtr directionalLight =
        DeferredDirectionalLight::create(lightDirection,									
										  nearPlane,
										  Utilities::nearPlaneSize(nearPlane, fov),
										  _cam);

    DeferredPointLight::PointLightPtr pointLight =
        DeferredPointLight::create(lightPosition,
									lightRadius,
									attenuation,
									Vector2i(wndWidth,
									    	 wndHeight),
									nearPlane,
                                    Utilities::nearPlaneSize(nearPlane, fov),
									_cam);

    _scene = new Scene();
    _scene->addDirectionalLight(directionalLight);
    _scene->addPointLight(pointLight);
    _deferredRenderer = new DeferredRenderer(_scene, _cam, wndWidth, wndHeight, fov, nearPlane);
}

int main(int argc, char* argv[])
{
	Vector2i viewportSize(wndWidth, wndHeight);
	_viewport = new Viewport(viewportSize);

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

