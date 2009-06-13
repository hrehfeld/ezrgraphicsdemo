#include <iostream>
#include "OpenGL.h"
#include "Camera.h"
#include "Fbo.h"
#include "Scene.h"
#include "Time.h"

using namespace Eigen;

int wndWidth = 1024;
int wndHeight = 768;

GLdouble gNear = 0.1;
GLdouble gFar = 100.0;

Ezr::Camera* cam;
Ezr::Fbo* fbo;
Ezr::Scene* scene;
Ezr::Time* time;
int _x, _y;

bool camMove, useFbo, w, s, a, d = false;
GLuint textureID;
GLuint depthbuffer;


void display(void){    
 
	cam->UpdateCamPos(time->GetFrameInterval(), w, s, a, d);
	time->CalculateFrameRate();
	
	if(useFbo)
	{
		fbo->bind();
	}

	//glPushAttrib(GL_VIEWPORT_BIT);
	//glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
	cam->CamLookAt();
	
	glutSolidTeapot(1);
	//scene->drawScene();
		
	if(useFbo)
	{
		//glPopAttrib();
		fbo->unbindFbo();
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

		//draw a screensize rectangle with the fbo rendered scene as texture on it
		glMatrixMode(GL_MODELVIEW);	
		glPushMatrix();
		glLoadIdentity();						
		glMatrixMode (GL_PROJECTION); 
		glPushMatrix (); 
		glLoadIdentity ();
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBegin (GL_QUADS); 
			glNormal3f( 0.0f, 0.0f, 1.0);
			glTexCoord2f(0.0f, 0.0f); glVertex3i (-1, -1, -1); 
			glTexCoord2f(1.0f, 0.0f); glVertex3i (1, -1, -1); 
			glTexCoord2f(1.0f, 1.0f); glVertex3i (1, 1, -1); 
			glTexCoord2f(0.0f, 1.0f); glVertex3i (-1, 1, -1); 
		glEnd ();

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);

		glPopMatrix();
		glMatrixMode (GL_MODELVIEW); 
		glPopMatrix(); 
	}
	std::cout << time->GetFramesPerSecond() << std::endl;
	glutSwapBuffers();
}

void reshape (int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)(w)/h , 1.0f, 100.0f); // FOV, AspectRatio, NearClip, FarClip

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
			useFbo = true;
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
		camMove = !state;
		//glutSetCursor(state ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
		if (state == GLUT_DOWN){
			cam->SetRotationCenter(x, y);
		}
	}
}

void mouseMotion(int x, int y)
{
	if (camMove){
		cam->SetMouseView(x, y);
		//std::cout << x << "," << y << std::endl;
	}
}


//init openGL
void init(void){
	glShadeModel (GL_SMOOTH);				
	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);	
	glClearDepth (0.0f);
	glClearDepth (gFar);
	glEnable (GL_DEPTH_TEST);				
	glDepthFunc (GL_LEQUAL);	

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

	//if(useFbo)
	//{
		fbo = new Ezr::Fbo(wndWidth, wndHeight, true);
		fbo->bind();

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D,textureID);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, wndWidth, wndHeight, 0, GL_RGB, GL_FLOAT, NULL);
		
		fbo->attachFboTexture(GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureID);
		fbo->attachRBO(GL_DEPTH_ATTACHMENT_EXT);
		
		fbo->checkFbo();
		fbo->unbindFbo();

		//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	//}

	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	//glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {3.0, 3.0, 2.0, 0.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	time = new Ezr::Time();
	
	glViewport(0, 0, wndWidth, wndHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)wndWidth/(GLfloat)wndHeight, 0.1 ,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	

	cam = new Ezr::Camera(wndWidth, wndHeight);
	cam->PositionCamera( 0, 0, 7,   0, 0, -1,   0, 1, 0);

	//scene = new Ezr::Scene();
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
	glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(releaseKey);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	
	glutMainLoop();
		
	return 0;
}

