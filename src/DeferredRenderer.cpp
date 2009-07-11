#include "DeferredRenderer.h"

#include <Eigen/Geometry>

#include "OpenGL.h"
#include "Viewport.h"
#include "Fbo.h"
#include "Scene.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;


namespace Ezr
{
	const std::string DeferredRenderer::colorBuffer("color3_depth1");
	const std::string DeferredRenderer::normalBuffer("normal2");
	
	DeferredRenderer::DeferredRenderer(Viewport* view, Camera* camera)
		: _view(view)
	{
		Vector2i windowSize(_view->getWindowSize());
		_geometryPass = new Ezr::Fbo(windowSize.x(), windowSize.y(), Ezr::Fbo::Depth);
		_geometryPass->attachColorbuffer(DeferredRenderer::colorBuffer, GL_RGBA16F);
		_geometryPass->attachColorbuffer(DeferredRenderer::normalBuffer, GL_RGBA16F);
//		glDrawBuffer(GL_NONE);
		_geometryPass->checkFbo();
		_geometryPass->unbindFbo();


		_lightPass = new Ezr::Fbo(windowSize.x(), windowSize.y(), Fbo::None);
		_lightPass->attachColorbuffer("result", GL_RGBA16F);
//		glDrawBuffer(GL_NONE);
		
		_lightPass->checkFbo();
		_lightPass->unbindFbo();

		
	}

	DeferredRenderer::~DeferredRenderer()
	{
	}

	void DeferredRenderer::setScene(Scene* scene)
	{
		_scene = scene;
	}

	void DeferredRenderer::setLights(vector<Light*>& lights)
	{
		_lights = lights;
	}

	void DeferredRenderer::draw()
	{
		glPushAttrib(GL_VIEWPORT_BIT);
		Vector2i windowSize(_view->getWindowSize());
		glViewport(0, 0, windowSize.x(), windowSize.y());

		_geometryPass->bind();
        std::string b1("color3_depth1");
		_geometryPass->clearColorAttachment(b1, 0, 0, 0, 1);

		std::string b2("normal2");
		_geometryPass->clearColorAttachment(b2, 0.5f, 0.5f, 1, 1);
		glClear(GL_DEPTH_BUFFER_BIT);
		OpenGl::checkError("after FBO depth buffer clear");


		_geometryShader.bind();

		//tell the shader about uniforms
		GLuint program = _geometryShader.getProgram();
		glUniform1i(glGetUniformLocation(program, "colorMap"), 0);
		glUniform1i(glGetUniformLocationARB(program, "normalMap"), 1);

		GLfloat mvm[16];
		glGetFloatv (GL_MODELVIEW_MATRIX, mvm);
		GLfloat pm[16];
		glGetFloatv (GL_PROJECTION_MATRIX, pm);
		_modelView = Matrix4f(mvm);
		// Matrix4f modelViewMatrixInverse = _modelView.inverse();
	
		// Matrix4f projectionMatrix(pm);
		// Matrix4f projectionMatrixInverse = projectionMatrix.inverse();
	
		// Matrix3f normalMatrix(_modelView.block(0,0,3,3));
		// normalMatrix = normalMatrix.inverse();
		// normalMatrix.transposeInPlace();
		// Matrix3f normalMatrixInverse = normalMatrix.inverse();
			
		_scene->draw();


		_geometryShader.unbind();
		_geometryPass->unbindFbo();

		glPopAttrib();
		glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//light pass

		_lightPass->bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDepthMask(false);
			
		glActiveTexture(GL_TEXTURE0);
		_geometryPass->getColorAttachment(DeferredRenderer::colorBuffer)->bind();
		glEnable(GL_TEXTURE_2D);
		
		// //multitexturing
		glActiveTexture(GL_TEXTURE1);
		_geometryPass->getColorAttachment(DeferredRenderer::normalBuffer)->bind();
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);


		int passes = 0;
		for (vector<Light*>::iterator it = _lights.begin(); it < _lights.end(); ++it) {
			boolean blend = passes > 0;
			if (blend)
			{
				_lightPass->setDrawBuffer(resultAttachment);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				_lightPass->setDrawBuffers();
			}
			
			render(it);

			if (blend)
			{
				glDisable(GL_BLEND);
			}

		
			passes++;

		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);


		_lightPass->unbindFbo();


		//draw quad
		lightPass->getColorAttachment("result")->bind();
		glEnable(GL_TEXTURE_2D);
		glColor4f(1, 1, 1, 1);
		drawPass(_modelView, nearPlane, fov);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glDepthMask(true);
		
		
	}

	void setUniforms(GLuint program)
	{
		glUniform1i(glGetUniformLocation(program, "color3_depth1"), 0);
		glUniform1i(glGetUniformLocation(program, "normal2"), 1);
		glUniform1f(glGetUniformLocation(program, "planes.nearDistance"),_nearPlaneDistance);
		glUniform1f(glGetUniformLocation(program, "planes.nearSize"), _nearPlaneSize);

		Ezr::OpenGl::checkError("pointlight bind: before screensize");
		Vector2i screenSize(_view.getWindowSize());
		glUniform2i(glGetUniformLocation(program, "screenSize"),
					screenSize.x(), screenSize.y());
		Ezr::OpenGl::checkError("pointlight bind: screensize");
		

		Vector3f eye = _camera->GetCamPos();
		glUniform3f(glGetUniformLocation(program, "eye"), eye.x(), eye.y(), eye.z());
		

		// glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrix"),
		// 				   16, false, _modelView->data());
		// glUniformMatrix4fv(glGetUniformLocation(program, "modelViewMatrixInverse"),
		// 				   16, false, modelViewMatrixInverse.data());
		// glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"),
		// 				   16, false, projectionMatrix.data());
		// glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrixInverse"),
		// 				   16, false, projectionMatrixInverse.data());
		// glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"),
		// 				   9, false, normalMatrix.data());
		// glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrixInverse"),
		// 				   9, false, normalMatrixInverse.data());
	}

	void DeferredRenderer::render(DirectionalLight light) {
		std::cout << "directional light" << std::endl;

		_directionalLighting->bind(light, _modelView);

		
		drawPass(_modelView, nearPlane, fov);
		_directionalLighting->unbind();
		
	}

	void DeferredRenderer::render(Pointlight light) {
		std::cout << "point light" << std::endl;

		_pointLighting->bind();


		glEnable(GL_CULL_FACE);

		glPushMatrix();
		Vector3f lightPosition = light.getPosition();
		glTranslatef(lightPosition.x(), lightPosition.y(), lightPosition.z());
		glutSolidSphere(light.getRadius(), 24, 12);
		glPopMatrix();

		glDisable(GL_CULL_FACE);

		_pointLighting->unbind();

				// if (drawLightGeometry)
				// {
				// 	glEnable(GL_BLEND);
				// 	glBlendFunc(GL_ONE, GL_ONE);
				// 	glEnable(GL_CULL_FACE);
					
				// 	glPushMatrix();
				// 	glTranslatef(lightPosition->x(), lightPosition->y(), lightPosition->z());
				// 	glColor4f(0.1f, 0.1f, 0.1f, 1);
				// 	glutSolidSphere(lightRadius, 24, 12);
				// 	glColor4f(1, 1, 1, 1);
				// 	glPopMatrix();
					
				// 	glDisable(GL_CULL_FACE);
				// 	glDisable(GL_BLEND);

				// }			
		
		
	}

	void DeferredRenderer::render(Light light) {
		std::cout << "light" << std::endl;
	}
	

}
