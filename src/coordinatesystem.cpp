/* 
* \file coordinatesystem.cpp
*
* \author Marcus Zepp
*
* \version $Revision: 1.0 $
*
* \date $Date: 2014/03/26 14:16:20 $
*
* Contact: zeppfisj@mailbox.tu-berlin.de
*
* This file is part of sfs-visualizer.
*
* sfs-visualizer is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* sfs-visualizer is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with sfs-visualizer. If not, see <http://www.gnu.org/licenses/>.
*/ 

#include "coordinatesystem.h"
#include "application.h"
#include "cameracontrol.h"

namespace sfs_visualizer
{

CoordinateSystem::CoordinateSystem(void)
{
}

CoordinateSystem::~CoordinateSystem(void)
{
}

void CoordinateSystem::draw_impl()
{
    CameraControl cam = sfs_visualizer::Application::instance().getEngine().Camera();

    float offset = -0.7f;
    glTranslatef(offset, offset, 0.1f );

    glRotatef(cam.Spin_x(), 1, 0, 0);
    glRotatef(cam.Spin_y(), 0, 1, 0);
    glRotatef(cam.Spin_z(), 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    glScalef(0.1f,0.1f,0.1f);
    glBegin(GL_LINES);
    glColor4f(1.0f,0.0f,0.0f,1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f); // from
    glVertex3f(1.0f, 0.0f, 0.0f); // to
    glEnd(); // GL_LINES
    glBegin(GL_LINES);
    glColor4f(0.0f,1.0f,0.0f,1.0f);

    glVertex3f(0.0f, 0.0f, 0.0f); // from
    glVertex3f(0.0f, 1.0f, 0.0f); // to
    glEnd(); // GL_LINES
    glBegin(GL_LINES);
    glColor4f(0.0f,0.0f,1.0f,1.0f);

    glVertex3f(0.0f, 0.0f, 0.0f); // from
    glVertex3f(0.0f, 0.0f, 1.0f); // to
    glEnd(); // GL_LINES

}
}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='