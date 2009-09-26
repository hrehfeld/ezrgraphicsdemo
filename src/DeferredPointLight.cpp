#include "DeferredPointLight.h"

#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Utilities.h"

#include <iostream>
#include <Eigen/Geometry>
#include <Eigen/LU>


using namespace std;
using namespace Eigen;

namespace Ezr
{
	const string DeferredPointLight::vertexShaderPath("res/shaders/lightPoint.vert");
	const string DeferredPointLight::fragmentShaderPath("res/shaders/lightPoint.frag");
	const float DeferredPointLight::epsilon(0.5f);
																								  
	DeferredPointLight::DeferredPointLight(const Vector3f& lightPosition,
										     const float lightRadius,
										     const float attenuation,
										     const Vector2i& screenSize,
										     const float nearPlaneDistance,
										     const float nearSize,
										     Camera* camera)
		: _program(Utilities::loadFile(DeferredPointLight::vertexShaderPath),
				   Utilities::loadFile(DeferredPointLight::fragmentShaderPath)),
		  _lightPosition(lightPosition),
		  _lightRadius(lightRadius),
		  _attenuation((1.0f - epsilon) / (lightRadius*lightRadius*epsilon)),
		  _screenSize(screenSize),
		  _nearPlaneDistance(nearPlaneDistance),
		  _nearPlaneSize(nearSize),
		  _camera(camera)		
	{
		std::cout << (1.0f - epsilon) / (lightRadius*lightRadius*epsilon) << std::endl;
	}

    DeferredPointLight::~DeferredPointLight()
    {}	

    void DeferredPointLight::draw()
    {
          glPushMatrix();
	      glTranslatef(_lightPosition.x(), _lightPosition.y(), _lightPosition.z());
	      glutSolidSphere(_lightRadius, 24, 12);
	      glPopMatrix();
    }

    void DeferredPointLight::drawGeometry()
    {
        glPushMatrix();
        glTranslatef(_lightPosition.x(), _lightPosition.y(), _lightPosition.z());
        glColor4f(0.1f, 0.1f, 0.1f, 1);
        glutSolidSphere(_lightRadius, 24, 12);
        glColor4f(1, 1, 1, 1);
        glPopMatrix();
    }

	void DeferredPointLight::bind()
	{
		glActiveTexture(GL_TEXTURE0);
		_color->bind();
		glEnable(GL_TEXTURE_2D);
		
		// //multitexturing
		glActiveTexture(GL_TEXTURE1);
		_normal->bind();
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		
		_program.bind();
		
		//tell the shader about uniforms
		GLint program = _program.getProgram();
		
		glUniform1i(glGetUniformLocation(program, "color"), 0);
		glUniform1i(glGetUniformLocation(program, "normal"), 1);
		glUniform1f(glGetUniformLocation(program, "planes.nearDistance"),_nearPlaneDistance);
		glUniform1f(glGetUniformLocation(program, "planes.nearSize"), _nearPlaneSize);
		
		glUniform1f(glGetUniformLocation(program, "attenuation"), _attenuation);
		glUniform1f(glGetUniformLocation(program, "lightRadius"), _lightRadius);

		Ezr::OpenGl::checkError("pointlight bind: before screensize");
		glUniform2i(glGetUniformLocation(program, "screenSize"),
                        _screenSize.x(), _screenSize.y());
		Ezr::OpenGl::checkError("pointlight bind: screensize");
		
		Vector3f eye = _camera->GetCamPos();
		glUniform3f(glGetUniformLocation(program, "eye"), eye.x(), eye.y(), eye.z());
		
        GLfloat mvm[16];
	    glGetFloatv (GL_MODELVIEW_MATRIX, mvm);
	    Matrix4f modelView(mvm);

		//light in view space
		Vector4f lightPosition(_lightPosition.x(),
							   _lightPosition.y(),
							   _lightPosition.z(),
							   1);
		Vector4f lightPositionView = modelView * lightPosition;
		//lightPositionView *= 1.0/lightPositionView.w();

    	glUniform3f(glGetUniformLocation(program, "light"),
                    lightPositionView.x(),
					lightPositionView.y(),
					lightPositionView.z());		
	}

	void DeferredPointLight::unbind()
	{
		_program.unbind();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
    }

    DeferredPointLight::PointLightPtr DeferredPointLight::create(const Vector3f& lightPosition,
							                                      float lightRadius,
							                                      float attenuation,
							                                      const Vector2i& screenSize,
							                                      float nearPlaneDistance,
							                                      float nearSize,
							                                      Camera* camera)
    {
        PointLightPtr toRet(new DeferredPointLight(lightPosition,
							                        lightRadius,
							                        attenuation,
							                        screenSize,
							                        nearPlaneDistance,
							                        nearSize,
							                        camera));
        return toRet;
    }   
}
