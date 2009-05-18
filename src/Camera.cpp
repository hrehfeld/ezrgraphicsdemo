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


namespace EZR{

	//// CAMERA ////////////////////////////////////////////////////////////
	//
	// This is our camera constructor. Set the camera speed
	////////////////////////////////////////////////////////////////////////
	Camera::Camera(int screenWidth, int screenHeight)
	{
		m_camPosition.x() = 0.0;
		m_camPosition.y() = 0.0;
		m_camPosition.z() = 0.0;
		
		m_camView.x() = 0.0;
		m_camView.y() = 1.0;
		m_camView.z() = 0.5;
		
		m_camUpVector.x()= 0.0;
		m_camUpVector.y()= 0.0;
		m_camUpVector.z()= 1.0;

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
		m_camPosition.x()	= positionX;
		m_camPosition.y()	= positionY;
		m_camPosition.z()	= positionZ;

		m_camView.x() = viewX;
		m_camView.y() = viewY;
		m_camView.z() = viewZ;

		m_camUpVector.x() = upVectorX;
		m_camUpVector.y() = upVectorY;
		m_camUpVector.z() = upVectorZ;
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
		Vec2 mousePos(0,0);
		int middleX = m_rotCenterX;				
		int middleY = m_rotCenterY;				
		float angleY = 0.0f;							
		float angleZ = 0.0f;							
		static float currentRotX = 0.0f;
		
		mousePos.x() = mousePosX;
		mousePos.y() = mousePosY;
		
		// If our cursor is still in the middle, we never moved... so don't update the screen
		if( (mousePos.x() == middleX) && (mousePos.y() == middleY) ) return;

		//Set cursor position in the middle of the screen
		glutWarpPointer(middleX, middleY);

		angleY = static_cast<float>(middleX - mousePos.x()) / 1000.0f;		
		angleZ = static_cast<float>(middleY - mousePos.y()) / 1000.0f;		

		// Here we keep track of the current rotation (for up and down) so that
		// we can restrict the camera from doing a full 360 loop.
		currentRotX -= angleZ;  

		// If the current rotation (in radians) is greater than 1.0, we want to cap it.
		if(currentRotX > 1.0f)
			currentRotX = 1.0f;
		// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
		else if(currentRotX < -2.0f)
			currentRotX = -2.0f;

		// Otherwise, we can rotate the view around our position
		else
		{
			Vec3 vAxis = ((m_camView - m_camPosition).cross(m_camUpVector));
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
	void Camera::RotateView(const float angle, const float x, const float y, const float z)
	{
		//quaternion rotation
		//r' = q * r * q.conjugiert

		Eigen::Quaternionf temp, quat_view, result;
		
		float sinAngle = sinf(angle * 0.5f);
		temp.x() = x * sinAngle;
		temp.y() = y * sinAngle;
		temp.z() = z * sinAngle;
		temp.w() = cosf(angle * 0.5f);

		Vec3 view = m_camView - m_camPosition;

		quat_view.x() = view.x();
		quat_view.y() = view.y();
		quat_view.z() = view.z();
		quat_view.w() = 0;

		result = (temp * quat_view) * temp.conjugate();

		m_camView.x() = m_camPosition.x() + result.x();
		m_camView.y() = m_camPosition.y() + result.y();
		m_camView.z() = m_camPosition.z() + result.z();
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
		Vec3 viewVector = m_camView - m_camPosition;
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
		Vec3 vCross = (m_camView - m_camPosition).cross(m_camUpVector);

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
	Vec3 Camera::GetCamPos() const
	{
		return m_camPosition;
	}

	//// GET LOOK AT ///////////////////////////////////////////////////////
	//
	//	Returns the current position, the camera is looking at
	////////////////////////////////////////////////////////////////////////
	Vec3 Camera::GetLookAt() const
	{
		return m_camView;
	}

}