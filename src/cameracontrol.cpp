/* 
* \file cameracontrol.cpp
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

#include "cameracontrol.h"
#include "stdincl.h"
#include "propertymanager.h"

namespace sfs_visualizer
{
       
  CameraControl::CameraControl(void)
      :_rot_y (0), _rot_x(0),_rot_z(0), _zoom(0), _old_x(0),_old_y(0),_move_z(0),
      _xRotProp (PropertyManager::instance().addProperty("XRot",0.0f)),
      _yRotProp (PropertyManager::instance().addProperty("YRot",0.0f)),
      _zRotProp( PropertyManager::instance().addProperty("ZRot",0.0f)),
      _xRotSpeedProp (PropertyManager::instance().addProperty("xRotSpeed",0.0f)),
      _yRotSpeedProp (PropertyManager::instance().addProperty("yRotSpeed",0.0f)),
      _zRotSpeedProp( PropertyManager::instance().addProperty("zRotSpeed",0.0f)),
      _zoomProp (PropertyManager::instance().addProperty("Zoom",0.0f))
  {
        _xRotSpeedProp.Step(0.1);
        _yRotSpeedProp.Step(0.1);
        _zRotSpeedProp.Step(0.1);
  }

 void CameraControl::reset(void)
  {
    
    _xRotProp.resetToDefault();
    _yRotProp.resetToDefault();
    _zRotProp.resetToDefault();
    _zoomProp.resetToDefault();

    _rot_y = _yRotProp.Value();
    _rot_x =_xRotProp.Value();
    _rot_z = _zRotProp.Value();
    _zoom =_zoomProp.Value();
    _old_x= 0;
    _old_y=0;
    _move_z=0;
  }

  CameraControl::~CameraControl(void)
  {
  }

  void CameraControl::init(int width, int height)
  {
      glViewport(0, 0, width, height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

        //setup our orthographic cam
      float size = 0.8f;
      gluOrtho2D(-size,size,-size,size);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity ();
  }

  void CameraControl::apply()
  {
      if (_yRotSpeedProp.Value()>0)
          _yRotProp.Value(_yRotProp.Value()+_yRotSpeedProp.Value());
      if (_xRotSpeedProp.Value()>0)
          _xRotProp.Value(_xRotProp.Value()+_xRotSpeedProp.Value());
      if (_zRotSpeedProp.Value()>0)
          _zRotProp.Value(_zRotProp.Value()+_zRotSpeedProp.Value());
      _rot_x = _xRotProp.Value();
      _rot_y = _yRotProp.Value();
      _rot_z = _zRotProp.Value();
      _zoom = _zoomProp.Value();

      glLoadIdentity();

      //go to texture coordinate mode
      glMatrixMode(GL_TEXTURE);

      glLoadIdentity();

       //pseudo orbitmodel -> move to middle then rotate, then move back
      glTranslatef(0.5, 0.5, 0.5);

      glRotatef(-_rot_x, 1, 0, 0);
      glRotatef(-_rot_y, 0, 1, 0);
      glRotatef(-_rot_z, 0, 0, 1);

      //change size, that rotation in texturespace does not truncate edges

      float scalefactor = 1.7f*(_zoom+90)*0.01f;
    
      glScalef(scalefactor,scalefactor,scalefactor);
      glTranslatef(-0.5,-0.5, -0.5);	

        //back to modelmatrix
      glMatrixMode(GL_MODELVIEW);
      gluLookAt(0.0f,0.0f,1.0f,/*eye*/
          0.0f,0.0f,-1.0f,/*worldpoint*/
          0.0f,1.0f,0.0f);/*up direction*/

  }

  void CameraControl::mouse(int button, int state, int x, int y)
  {
      switch(button) {
      case 0://Left Mouse button
          _old_x = x - _rot_y;
          _old_y = y - _rot_x;
          break;
      case 2://Right Mouse button
          _old_z = x - _old_z;
          _old_y = y - _zoom;
          _move_z = !_move_z;
      }
  }

  void CameraControl::motion(int x, int y)
  {
      if(!_move_z) {
          _rot_y = x - _old_x;
          _rot_x = y - _old_y;
      } else { //if movez is set we use move movement for zooming and rotating around z axis
          _zoom = y - _old_y;
          _rot_z = x - _old_z;
      }
      _xRotProp.Value(_rot_x);
      _yRotProp.Value(_rot_y);
      _zRotProp.Value(_rot_z);
      _zoomProp.Value(_zoom);
  }

}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='