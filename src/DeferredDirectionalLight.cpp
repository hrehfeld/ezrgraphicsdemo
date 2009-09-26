#include "DeferredDirectionalLight.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Utilities.h"
#include <Eigen/Geometry>
#include <Eigen/LU>

using namespace std;
using namespace Eigen;

namespace Ezr
{
	const string DeferredDirectionalLight::vertexShaderPath("res/shaders/lightDirectional.vert");
	const string DeferredDirectionalLight::fragmentShaderPath("res/shaders/lightDirectional.frag");
																								  
	DeferredDirectionalLight::DeferredDirectionalLight(const Vector3f& lightDirection,
													const float nearPlaneDistance,
													const float nearSize,
													Camera* camera)
		: _program(Utilities::loadFile(DeferredDirectionalLight::vertexShaderPath),
				   Utilities::loadFile(DeferredDirectionalLight::fragmentShaderPath)),
		  _lightDirection(lightDirection),
		  _nearPlaneDistance(nearPlaneDistance),
		  _nearPlaneSize(nearSize),
		  _camera(camera)		 
	{}

    DeferredDirectionalLight::~DeferredDirectionalLight()
	{}	

	void DeferredDirectionalLight::bind()
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
		
		Vector3f eye = _camera->GetCamPos();
		glUniform3f(glGetUniformLocation(program, "eye"), eye.x(), eye.y(), eye.z());
		
        GLfloat mvm[16];
	    glGetFloatv (GL_MODELVIEW_MATRIX, mvm);
	    Matrix4f modelView(mvm);

		//light in view space
		Vector4f lightDirection(_lightDirection.x(),
								_lightDirection.y(),
								_lightDirection.z(),
								0);
		Vector4f lightDirectionView = modelView * lightDirection;
		lightDirectionView.normalize();
		glUniform3f(glGetUniformLocation(program, "lightdir"),
					lightDirectionView.x(),
					lightDirectionView.y(),
					lightDirectionView.z());				
	}

	void DeferredDirectionalLight::unbind()
	{
		_program.unbind();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
	}
	
    DeferredDirectionalLight::DirectionalLightPtr 
        DeferredDirectionalLight::create(const Vector3f& lightDirection,
									      float nearPlaneDistance,
									      float nearSize,
									      Camera* camera)
    {
        DirectionalLightPtr toRet(new DeferredDirectionalLight(lightDirection,
								                                nearPlaneDistance,
									                            nearSize,
									                            camera));
        return toRet;
    }
}
