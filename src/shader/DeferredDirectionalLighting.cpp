#include "DeferredDirectionalLighting.h"

#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Utilities.h"

#include <Eigen/Geometry>
#include <Eigen/LU>


using namespace std;

namespace Ezr
{
	const string DeferredDirectionalLighting::vertexShaderPath("res/shaders/deferred/lightDirectional.vert");
	const string DeferredDirectionalLighting::fragmentShaderPath("res/shaders/deferred/lightDirectional.frag");
																								  
	DeferredDirectionalLighting::DeferredDirectionalLighting(const Vector3f* lightDirection,
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
		: _program(Utilities::loadFile(DeferredDirectionalLighting::vertexShaderPath),
				   Utilities::loadFile(DeferredDirectionalLighting::fragmentShaderPath)),
		  _lightDirection(lightDirection),
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
	}

	void DeferredDirectionalLighting::bind()
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
		
		Vector3f eye = _camera->GetCamPos();
		glUniform3f(glGetUniformLocation(program, "eye"), eye.x(), eye.y(), eye.z());
		
		//light in view space
		Vector4f lightDirection(_lightDirection->x(),
								_lightDirection->y(),
								_lightDirection->z(),
								0);
		Vector4f lightDirectionView = (*_modelView) * lightDirection;
		lightDirectionView.normalize();
		glUniform3f(glGetUniformLocation(program, "lightdir"),
					lightDirectionView.x(),
					lightDirectionView.y(),
					lightDirectionView.z());
			
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

	void DeferredDirectionalLighting::unbind()
	{
		_program.unbind();
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		
	}

	DeferredDirectionalLighting::~DeferredDirectionalLighting()
	{
	}

	void DeferredDirectionalLighting::setMatrices(const Matrix4f* modelView,
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
