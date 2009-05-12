#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include "GL/glut.h"
#include "camera.h"

using namespace Eigen;

int gWndWidth = 640;
int gWndHeight = 480;

GLdouble gNear = 0.1;
GLdouble gFar = 120.0;

Vector2i _mouseCoord = Vector2i(0,0);

Camera cam;

int _x, _y;




void display(void){
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
		
	cam.activateGL();
	glutSolidTorus(0.3, 2, 12, 36);
	
	glutSwapBuffers();

}

void reshape (int w, int h){
	cam.setViewport(0, 0, w, h);
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
	}
}

void mouse(int button, int state, int x, int y)
{
	float dx = static_cast<float>(x - _mouseCoord.x()) / static_cast<float>(cam.vpHeight());
	float dy = - static_cast<float>(y - _mouseCoord.y()) / static_cast<float>(cam.vpWidth());

	std::cout << dx << std::endl;

	 Quaternionf q = AngleAxisf( dx*M_PI, Vector3f::UnitY())
                            * AngleAxisf(-dy*M_PI, Vector3f::UnitX());
     cam.rotateAroundTarget(q);

	 _mouseCoord = Vector2i(x,y);
	 display();
}

void idle()
{
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
	
	cam.setTarget(Eigen::Vector3f(0, 0, 0));
	cam.setPosition(Eigen::Vector3f(10,10,10));

	reshape(gWndWidth, gWndHeight);
}


int main(int argc, char* argv[])
{
	//setup a GLUT window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(gWndWidth, gWndHeight);

	glutCreateWindow("Demo");
	
	init();

	//registering callback functions
	glutReshapeFunc(reshape);	
    glutDisplayFunc(display);
    glutIdleFunc(display);   
    glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	
	glutMainLoop();
		
	return 0;
}

