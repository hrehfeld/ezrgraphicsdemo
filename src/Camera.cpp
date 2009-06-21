/*******************************************************************************
 *  Camera.cpp
 *
 *  Echtzeit-Rendering Framework 2009
 *
 *  Questions?: <fronc@uni-koblenz.de>, <mfreidank@uni-koblenz.de>
 ******************************************************************************/

#include "Camera.h"
#include "OpenGL.h"
#include "MyMath.h"
#include <Eigen/Geometry>
#include <algorithm>

using namespace Eigen;

namespace Ezr{

	//// CAMERA ////////////////////////////////////////////////////////////
	//
	// This is our camera constructor. Set the camera speed
	////////////////////////////////////////////////////////////////////////
	Camera::Camera(int screenWidth, int screenHeight) : sensitivity(1),
														invertMouseY(true),
														_pitch(0),
														_yaw(0)
	{
		m_camPosition << 0.0, 0.0, 0.0;
		m_camView << 0.0, 1.0, 0;
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

	void Camera::rotateView(Vector2f& relativeMouseMovement)
	{
		// const float minimumMouseMovement = 0.001f;
		// if (std::abs(relativeMouseMovement.x()) < minimumMouseMovement
		// 	&& std::abs(relativeMouseMovement.y()) < minimumMouseMovement)
		// {
		// 	return;
		// }
		
		// std::cout
		// 	<< "x" << relativeMouseMovement.x()
		// 	<< ", y" << relativeMouseMovement.y()
		// 	<< std::endl;
		
		_pitch += -relativeMouseMovement.y() * sensitivity * ((invertMouseY) ? -1 : 1);
		_pitch = MyMath::clamp(_pitch, -0.2f * MyMath::PI, 0.2f * MyMath::PI);

		_yaw += -relativeMouseMovement.x() * sensitivity;
		_yaw = MyMath::clampLoop(_yaw, 0, 2 * MyMath::PI);
		
		RotateView();
 	}

	//// ROTATE VIEW ///////////////////////////////////////////////////////
	//
	// Rotates the view around the position using a quaternion rotation
	////////////////////////////////////////////////////////////////////////
	/*__attribute__((force_align_arg_pointer))*/ void Camera::RotateView()
	{

		

		Eigen::Vector3f newView(1, 0, 0);
		Eigen::AngleAxisf pitchRotation(_pitch, Eigen::Vector3f::UnitZ());
		newView = pitchRotation * newView;
		// std::cout << "pitch rotation (" << _pitch << "): "
		// 		  << newView.x() <<  ", " << newView.y() << ", " <<  newView.z()
		// 		  << std::endl;
		Eigen::AngleAxisf yawRotation(_yaw, Eigen::Vector3f::UnitY());
		newView = yawRotation * newView;
		// std::cout << "yaw rotation (" << _yaw << "): "
		// 		  << newView.x() << ", " << newView.y() << ", " <<  newView.z()
		// 		  << std::endl;
		newView.normalize();
		m_camView = newView;
		
		// //quaternion rotation
		// //r' = q * r * q.conjugiert

		
		// float sinAngle = sinf(angle * 0.5f);
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

	}

	//// MOVE CAMERA ///////////////////////////////////////////////////////
	//
	// Move the camera forward or backward depending on the speed
	////////////////////////////////////////////////////////////////////////
	void Camera::MoveCamera(const float camSpeed)
	{
		m_camPosition.x() += m_camView.x() * camSpeed;		
		m_camPosition.y() += m_camView.y() * camSpeed;		
		m_camPosition.z() += m_camView.z() * camSpeed;		
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
		m_camStrafe = m_camView.cross(m_camUpVector);
		m_camStrafe.normalize();

		CheckMovementKeys(frameInterval,w,s,a,d);
	}

	//// CAMERA LOOK AT ////////////////////////////////////////////////////
	//
	//	Set gluLookAt()
	////////////////////////////////////////////////////////////////////////
	void Camera::CamLookAt()
	{
		Vector3f lookAt = m_camPosition + m_camView;

		gluLookAt(m_camPosition.x(), m_camPosition.y(), m_camPosition.z(),	
				  lookAt.x(),	   lookAt.y(),     lookAt.z(),	
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
	Eigen::Vector3f* Camera::GetLookAt()
	{
		Vector3f* lookAt = new Vector3f(m_camView + m_camPosition);

		return lookAt;
	}

}
