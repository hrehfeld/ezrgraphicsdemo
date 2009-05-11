// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2008 Gael Guennebaud <g.gael@free.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_CAMERA_H
#define EIGEN_CAMERA_H

#include <Eigen/Geometry>
// #include <frame.h>

class Frame
{
  public:
    inline Frame(const Eigen::Vector3f& pos = Eigen::Vector3f::Zero(),
                 const Eigen::Quaternionf& o = Eigen::Quaternionf())
      : orientation(o), position(pos)
    {}
    Frame lerp(float alpha, const Frame& other) const
    {
      return Frame((1.f-alpha)*position + alpha * other.position,
                   orientation.slerp(alpha,other.orientation));
    }

    Eigen::Quaternionf orientation;
    Eigen::Vector3f position;
};

class Camera
{
  public:

    Camera(void);
    
    Camera(const Camera& other);
    
    virtual ~Camera();
    
    Camera& operator=(const Camera& other);
    
    void setViewport(unsigned int offsetx, unsigned int offsety, unsigned int width, unsigned int height);
    void setViewport(unsigned int width, unsigned int height);
    
    inline unsigned int vpX(void) const { return mVpX; }
    inline unsigned int vpY(void) const { return mVpY; }
    inline unsigned int vpWidth(void) const { return mVpWidth; }
    inline unsigned int vpHeight(void) const { return mVpHeight; }

    inline float fovY(void) const { return mFovY; }
    void setFovY(float value);
    
    void setPosition(const Eigen::Vector3f& pos);
    inline const Eigen::Vector3f& position(void) const { return mFrame.position; }

    void setOrientation(const Eigen::Quaternionf& q);
    inline const Eigen::Quaternionf& orientation(void) const { return mFrame.orientation; }

    void setFrame(const Frame& f);
    const Frame& frame(void) const { return mFrame; }
    
    void setDirection(const Eigen::Vector3f& newDirection);
    Eigen::Vector3f direction(void) const;
    void setUp(const Eigen::Vector3f& vectorUp);
    Eigen::Vector3f up(void) const;
    Eigen::Vector3f right(void) const;
    
    void setTarget(const Eigen::Vector3f& target);
    inline const Eigen::Vector3f& target(void) { return mTarget; }
    
    const Eigen::Transform3f& viewMatrix(void) const;
    const Eigen::Matrix4f& projectionMatrix(void) const;
    
    void rotateAroundTarget(const Eigen::Quaternionf& q);
    void localRotate(const Eigen::Quaternionf& q);
    void zoom(float d);
    
    void localTranslate(const Eigen::Vector3f& t);
    
    /** Setup OpenGL matrices and viewport */
    void activateGL(void);
    
    Eigen::Vector3f unProject(const Eigen::Vector2f& uv, float depth, const Eigen::Matrix4f& invModelview) const;
    Eigen::Vector3f unProject(const Eigen::Vector2f& uv, float depth) const;
    
  protected:
    void updateViewMatrix(void) const;
    void updateProjectionMatrix(void) const;

  protected:

    unsigned int mVpX, mVpY;
    unsigned int mVpWidth, mVpHeight;

    Frame mFrame;
    
    mutable Eigen::Transform3f mViewMatrix;
    mutable Eigen::Matrix4f mProjectionMatrix;

    mutable bool mViewIsUptodate;
    mutable bool mProjIsUptodate;

    // used by rotateAroundTarget
    Eigen::Vector3f mTarget;
    
    float mFovY;
    float mNearDist;
    float mFarDist;
};

#endif // EIGEN_CAMERA_H