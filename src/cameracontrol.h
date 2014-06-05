/**
 * \class CameraControl
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

#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H
#include "property.h"
namespace sfs_visualizer
{

    /**
    * An orthographic camera, with x-y-z-rotation and zoom in texturespace 
    **/
    class CameraControl
    {
    public:
        CameraControl(void);
        ~CameraControl(void);

        void init(int width, int height);
        void apply();
        void mouse(int button, int state, int x, int y);
        void motion(int x, int y);
        void reset();

    public:
    
        int Spin_x() const { return _rot_x; }
        void Spin_x(int val) { _rot_x = val; }
        int Spin_y() const { return _rot_y; }
        void Spin_y(int val) { _rot_y = val; }
        int Spin_z() const { return _rot_z; }
        void Spin_z(int val) { _rot_z = val; }
        int Zoom() const { return _zoom; }
        void Zoom(int val) { _zoom = val; }

    private:
        Property& _xRotProp;
        Property& _yRotProp;
        Property& _zRotProp;
        Property& _xRotSpeedProp;//automatic rotation
        Property& _yRotSpeedProp;
        Property& _zRotSpeedProp;
        Property& _zoomProp;

        int _rot_y;//in degrees
        int _rot_x;//in degrees
        int _rot_z;//in degrees

        int _zoom; 
        int _old_x;
        int _old_y;
       
        int _old_z;
        bool _move_z;
    };

}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='