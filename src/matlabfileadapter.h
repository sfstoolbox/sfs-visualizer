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