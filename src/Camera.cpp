/*******************************************************************************
 *  Camera.cpp
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>, <mfreidank@uni-koblenz.de>
 ******************************************************************************/

#include "Camera.h"
#include "OpenGL.h"
#include <Eigen/Geometry>


namespace Ezr{

	//// CAMERA ////////////////////////////////////////////////////////////
	//
	// This is our camera constructor. Set the camera speed
	////////////////////////////////////////////////////////////////////////
	Camera::Camera(int screenWidth, int screenHeight)
	{
		m_camPosition << 0.0, 0.0, 0.0;
		m_camView << 0.0, 1.0, 0.5;
		m_camUpVector << 0.0, 0.0, 1.0;

		m_screenWidth  = screenWidth;
		m_screenHeight = screenHeight;

		m_camSpeed = 15.0f;
	}

	//// POSITION CAMERA ///////////////////////////////////////////////////
	//
	// This function sets the camera's position and view and up vector.
	////////////////////////////////////////////////////////////////////////
	void Camera::PositionCamera(const float positionX, const float positionY, const float positionZ,
				  				 const float viewX,     const float viewY,     const float viewZ,
								 const float upVectorX, const float upVectorY, const float upVectorZ)
	{
		m_camPosition << positionX, positionY, positionZ;
        m_camView << viewX, viewY, viewZ;
        m_camUpVector << upVectorX, upVectorY, upVectorZ;
	}

	//// SET ROTATION CENTER ///////////////////////////////////////////////
	//
	// Sets the current rotation center.
	////////////////////////////////////////////////////////////////////////
	void Camera::SetRotationCenter(int x, int y)
	{
		m_rotCenterX = x; 
		m_rotCenterY = y;
	}

	//// SET MOUSE VIEW ////////////////////////////////////////////////////
	//
	// Allows us to look around using the mouse.
	////////////////////////////////////////////////////////////////////////
	void Camera::SetMouseView(int mousePosX, int mousePosY)
	{
		int middleX = m_rotCenterX;				
		int middleY = m_rotCenterY;				
		static float currentRotX = 0.0f;
		
		Eigen::Vector2f mousePos(mousePosX, mousePosY);
		
		// If our cursor is still in the middle, we never moved... so don't update the screen
		if( (mousePos.x() == middleX) && (mousePos.y() == middleY) ) 
            return;

		//Set cursor position in the middle of the screen
		/** @todo 2009-06-20 23:19 hrehfeld    move somewhere central */
		glutWarpPointer(middleX, middleY);

		Eigen::Vector2f middle(middleX, middleY);

		float angleY = (static_cast<float>(middleX) - mousePos.x()) / 1000.0f;		
		float angleZ = (static_cast<float>(middleY) - mousePos.y()) / 1000.0f;		

		// Here we keep track of the current rotation (for up and down) so that
		// we can restrict the camera from doing a full 360 loop.
		currentRotX -= angleZ;

		// If the current rotation (in radians) is greater than 1.0, we want to cap it.
		if(currentRotX > 1.0f)
		{
			currentRotX = 1.0f;
		}
		// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
		else if(currentRotX < -2.0f)
		{
			currentRotX = -2.0f;
		}
		// Otherwise, we can rotate the view around our position
		else
		{
			Eigen::Vector3f vAxis = ((m_camView - m_camPosition).cross(m_camUpVector));
			vAxis.normalize();

			// Rotate around our perpendicular axis and along the y-axis
			RotateView(angleZ, vAxis.x(), vAxis.y(), vAxis.z());
		}

		// Rotate around the y axis no matter what the currentRotX is
		RotateView(angleY, 0, 1, 0);
 	}

