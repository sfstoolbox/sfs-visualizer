/* 
* \file texturefactory.cpp
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

#include "texturefactory.h"
#include "SOIL.h"
#include "application.h"

namespace sfs_visualizer
{

TextureFactory::TextureFactory(void)
{

}


TextureFactory::~TextureFactory(void)
{
}

std::vector<GLuint>& TextureFactory::Alltextures()
{
   static std::vector<GLuint> texture;
   return texture;
}

int TextureFactory::loadFile(const std::string& strFilename)
{
   int texID = (int) SOIL_load_OGL_texture
        (
        strFilename.c_str(),
        SOIL_LOAD_RGBA,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS
        );
    Alltextures().push_back(texID);
    return texID;
}

int TextureFactory::loadBuffer(const unsigned char *const buffer,int width,int height)
{
    int texID = (int) SOIL_create_OGL_texture(
        buffer,
        width,
        height,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS
        );
    Alltextures().push_back(texID);
    return texID;
}

int TextureFactory::build_texture3D(TYPE type,GLsizei width,GLsizei heigth,GLsizei depth)
  {
      GLuint texid;
      glGenTextures(1 , &texid);
    // tell OpenGL we're going to be setting up the texture name it gave us	
    glBindTexture(GL_TEXTURE_3D, texid);	
  
    // we want the texture to repeat over the S axis, so if we specify coordinates out of range we still get textured.
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // same as above for T axis
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // same as above for R axis
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // this is a 3d texture, level 0 (max detail), GL should store it in RGB8 format, its WIDTHxHEIGHTxDEPTH in size, 
    // it doesnt have a border, we're giving it to GL in RGB format as a series of unsigned bytes, and texels is where the texel data is.
    switch (type)
    {

    case RED2:

        glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, width, heigth, depth, 0, GL_LUMINANCE, GL_FLOAT, NULL);

         //for use in shader as image3D this texture must be bound as read_write image
        glBindImageTexture(0, texid, 0, /*layered=*/GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F); 
        break;

    case RED:

         glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, heigth, depth, 0, GL_RED, GL_FLOAT, NULL);

              //for use in shader as image3D this texture must be bound as read_write image
            glBindImageTexture(0, texid, 0, /*layered=*/GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F); 
        break;
    case FLOAT:
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, heigth, depth, 0, GL_RGBA, GL_FLOAT, NULL);

          //for use in shader as image3D this texture must be bound as read_write image
         glBindImageTexture(0, texid, 0, /*layered=*/GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F); 
      break;
    case INT:
       glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16, width, heigth, depth, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
	   glBindImageTexture(0, texid, 0, /*layered=*/GL_TRUE, 0, GL_READ_WRITE, GL_RGBA16); 
       break;
    case BYTE:
      glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, heigth, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	  glBindImageTexture(0, texid, 0, /*layered=*/GL_TRUE, 0, GL_READ_WRITE, GL_RGBA); 
      break;
    }

  
    Alltextures().push_back(texid);

 
    return texid;
  }

int TextureFactory::build_texture2D(TYPE type,GLsizei width,GLsizei heigth)
  {
      GLuint texid;
      glGenTextures(1 , &texid);
    // tell OpenGL we're going to be setting up the texture name it gave us	
    glBindTexture(GL_TEXTURE_2D, texid);	
     // we want the texture to repeat over the S axis, so if we specify coordinates out of range we still get textured.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // same as above for T axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // same as above for R axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    // this is a 3d texture, level 0 (max detail), GL should store it in RGB8 format, its WIDTHxHEIGHTxDEPTH in size, 
    // it doesnt have a border, we're giving it to GL in RGB format as a series of unsigned bytes, and texels is where the texel data is.
    switch (type)
    {
      case FLOAT:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, heigth, 0, GL_RGBA, GL_FLOAT, NULL);

          break;
       }

       Alltextures().push_back(texid);

    return texid;
  }



int TextureFactory::build_texture1D(TYPE type,GLsizei width)
  {
      GLuint texid;
      glGenTextures(1 , &texid);
    // tell OpenGL we're going to be setting up the texture name it gave us	
    glBindTexture(GL_TEXTURE_1D, texid);	
 
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
    switch (type)
    {

    case FLOAT:
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, width, 0, GL_RGBA, GL_FLOAT, NULL);
      glBindImageTexture(0, texid, 0, /*layered=*/GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F); //wichtig um die texture schreibbar zu machen
      break;
    case INT:
       glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16, width, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);
       break;
    case BYTE:
      glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      break;
    }

    Alltextures().push_back(texid);

    return texid;

  }

Statistics TextureFactory::statistics(GLint textureId,int width,int height,int depth )
{

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_3D,textureId);
    int size = width*height*depth;
    GLfloat* textureValues = new GLfloat[size * 4]; //we have 4 floats for RGBA

    glGetTexImage (GL_TEXTURE_3D,0,GL_RGBA, GL_FLOAT,textureValues);

    Statistics s;
    s.maxR =0.0f;
    s.maxG =0.0f;
    s.maxB =0.0f;
    s.maxA =0.0f;
    s.minR =1000000.0f;
    s.minG =1000000.0f;
    s.minB =1000000.0f;
    s.minA =1000000.0f;
    s.maxVal = 0.0f;
    s.minVal = 100000.0f;
    s.rms =0;
    int textureCounter = 0;
    for (int i = 0; i < size; i++,textureCounter+=4 ) {
        float r = textureValues[textureCounter] ; 
        float g =  textureValues[textureCounter+1];
        float b = textureValues[textureCounter+2];
        float a = textureValues[textureCounter+3];
        float value = r>0.0?r:b>0.0f?-b:0.0f;
        s.rms += value*value;
        if(value<s.minVal)
            s.minVal =value;
        if(value>s.maxVal)
            s.maxVal =value;
        if(r>s.maxR)
            s.maxR =r;
        if(g>s.maxG)
            s.maxG =g;
        if(b>s.maxB)
            s.maxB =b;
        if(a>s.maxA)
            s.maxA =a;

        if(r<s.minR)
            s.minR =r;
        if(g<s.minG)
            s.minG =g;
        if(b<s.minB)
            s.minB =b;
        if(a<s.minA)
            s.minA =a;
    }
    s.rms = sqrt(s.rms/size);
if (Application::instance().getCommandlineOptions().count("verbose"))
{
    std::cout << "                                                                          \r" <<std::flush;
    std::cout << "Max:" << s.maxVal << " Min: " << s.minVal << " rms:" <<s.rms<< std::endl;
    std::cout << "Max: R=" << s.maxR << " G=" <<s.maxG << " B=" << s.maxB << " A=" << s.maxA <<std::endl<< "Min: R=" << s.minR << " G=" <<s.minG << " B=" << s.minB << " A=" << s.minA << std::endl;
}
return s;
}
}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='