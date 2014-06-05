/**
 * \class MatlabFileAdapter
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

#ifndef MATLABFILEADAPTER_H
#define MATLABFILEADAPTER_H
#include "stdincl.h"
#include "matio.h"

namespace sfs_visualizer
{
    /**
    * this class uses the matio-lib for reading and writing mat-files                               
    **/
    class MatlabFileAdapter
    {
    public:
        MatlabFileAdapter(const std::string& file_name);
        MatlabFileAdapter();
        ~MatlabFileAdapter(void);
        
        void load(const std::string& file_name);
        GLfloat readAt(int x, int y, int z);
        void readRange(int start, int count, GLfloat* result);
        void readRawFloats(GLfloat** result);
        
        static void writeTexture2Matlabfile(GLint textureId,int width,int height,int depth ,const std::string& file);
      
        
        int Xdim() const { return _xdim; }
        void Xdim(int val) { _xdim = val; }
        int Ydim() const { return _ydim; }
        void Ydim(int val) { _ydim = val; }
        int Zdim() const { return _zdim; }
        void Zdim(int val) { _zdim = val; }

    private:
        const std::string& _file_name;
        mat_t* _mat;
        matvar_t* _matvar;
        int _xdim;
        int _ydim;
        int _zdim;
        int _stride;
        char* _rp;
        char* _ip;
        char* _vp;
    };

}

#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='