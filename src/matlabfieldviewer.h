/**
 * \class MatlabFieldViewer
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

#ifndef MATLABFIELDVIEWER_H
#define MATLABFIELDVIEWER_H
#include "renderobject.h"
#include "renderobjectbase.h"
#include "fieldviewerbase.h"
#include "stdincl.h"
#include "shader.h"
#include "infoprovider.h"
#include <boost/thread.hpp>

namespace sfs_visualizer
{
    /**
     * A FieldViewer for Matlab files                                       
     **/
    class MatlabFieldViewer :public FieldViewerBase
    {
    public:
        MatlabFieldViewer();
        ~MatlabFieldViewer(void);

        virtual void update();

        virtual void key(unsigned char key, int x, int y);
        virtual void mouse(int button, int state, int x, int y);
        virtual void specialkey(int key, int x, int y);
        virtual void timer(int updateRateMs);

    protected:

        virtual void draw_impl();
        virtual void initProperties();

    private:

        void readMatlabfile(const char *file_name);
        void updateTexture();
        void initTask(boost::exception_ptr& error);

    private:

        float _left;
        float _up;
        Shader *_shader;
        bool _bInitalLoad;
        bool _bCutMode;
        float _radiusScale;
        int _matlabfilesCount;

    };
}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='
