//##ifdef WIN32
//#include <windows.h>
//#endif
#include <iostream>
#include "GL/glut.h"

int gWndWidth = 640;
int gWndHeight = 480;

GLdouble gNear = 0.1;
GLdouble gFar = 120.0;




void display(void){
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);

	glutSolidTorus(0.3, 2, 12, 36);
	
	glutSwapBuffers();

}

void reshape (int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (float)w / (float)h, gNear, gFar);
	
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
	/*switch(button)
	{
		case GL_LEFT_BUTTON:
			if(state == GLUT_DOWN)
			{

			}

	}*/
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

