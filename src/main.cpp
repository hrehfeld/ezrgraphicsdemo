#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include "GL/glut.h"
#include "camera.h"

using namespace Eigen;

int wndWidth = 640;
int wndHeight = 480;

GLdouble gNear = 0.1;
GLdouble gFar = 100.0;

Vector2i _mouseCoord = Vector2i(0,0);

Camera* cam;

int _x, _y;

bool camMove, w, s, a, d = false;


void display(void){    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
	cam->CamLookAt();		
	glutSolidTorus(0.3, 2, 12, 36);	
	glutSwapBuffers();
}

void reshape (int w, int h){
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glMatrixMode (GL_MODELVIEW);
}


//keyboard functions
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
	}
}

void releaseKey(unsigned char key, int x, int y)
{
	if(key=='w') w=false;
    if(key=='s') s=false;
	if(key=='a') a=false;
    if(key=='d') d=false;
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		camMove = !state;
		//glutSetCursor(state ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
		if (state == GLUT_DOWN)
			cam->SetRotationCenter(x, y);
	}
}

void mouseMotion(int x, int y)
{
	if (camMove)
		cam->SetMouseView(x, y);
	display();
}


//init openGL
void init(void){
	glShadeModel (GL_SMOOTH);				
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);	
	glClearDepth (0.0f);
	glClearDepth (gFar);
	glEnable (GL_DEPTH_TEST);				
	glDepthFunc (GL_LEQUAL);				
	
	//glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glViewport(0, 0, wndWidth, wndHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)wndWidth/(GLfloat)wndHeight, 0.1 ,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cam = new Camera(wndWidth, wndHeight);
	cam->PositionCamera( 0, 0, 7,   0, 0, -1,   0, 1, 0);
	
	reshape(wndWidth, wndHeight);
}


int main(int argc, char* argv[])
{
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
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(releaseKey);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	
	glutMainLoop();
		
	return 0;
}

