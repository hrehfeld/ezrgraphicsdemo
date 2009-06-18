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

namespace Ezr{

	typedef Eigen::Vector3f Vec3;
	typedef Eigen::Vector2f Vec2;
	
	class Camera {
		
	public:
		
		Camera(int screenWidth, int screenHeight);

		Vec3& Position() {	return m_camPosition;	}
		Vec3& View()	    {	return m_camView;		}
		Vec3& UpVector() {	return m_camUpVector;	}
		Vec3& Strafe()	{	return m_camStrafe;		}
		
		//This function sets the camera's position and view and up vector.
		void PositionCamera(const float positionX, const float positionY, const float positionZ,
			 				const float viewX,     const float viewY,     const float viewZ,
							const float upVectorX, const float upVectorY, const float upVectorZ);

		//Allows us to look around using the mouse.
		void SetMouseView(int mousePosX, int mousePosY);

		//Rotates the view around the position using a quaternion rotation
		void RotateView(const float angle, const float X, const float Y, const float Z);

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
		Vec3& GetCamPos();

		//Returns the current position, the camera is looking at
		Vec3& GetLookAt();

		//sets the center of the current rotation axis
		void SetRotationCenter(int, int);

		//http://eigen.tuxfamily.org/dox/StructHavingEigenMembers.html
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW			

	private:
		
		//camera position
		Vec3 m_camPosition;					

		//camera view
		Vec3 m_camView;						

		//camera up vector
		Vec3 m_camUpVector;		
		
		//camera strafe vector
		Vec3 m_camStrafe;
		
		//camera speed
		float m_camSpeed;

		//screen width and screen heigth
		int m_screenWidth, m_screenHeight;

		//center of rotation
		int m_rotCenterX, m_rotCenterY;
	};
}

#endif
