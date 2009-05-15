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

#include "gpuhelper.h"
// PLEASE don't look at this old code... ;)

#include <fstream>
#include <algorithm>

GpuHelper gpu;

GpuHelper::GpuHelper()
{
    mVpWidth = mVpHeight = 0;
    mCurrentMatrixTarget = 0;
    mInitialized = false;
}

GpuHelper::~GpuHelper()
{
}

void GpuHelper::pushProjectionMode2D(ProjectionMode2D pm)
{
    // switch to 2D projection
    pushMatrix(Matrix4f::Identity(),GL_PROJECTION);

    if(pm==PM_Normalized)
    {
        //glOrtho(-1., 1., -1., 1., 0., 1.);
    }
    else if(pm==PM_Viewport)
    {
        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        glOrtho(0., vp[2], 0., vp[3], -1., 1.);
    }

    pushMatrix(Matrix4f::Identity(),GL_MODELVIEW);
}

void GpuHelper::popProjectionMode2D(void)
{
    popMatrix(GL_PROJECTION);
    popMatrix(GL_MODELVIEW);
}


