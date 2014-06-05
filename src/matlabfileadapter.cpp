/* 
* \file matlabfileadapter.cpp
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

#include "matlabfileadapter.h"

namespace sfs_visualizer
{
    MatlabFileAdapter::MatlabFileAdapter()
        :_file_name(""),
        _rp (NULL),
        _ip (NULL),
        _vp (NULL)
    {}

     MatlabFileAdapter::MatlabFileAdapter(const std::string& file_name)
     :_file_name(file_name),
     _rp (NULL),
     _ip (NULL),
     _vp (NULL)
    {
        load(file_name);
    }

    void MatlabFileAdapter::load(const std::string& file_name)
    {
        _mat  = Mat_Open(file_name.c_str(),MAT_ACC_RDONLY );
        if (_mat == NULL)
        {
            throw std::bad_exception("Could not read file");
        }
        else
        {
            if((_matvar = Mat_VarReadNext(_mat)) == NULL ) 
            {
                throw std::bad_exception("Could not read variables in file .mat file");
            }

            if (_matvar->data == NULL)
                throw std::bad_alloc(); //not enough memory

            _xdim = _matvar->dims[0];
            _ydim = _matvar->dims[1];
            _zdim = _matvar->dims[2];

            if (_matvar->isComplex)
            {
                mat_complex_split_t *complex_data = static_cast<mat_complex_split_t*>(_matvar->data);
                _rp = static_cast<char*>(complex_data->Re);
                _ip = static_cast<char*>(complex_data->Im);
            } 
            else
            {
                _vp = static_cast<char*>(_matvar->data);
            }

            _stride = Mat_SizeOf(_matvar->data_type);

        }
    }

    MatlabFileAdapter::~MatlabFileAdapter(void)
    {
        Mat_VarFree(_matvar);
        _matvar = NULL;
        Mat_Close(_mat);
    }

    void MatlabFileAdapter::readRawFloats(GLfloat** result)
    {
            if (_matvar == NULL)
               throw std::bad_exception("Variable konnte nicht gelesen werden");

        if (_vp == NULL)
            *result = reinterpret_cast<GLfloat*>(_rp);
        else
            *result = reinterpret_cast<GLfloat*>(_vp);
    }

    void MatlabFileAdapter::readRange(int start, int count, GLfloat* result)
    {
        double val = 0.0;
       
        float abs = 0;
        float alphascale = 0.05f;
        float boost = 10.0f;
         size_t idx = start;
        int resultpos =0;
        while (idx++ <start+count)
        {
            if (_matvar->data_size==8)//double
            {
                if (_matvar->isComplex)
                {
                    double *re = reinterpret_cast <double*>(_rp+idx*_stride);
                    double *im = reinterpret_cast<double*>(_ip+idx*_stride);

                    val = *re;
                }
                else
                {
                    val = *reinterpret_cast <double*>(_vp+idx*_stride);
                }
            }
            else if (_matvar->data_size==4)//float
            {
                if (_matvar->isComplex)
                {
                    float *re = reinterpret_cast <float*>(_rp+idx*_stride);
                    float *im  = reinterpret_cast<float*>(_ip+idx*_stride);
                   
                    val = *re;
                }
                else
                {
                    val = *reinterpret_cast <float*>(_vp+idx*_stride);
                }
            }
            else
            {
                throw std::bad_exception("Unknown Datatype");
            }

            GLfloat abs = (val*boost);

            result[resultpos++] = abs > 0.0? (GLfloat) abs :0.0f;
            result[resultpos++] = 0.0f;
            result[resultpos++] = abs < 0.0 ?(GLfloat) (-abs):0.0f;
            result[resultpos++] = abs > 0? (GLfloat) (abs*alphascale) :(GLfloat) (-abs*alphascale) ;// alphavalue;
        }

    }

    GLfloat MatlabFileAdapter::readAt(int x, int y, int z)
    {
        double *val = NULL;
        double *re =NULL;
        double *im =NULL;
        double abs = 0;
        size_t idx = _ydim*_zdim*x+_ydim*z+y;

        if (_matvar->isComplex)
        {
            re = reinterpret_cast <double*>(_rp+idx*_stride);
            im = reinterpret_cast<double*>(_ip+idx*_stride);
            val = re;
        }
        else
        {
            val = reinterpret_cast <double*>(_vp+idx*_stride);
        }

        return *val;

    }

   
        void MatlabFileAdapter::writeTexture2Matlabfile(GLint textureId,int width,int height,int depth ,const std::string& file)
        {
        size_t dims[3] = {width,height,depth};
        int    err = 0, i;
        int size = width*height*depth;
        float* matlabValues = new float[size];
        static enum mat_ft            mat_file_ver = MAT_FT_DEFAULT;
        static enum matio_compression compression  = MAT_COMPRESSION_NONE;

        matio_classes matvar_class = MAT_C_SINGLE;
        //mat_complex_split_t z = {NULL,NULL},s = {NULL,NULL};
        mat_t *mat;
        matvar_t *matvar;
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_3D,textureId);

        GLfloat* textureValues = new GLfloat[size * 4]; //we have 4 floats for RGBA

        glGetTexImage (GL_TEXTURE_3D,0,GL_RGBA, GL_FLOAT,textureValues);

        int textureCounter = 0;
        for (int i = 0; i < size; i++,textureCounter+=4 ) {
          float r = textureValues[textureCounter] ; 
          float g =  textureValues[textureCounter+1];
          float b = textureValues[textureCounter+2];
          float a = textureValues[textureCounter+3];
          float value = r>0.0?r:b>0.0f?-b:0.0f;
          matlabValues[i] = value;
        }
    
        //maybe usedfull
        /* if ( !strcmp(optarg,"5") ) {
            mat_file_ver = MAT_FT_MAT5;
        } else if ( !strcmp(optarg,"7.3") ) {
            mat_file_ver = MAT_FT_MAT73;
        } else if ( !strcmp(optarg,"4") ) {
            mat_file_ver = MAT_FT_MAT4;
        } else {
            fprintf(stderr,"Unrecognized MAT file version %s",argv[2]);
            exit(EXIT_FAILURE);
        }*/

        /*z.Re = d;
        z.Im = d+25;
        s.Re = f;
        s.Im = f+25;*/

        mat = Mat_CreateVer(file.c_str(),NULL,mat_file_ver);
        if ( !mat ) {
            throw std::bad_exception("File could not be created");
         }

        switch (matvar_class) {
       /* case MAT_C_DOUBLE:
            matvar = Mat_VarCreate("a",matvar_class,MAT_T_DOUBLE,2,dims,d,0);
            Mat_VarWrite(mat,matvar,compression);
            Mat_VarFree(matvar);
            break;*/
        case MAT_C_SINGLE:
            matvar = Mat_VarCreate("a",matvar_class,MAT_T_SINGLE,3,dims,matlabValues,0);
            Mat_VarWrite(mat,matvar,compression);
            Mat_VarFree(matvar);
            break;
       
        default:
            break;
        }
        delete[] textureValues;
        textureValues= NULL;
        delete[] matlabValues;
        matlabValues= NULL;
        Mat_Close(mat);
    }
}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='