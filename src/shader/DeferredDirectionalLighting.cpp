#include "DeferredDirectionalLighting.h"

#include "DirectionalLight.h"
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
																								  
	DeferredDirectionalLighting::DeferredDirectionalLighting()
		: _program(Utilities::loadFile(DeferredDirectionalLighting::vertexShaderPath),
				   Utilities::loadFile(DeferredDirectionalLighting::fragmentShaderPath))
	{
	}

	void DeferredDirectionalLighting::bind(const DirectionalLight& light, const Matrix4f& modelView)
	{
		_program.bind();
		
		//tell the shader about uniforms
		GLint program = _program.getProgram();


		Vector3f lightDir = light.getDirection();

		//light in view space
		Vector4f lightDirection(lightDir.x(),
								lightDir.y(),
								lightDir.z(),
								0);
		Vector4f lightDirectionView = (modelView) * lightDirection;
		lightDirectionView.normalize();
		glUniform3f(glGetUniformLocation(program, "lightdir"),
					lightDirectionView.x(),
					lightDirectionView.y(),
					lightDirectionView.z());
	}

	void DeferredDirectionalLighting::unbind()
	{
		_program.unbind();
	}

	DeferredDirectionalLighting::~DeferredDirectionalLighting()
	{
	}
}
