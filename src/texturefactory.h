/**
 * \class TextureFactory
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

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "forwarddecl.h"
#include "stdincl.h"

namespace sfs_visualizer
{
     /**
     * struct to keep statistical data                                   
     **/
    struct Statistics
    {
        float maxR;
        float maxG;
        float maxB;
        float maxA;
        float minR;
        float minG;
        float minB;
        float minA;
        float maxVal;
        float minVal;
        double rms;
    };
    /**
     * helper class to load and construct textures of different type (1D,2D,3D)                                       
     **/
    class TextureFactory
    {
    public:
        ///Defines what Data type is used in graphics memory
        enum TYPE {FLOAT,INT,BYTE,RED,RED2};

        TextureFactory(void);
        ~TextureFactory(void);

        static int build_texture3D(TYPE type,GLsizei width,GLsizei height,GLsizei depth);
        static int build_texture2D(TYPE type,GLsizei width,GLsizei height);
        static int build_texture1D(TYPE type,GLsizei width);
        static int loadFile(const std::string& strFilename);
        static int loadBuffer(const unsigned char *const buffer,int width,int heigh);
        static Statistics statistics(GLint textureId,int width,int height,int depth );

    public:
        ///Returns a list of all texture ids, created with this class
        static std::vector<GLuint>& Alltextures();
    };

}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='