	//// ROTATE VIEW ///////////////////////////////////////////////////////
	//
	// Rotates the view around the position using a quaternion rotation
	////////////////////////////////////////////////////////////////////////
	/*__attribute__((force_align_arg_pointer))*/ void Camera::RotateView(const float angle, const float x, const float y, const float z)
	{
		//quaternion rotation
		//r' = q * r * q.conjugiert

		
		float sinAngle = sinf(angle * 0.5f);

		std::cout << x * sinAngle << ", "
				  << y * sinAngle << ", "
				  << z * sinAngle << ", "
				  << cosf(angle * 0.5f)
				  << std::endl;

		std::cout << "Vector4f" << std::endl;

		Eigen::Vector4f temp(x * sinAngle,
								y * sinAngle,
								z * sinAngle,
								cosf(angle * 0.5f));
		// Eigen::Quaternionf temp(x * sinAngle,
		// 						y * sinAngle,
		// 						z * sinAngle,
		// 						cosf(angle * 0.5f));

		// Eigen::Vector3f view = m_camView - m_camPosition;
		// Eigen::Quaternionf quat_view(view.x(), view.y(), view.z(), 0);

		// Eigen::Quaternionf result = (temp * quat_view) * temp.conjugate();

		// m_camView.x() = m_camPosition.x() + result.x();
		// m_camView.y() = m_camPosition.y() + result.y();
		// m_camView.z() = m_camPosition.z() + result.z();
	}

	//// STRAFE CAMERA /////////////////////////////////////////////////////
	//
	// Strafes the camera left and right depending on the speed (-/+)
	////////////////////////////////////////////////////////////////////////
	void Camera::StrafeCamera(const float camSpeed)
	{	
		// Add the strafe vector to our position
		m_camPosition.x() += m_camStrafe.x() * camSpeed;
		m_camPosition.z() += m_camStrafe.z() * camSpeed;

		// Add the strafe vector to our view
		m_camView.x() += m_camStrafe.x() * camSpeed;
		m_camView.z() += m_camStrafe.z() * camSpeed;
	}

	//// MOVE CAMERA ///////////////////////////////////////////////////////
	//
	// Move the camera forward or backward depending on the speed
	////////////////////////////////////////////////////////////////////////
	void Camera::MoveCamera(const float camSpeed)
	{
		// Get the current view vector (the direction we are looking)
		Eigen::Vector3f viewVector = m_camView - m_camPosition;
		viewVector.normalize();

		m_camPosition.x() += viewVector.x() * camSpeed;		
		m_camPosition.y() += viewVector.y() * camSpeed;		
		m_camPosition.z() += viewVector.z() * camSpeed;		
		m_camView.x() += viewVector.x() * camSpeed;			
		m_camView.y() += viewVector.y() * camSpeed;			
		m_camView.z() += viewVector.z() * camSpeed;			
	}

	//// CHECK MOVEMENT KEYS ///////////////////////////////////////////////
	//
	// If you push a movement key (w,a,s,d), the camera will move
	////////////////////////////////////////////////////////////////////////
	void Camera::CheckMovementKeys(const float frameInterval, bool w, bool s, bool a, bool d)
	{
		float camSpeed = m_camSpeed * frameInterval;

		if(w) 
		{				
			MoveCamera(camSpeed);				
		}

		if(s) {			

			MoveCamera(-camSpeed);				
		}

		if(a) 
		{			
			StrafeCamera(-camSpeed);
		}

		if(d) 
		{			
			StrafeCamera(camSpeed);
		}
	}

	//// UPDATE CAMERA POSITION ////////////////////////////////////////////
	//
	// This function updates the camera position
	////////////////////////////////////////////////////////////////////////
	void Camera::UpdateCamPos(const float frameInterval, bool w, bool s, bool a, bool d) 
	{
		m_camStrafe = (m_camView - m_camPosition).cross(m_camUpVector);
		m_camStrafe.normalize();

		CheckMovementKeys(frameInterval,w,s,a,d);
	}

	//// CAMERA LOOK AT ////////////////////////////////////////////////////
	//
	//	Set gluLookAt()
	////////////////////////////////////////////////////////////////////////
	void Camera::CamLookAt()
	{
		gluLookAt(m_camPosition.x(), m_camPosition.y(), m_camPosition.z(),	
				  m_camView.x(),	   m_camView.y(),     m_camView.z(),	
				  m_camUpVector.x(), m_camUpVector.y(), m_camUpVector.z());
	}

	//// GET CAMERA POSITION ///////////////////////////////////////////////
	//
	//	Returns the current camera position
	////////////////////////////////////////////////////////////////////////
	Eigen::Vector3f& Camera::GetCamPos()
	{
		return m_camPosition;
	}

	//// GET LOOK AT ///////////////////////////////////////////////////////
	//
	//	Returns the current position, the camera is looking at
	////////////////////////////////////////////////////////////////////////
	Eigen::Vector3f& Camera::GetLookAt()
	{
		return m_camView;
	}

}
