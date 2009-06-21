/*******************************************************************************
 *  Camera.h
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>, <mfreidank@uni-koblenz.de>
 ******************************************************************************/

#ifndef _CAMERA_H
#define _CAMERA_H

#include <Eigen/Core>

using namespace Eigen;

namespace Ezr{
		
	class Camera {
		
	public:
			
		//http://eigen.tuxfamily.org/dox/StructHavingEigenMembers.html
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW			
				
		Camera(int screenWidth, int screenHeight);

		Eigen::Vector3f& Position() {return m_camPosition;}
		Eigen::Vector3f& View() {return m_camView;}
		Eigen::Vector3f& UpVector() {return m_camUpVector;}
		Eigen::Vector3f& Strafe()	{return m_camStrafe;}
		
		//This function sets the camera's position and view and up vector.
		void PositionCamera(const float positionX, const float positionY, const float positionZ,
			 				const float viewX,     const float viewY,     const float viewZ,
							const float upVectorX, const float upVectorY, const float upVectorZ);

		/**
		 * FPS view rotation (pitch, yaw angles)
		 *
		 * @param relativeMouseMovement mouse movement relative to
		 * window center in fractions of window size.
		 */
		void rotateView(Vector2f& relativeMouseMovement);

		//Strafes the camera left and right depending on the speed (-/+)
		void StrafeCamera(const float camSpeed);

		//Move the camera forward or backward depending on the speed
		void MoveCamera(const float camSpeed);

		//If you push a movement key (w,a,s,d), the camera will move
		void CheckMovementKeys(const float frameInterval, bool w, bool s, bool a, bool d);

		//This function updates the camera position
		void UpdateCamPos(const float frameInterval, bool w, bool s, bool a, bool d);

		//Set gluLookAt()
		void CamLookAt();

		//Returns the current camera position
		Eigen::Vector3f& GetCamPos();

		//Returns the current position, the camera is looking at
		Eigen::Vector3f* GetLookAt();

	private:
		
		//camera position
		Eigen::Vector3f m_camPosition;					

		//camera view
		Eigen::Vector3f m_camView;						

		//camera up vector
		Eigen::Vector3f m_camUpVector;		
		
		//camera strafe vector
		Eigen::Vector3f m_camStrafe;
		
		//camera speed
		float m_camSpeed;

		//screen width and screen heigth
		int m_screenWidth, m_screenHeight;

		float _pitch;
		float _yaw;

		/**
		 * FPS view rotation (pitch, yaw angles)
		 */
		void RotateView();

		float sensitivity;
		bool invertMouseY;
		
	};
}

#endif
