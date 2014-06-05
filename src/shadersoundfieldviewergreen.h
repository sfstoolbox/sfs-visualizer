/**
 * \class ShaderSoundfieldViewerGreen
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

#ifndef SHADERSOUNDFIELDVIEWERGREEN_H
#define SHADERSOUNDFIELDVIEWERGREEN_H
#include "renderobject.h"
#include "renderobjectbase.h"
#include "fieldviewerbase.h"
#include "stdincl.h"
#include "shader.h"
#include "infoprovider.h"
#include "propertymanager.h"

namespace sfs_visualizer
{

    /**
     * A FieldViewer that loads a compute shader to compute WFS                                  
     **/
    class ShaderSoundfieldViewerGreen : public FieldViewerBase
    {
    public:
        ShaderSoundfieldViewerGreen();
        ~ShaderSoundfieldViewerGreen(void);

        virtual void update();

        virtual void key(unsigned char key, int x, int y);
        virtual void mouse(int button, int state, int x, int y);
        virtual void specialkey(int key, int x, int y);
        virtual void timer(int updateRateMs);

    protected:

        virtual void draw_impl();
        virtual void initProperties();

    private:

        void loadColorMap();
        void loadSourcesFromCSV();

        float _left;
        float _up;
        float _back;
        Shader *_computeShader;
        float _radiusScale;
        float _bufferpointer;
        float _bufferspeed;
        float _sampleRate;
        float _speedOfSound;
        float _nonlinearSlicePower;
        CommandManager _cmdManager;
    };
}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='