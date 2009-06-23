#include <iostream>
#include "OpenGL.h"
#include "Camera.h"
#include "Fbo.h"
#include "Scene.h"
#include "Timer.h"
#include "Utilities.h"
#include "gl/GlBindShader.h"
#include "Viewport.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

using namespace Eigen;


//non 2^n sizes make FBOs VERY slow
int wndWidth = 512;
int wndHeight = 512;

GLdouble gNear = 0.1;
GLdouble gFar = 100.0;

float fullscreenQuadSize = 1.0;

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
GLuint textureID;
GLuint depthbuffer;

static const std::string deferredVertexShaderPath("res/shaders/deferred/basic.vert");
static const std::string deferredFragmentShaderPath("res/shaders/deferred/basic.frag");
//static const std::string deferredVertexShaderPath("res/shaders/phong.vert");
//static const std::string deferredFragmentShaderPath("res/shaders/phong.frag");

static const std::string colorMapPath("res/textures/Feldweg00Mitte.tga");

GLuint colormap;

static Ezr::GlBindShader* deferredShader;

void init();
void load();
void loadImages();


void display(void){    
 
	cam->UpdateCamPos(timer->GetFrameInterval(), w, s, a, d);
	timer->CalculateFrameRate();
	
	if(useFbo)
	{
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, wndWidth, wndHeight);
		fbo->bind();
	}


	glClearColor(1.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
	cam->CamLookAt();

	if (useShader)
	{
		deferredShader->bind();
	}
	//draw geometry
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, colormap);
	glutSolidTeapot(1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	//scene->drawScene();
	
	if (useShader)
	{
		deferredShader->unbind();
	}
	
	if(useFbo)
	{
		fbo->unbindFbo();
		
		glPopAttrib();
		//glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

		//draw a screensize rectangle with the fbo rendered scene as texture on it
		glMatrixMode(GL_MODELVIEW);	
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode (GL_PROJECTION); 
		glPushMatrix (); 
		glLoadIdentity ();
		float orthoSize = 1.0 / fullscreenQuadSize;
		glOrtho(-orthoSize, orthoSize, -orthoSize, orthoSize, -1.0, 1.0);

		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

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

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_LIGHTING);
		
		glMatrixMode (GL_PROJECTION); 
		glPopMatrix();
		glMatrixMode (GL_MODELVIEW); 
		glPopMatrix();
	}
//	std::cout << timer->GetFramesPerSecond() << std::endl;
	glutSwapBuffers();
}

void reshape (int w, int h){
	Vector2i windowSize(w, h);
	window->setWindowSize(windowSize);
	
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

	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, wndWidth, wndHeight, 0, GL_RGB, GL_FLOAT, NULL);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, wndWidth, wndHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmapEXT(GL_TEXTURE_2D);

		fbo = new Ezr::Fbo(wndWidth, wndHeight);
		fbo->bind();

		fbo->attachFboTexture(GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureID);
		//fbo->attachRBO(GL_DEPTH_ATTACHMENT_EXT);
		//glDrawBuffer(GL_NONE);
		
		fbo->checkFbo();
		fbo->unbindFbo();


	}

	

	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {3.0, 3.0, 2.0, 0.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
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

void load()
{
	deferredShader = new Ezr::GlBindShader(Ezr::Utilities::loadFile(deferredVertexShaderPath),
	 									   Ezr::Utilities::loadFile(deferredFragmentShaderPath));

	loadImages();
}

void loadImages()
{
	ilInit();

	ILuint ImageName;
	ilGenImages(1, &ImageName);
	
	ilBindImage(ImageName);
	ilLoadImage(colorMapPath.c_str());

	ILenum Error;
	while ((Error = ilGetError()) != IL_NO_ERROR) {
		printf("%d: %s/n", Error, iluErrorString(Error)); 
	}

	ilutRenderer(ILUT_OPENGL);
	ilutEnable(ILUT_OPENGL_CONV);
	ilutGLBuildMipmaps();

	glGenTextures(1, &colormap);
	glBindTexture(GL_TEXTURE_2D, colormap);

	//let the gpu generate mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


	ILinfo img;
	iluGetImageInfo(&img);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.Width, img.Height, 0, img.Format, img.Type, img.Data );
	glBindTexture(GL_TEXTURE_2D, NULL);

	ilDeleteImages(1, &ImageName);	
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

