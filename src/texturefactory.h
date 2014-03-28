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