#include "DeferredPointLighting.h"

#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Utilities.h"

#include <iostream>
#include <Eigen/Geometry>
#include <Eigen/LU>


using namespace std;

namespace Ezr
{
	const string DeferredPointLighting::vertexShaderPath("res/shaders/deferred/lightPoint.vert");
	const string DeferredPointLighting::fragmentShaderPath("res/shaders/deferred/lightPoint.frag");
	const float DeferredPointLighting::epsilon(0.1f);
																								  
	DeferredPointLighting::DeferredPointLighting(const Vector3f* lightPosition,
												 const float lightRadius,
												 const float attenuation,
												 const Vector2i* screenSize,
												 const Texture* color3_depth1,
												 const Texture* normal2,
												 const float nearPlaneDistance,
												 const float nearSize,
												 Camera* camera,
												 const Matrix4f* modelView,
												 const Matrix4f* modelViewInverse,
												 const Matrix3f* normal,
												 const Matrix3f* normalInverse,
												 const Matrix4f* projection,
												 const Matrix4f* projectionInverse)
		: _program(Utilities::loadFile(DeferredPointLighting::vertexShaderPath),
				   Utilities::loadFile(DeferredPointLighting::fragmentShaderPath)),
		  _lightPosition(lightPosition),
		  _lightRadius(lightRadius),
		  _attenuation((1.0f - epsilon) / (lightRadius*lightRadius*epsilon)),
		  _screenSize(screenSize),
		  _color3_depth1(color3_depth1),
		  _normal2(normal2),
		  _nearPlaneDistance(nearPlaneDistance),
		  _nearPlaneSize(nearSize),
		  _camera(camera),
		  _modelView(modelView),
		  _modelViewInverse(modelViewInverse),
		  _normal(normal),
		  _normalInverse(normalInverse),
		  _projection(projection),
		  _projectionInverse(projectionInverse)
	{
		std::cout << (1.0f - epsilon) / (lightRadius*lightRadius*epsilon) << std::endl;
	}

	void DeferredPointLighting::bind()
	{
		glActiveTexture(GL_TEXTURE0);
		_color3_depth1->bind();
		glEnable(GL_TEXTURE_2D);
		
		// //multitexturing
		glActiveTexture(GL_TEXTURE1);
		_normal2->bind();
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		
		_program.bind();
		
		//tell the shader about uniforms
		GLint program = _program.getProgram();
		
		glUniform1i(glGetUniformLocation(program, "color3_depth1"), 0);
		glUniform1i(glGetUniformLocation(program, "normal2"), 1);
		glUniform1f(glGetUniformLocation(program, "planes.nearDistance"),_nearPlaneDistance);
		glUniform1f(glGetUniformLocation(program, "planes.nearSize"), _nearPlaneSize);
		
		glUniform1f(glGetUniformLocation(program, "attenuation"), _attenuation);
		glUniform1f(glGetUniformLocation(program, "lightRadius"), _lightRadius);

		Ezr::OpenGl::printGlError("pointlight bind: before screensize");
		glUniform2i(glGetUniformLocation(program, "screenSize"),
					_screenSize->x(), _screenSize->y());
		Ezr::OpenGl::printGlError("pointlight bind: screensize");
		

		Vector3f eye = _camera->GetCamPos();
		glUniform3f(glGetUniformLocation(program, "eye"), eye.x(), eye.y(), eye.z());
		
		//light in view space
		Vector4f lightPosition(_lightPosition->x(),
							   _lightPosition->y(),
							   _lightPosition->z(),
							   1);
		Vector4f lightPositionView = (*_modelView) * lightPosition;
		lightPositionView.normalize();
		glUniform3f(glGetUniformLocation(program, "light"),
					lightPositionView.x(),
					lightPositionView.y(),
					lightPositionView.z());
			
		glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"),
						   16, false, _modelView->data());
		glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrixInverse"),
						   16, false, _modelViewInverse->data());
		glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"),
						   16, false, _projection->data());
		glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrixInverse"),
						   16, false, _projectionInverse->data());
		glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"),
						   9, false, _normal->data());
		glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrixInverse"),
						   9, false, _normalInverse->data());
	}

	void DeferredPointLighting::unbind()
	{
		_program.unbind();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		
	}

	DeferredPointLighting::~DeferredPointLighting()
	{
	}

	void DeferredPointLighting::setMatrices(const Matrix4f* modelView,
												  const Matrix4f* modelViewInverse,
												  const Matrix3f* normal,
												  const Matrix3f* normalInverse,
												  const Matrix4f* projection,
												  const Matrix4f* projectionInverse)
	{
		_modelView = modelView;
		_modelViewInverse = modelViewInverse;
		_normal = normal;
		_normalInverse = normalInverse;
		_projection = projection;
		_projectionInverse = projectionInverse;
	}
	
}
