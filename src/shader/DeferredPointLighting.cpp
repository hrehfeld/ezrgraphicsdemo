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
	const float DeferredPointLighting::epsilon(0.5f);
																								  
	DeferredPointLighting::DeferredPointLighting()
		: _program(Utilities::loadFile(DeferredPointLighting::vertexShaderPath),
				   Utilities::loadFile(DeferredPointLighting::fragmentShaderPath)),
	{
		std::cout << (1.0f - epsilon) / (lightRadius*lightRadius*epsilon) << std::endl;
	}

	void DeferredPointLighting::bind(const PointLight& light, const Matrix4f& modelView)
	{

		float lightRadius = light.getRadius();

		float attenuation = ((1.0f - epsilon) / (lightRadius*lightRadius*epsilon));
		
		_program.bind();
		
		//tell the shader about uniforms
		GLint program = _program.getProgram();
		
		glUniform1f(glGetUniformLocation(program, "attenuation"), attenuation);
		glUniform1f(glGetUniformLocation(program, "lightRadius"), lightRadius);

		Vector3f lightPos = light.getPosition();

		//light in view space
		Vector4f lightPosition(lightPos->x(),
							   lightPos->y(),
							   lightPos->z(),
							   1);
		Vector4f lightPositionView = (modelView) * lightPosition;
		//lightPositionView *= 1.0/lightPositionView.w();

		glUniform3f(glGetUniformLocation(program, "light"),
					lightPositionView.x(),
					lightPositionView.y(),
					lightPositionView.z());
	}

	void DeferredPointLighting::unbind()
	{
		_program.unbind();
		
	}

	DeferredPointLighting::~DeferredPointLighting()
	{
	}
}
