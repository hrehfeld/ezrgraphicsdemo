#include "DeferredRenderer.h"
#include "OpenGL.h"
#include "Camera.h"
#include "Fbo.h"
#include "Utilities.h"
#include "Texture.h"
#include "Camera.h"
#include "Scene.h"
#include "MyMath.h"
#include "Image.h"
#include "Viewport.h"
#include "DeferredDirectionalLight.h"
#include "DeferredPointLight.h"
#include "DeferredDrawShader.h"

namespace Ezr{

    static const std::string colorMapPath("res/textures/lava.tga");
    static const std::string normalMapPath("res/textures/lava-normal.tga");

    DeferredRenderer::DeferredRenderer(Scene* scene, 
                                        Camera* camera, 
                                        int fboWidth, 
                                        int fboHeight,
                                        float fov,
                                        float nearPlane)
        :_cam(camera),
         _scene(scene),
         _fboWidth(fboWidth),
         _fboHeight(fboHeight),
         _fov(fov),
         _nearPlane(nearPlane),
         _renderDirectionalLights(true),
         _renderPointLights(true),
         _drawLightGeometry(false)
    {
        Image colorImage(colorMapPath);
        _colormap.reset(new Texture(colorImage));

	    Image normalImage(normalMapPath);
        boost::shared_ptr<Texture> normalmap(new Texture(normalImage));

        _deferredShader.reset(new DeferredDrawShader(_colormap, normalmap));

        //create the fbo for the g-buffer
        Texture* colorBuffer = new Texture(_fboWidth, _fboHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        Texture* normalBuffer = new Texture(_fboWidth, _fboHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        _gBuffer.reset(new Fbo(_fboWidth, _fboHeight, Fbo::Depth));
        _gBuffer->addColorAttachment(colorBuffer, "color");
        _gBuffer->addColorAttachment(normalBuffer, "normal");
	    glDrawBuffer(GL_NONE);
    	
         //error checking
	    _gBuffer->checkFbo();
	    _gBuffer->unbindFbo();

        //create the fbo for the lighting pass
        _lightBuffer.reset(new Fbo(_fboWidth, _fboHeight, Fbo::None));
        Texture* lightBuffer = new Texture(_fboWidth, _fboHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT);
        _lightBuffer->addColorAttachment(lightBuffer, "result");
	    glDrawBuffer(GL_NONE);
    	
        //error checking
	    _lightBuffer->checkFbo();
	    _lightBuffer->unbindFbo();
    }

    DeferredRenderer::~DeferredRenderer()
    {}

    void DeferredRenderer::render()
    {
        glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, _fboWidth, _fboHeight);
		OpenGl::checkError("after FBO binding");

		_gBuffer->bind();
        std::string b1("color");
		_gBuffer->clearColorAttachment(b1, 0, 0, 0, 1);

		std::string b2("normal");
		_gBuffer->clearColorAttachment(b2, 0.5f, 0.5f, 1, 1);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		OpenGl::checkError("after FBO depth buffer clear");

        glMatrixMode(GL_MODELVIEW);	
	    glLoadIdentity();

	    _cam->CamLookAt();
    	
	    _deferredShader->bind();
	    OpenGl::checkError("after shader binding");
	 	    
        // GLfloat pm[16];
	    //glGetFloatv (GL_PROJECTION_MATRIX, pm);
	    //Matrix4f modelViewMatrixInverse = modelViewMatrix.inverse();
    	
	    //Matrix4f projectionMatrix(pm);
	    //Matrix4f projectionMatrixInverse = projectionMatrix.inverse();
    	
	    /*Matrix3f normalMatrix(modelViewMatrix.block(0,0,3,3));
	    normalMatrix = normalMatrix.inverse();
	    normalMatrix.transposeInPlace();
	    Matrix3f normalMatrixInverse = normalMatrix.inverse();*/

	    glEnable(GL_CULL_FACE);
	    glCullFace(GL_FRONT);
	    glColor4f(1,0,0,1);

	    glutSolidTeapot(1);
        _scene->drawScene();

	    glColor4f(1,1,1,1);
	    glDisable(GL_CULL_FACE);
    	  
	    _deferredShader->unbind();
	    _gBuffer->unbindFbo(); 
		
	    glPopAttrib();
	    glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    _lightBuffer->bind();
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	    glClear(GL_COLOR_BUFFER_BIT);

	    glDepthMask(false);

        if(_renderDirectionalLights)
            renderDirectionalLights();

        if(_renderPointLights)
            renderPointLights();
    			
        _lightBuffer->unbindFbo();

	    //draw quad
	    _lightBuffer->getColorAttachment("result")->bind();
        glEnable(GL_TEXTURE_2D);
	    glColor4f(1, 1, 1, 1);
	    drawPass(_nearPlane, _fov);
	    glBindTexture(GL_TEXTURE_2D, 0);
	    glDisable(GL_TEXTURE_2D);
	    glDepthMask(true);
    }

    void DeferredRenderer::drawPass(float nearPlane, float fov)
    {
	    //nearplane, but with a small offset to ensure we don't get clipped
	    float nearP = nearPlane + 0.01f;

        float size = Utilities::nearPlaneSize(nearPlane + 0.01f, fov);

	    Vector4f quad[] = {
		    Vector4f(-size, -size, -nearP, 1),
		    Vector4f( size, -size, -nearP, 1),
		    Vector4f( size,  size, -nearP, 1),
		    Vector4f(-size,  size, -nearP, 1)
	    };

        
        GLfloat mvm[16];
	    glGetFloatv (GL_MODELVIEW_MATRIX, mvm);
	    Matrix4f modelView(mvm);
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

    void DeferredRenderer::renderDirectionalLights()
    {
         Scene::DirectionalLightList directionalLights = _scene->getDirectionalLights();
        for(Scene::DirectionalLightListIterator it = directionalLights.begin(); 
            it != directionalLights.end(); ++it)
        {        
            (*it)->setColorBuffer(_gBuffer->getColorAttachment("color"));
            (*it)->setNormalBuffer(_gBuffer->getColorAttachment("normal"));
            (*it)->bind();
            drawPass(_nearPlane, _fov);
            (*it)->unbind();
        }
    }

    void DeferredRenderer::renderPointLights()
    {
        Scene::PointLightList pointLights = _scene->getPointLights();
        for(Scene::PointLightListIterator it = pointLights.begin(); 
            it != pointLights.end(); ++it)
        {
            (*it)->setColorBuffer(_gBuffer->getColorAttachment("color"));
            (*it)->setNormalBuffer(_gBuffer->getColorAttachment("normal"));
            (*it)->bind();

	        _lightBuffer->setDrawBuffer("result");
	        glEnable(GL_BLEND);
	        glBlendFunc(GL_ONE, GL_ONE);
	        _lightBuffer->setDrawBuffers();

	        glEnable(GL_CULL_FACE);
	        (*it)->draw();
	        glDisable(GL_CULL_FACE);

	        glDisable(GL_BLEND);

	        (*it)->unbind();

	        if (_drawLightGeometry)
	        {
		        glEnable(GL_BLEND);
		        glBlendFunc(GL_ONE, GL_ONE);
		        glEnable(GL_CULL_FACE);
    			
                (*it)->drawGeometry();    			
		        
                glDisable(GL_CULL_FACE);
		        glDisable(GL_BLEND);
	        }	
	        OpenGl::checkError("pointlight unbind");
        }
    }
}