/**
 * \class RenderEngine
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

#ifndef ENGINE_H
#define ENGINE_H

#include "renderobject.h"
#include "helpoverlay.h"
#include "cameracontrol.h"
#include <boost/thread.hpp>

namespace sfs_visualizer
{
    /**
     * responsible for displaying and managing RenderObjects. It also dispatches userinputs
     **/
    class RenderEngine
    {
    public:
        RenderEngine(void);
        ~RenderEngine(void);

        void start();

        void init(void);
        void initOGL();
        void keyboard(unsigned char key, int x, int y);
        void motion(int x, int y);
        void mouse(int button, int state, int x, int y);
        void idle(void);
        void display(void) ;
        void reshape(int width, int height);
        void end(int code);
        void specialkey(int key, int x, int y);
        void timer (int val);
        void highspeedtimer(int val);

        void takeScreenshot();
        void shutdown();

    public: ///mutators and informators
        int Height() const { return _h; }
        int Width() const { return _w; }
        sfs_visualizer::CameraControl Camera() const { return _camera; }

    private:
        boost::exception_ptr _error;

        /* height, width  of window */
        int _h;
        int _w;		

        float _pointsize;
        std::vector<IRenderObject*> _lObjekte;
        CameraControl _camera;

        unsigned int _fpsCurrent;
        unsigned int _fpsCount;
        unsigned int _highspeedTimerUpdateRateMs;

        bool _bIsDisposed;

    };
    
}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='