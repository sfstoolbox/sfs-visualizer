/* 
* \file splashscreen.cpp
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

#include "splashscreen.h"
#include "renderobjectbase.h"
#include "application.h"
#include "texturefactory.h"

namespace sfs_visualizer
{

    SplashScreen::SplashScreen(IRenderObject* wavefieldView)
        : _wavefieldView(wavefieldView), _fadeout(100.0)
    {
        IsVisible(true);
        //glActiveTexture(GL_TEXTURE3);
        std::string strSplashscreenImage = Application::instance().getExecutableDir()+"splashscreen.bmp";

        _texID = TextureFactory::loadFile(strSplashscreenImage);
        GLint _tWidth,_tHeight;
//_texID = TextureFactory::loadBuffer(SplashscreenImage,1024,1024);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_tWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_tHeight);
//std::cout << _tWidth << " " << _tHeight << std::endl;
        //_texID = TextureFactory::build_texture2D(TextureFactory::FLOAT,740,538);
       // glTexSubImage2D(GL_TEXTURE_2D,0,0,0,740,538,GL_RGBA,GL_FLOAT,(GLvoid*)SplashscreenImage);

    }


    SplashScreen::~SplashScreen(void)
    {
    }

    void SplashScreen::key(unsigned char key, int x, int y)
    {
        switch (key) {

        case 'h':
            IsVisible(!IsVisible());
            break;

        default:
            break;
        }
    }

    void addQuadslice(float zOffset)
    {
        float z= 0.0f;
        
        glBegin(GL_QUADS);

        glNormal3f( 0.0f, 1.0f, 0.0f); //lightning

        glTexCoord3d(0.0f, 0.0f, 0.0f+z+zOffset);	
        glVertex3f(0.0f, 1.0f, 0.0f+zOffset);//top left

        glTexCoord3d(1.0f, 0.0f, 0.0f+z+zOffset);	
        glVertex3f(1.0f, 1.0f, 0.0f+zOffset); //top right

        glTexCoord3d(1.0f, 1.0f, 0.0f+z+zOffset);	
        glVertex3f(1.0f, 0.0f, 0.0f+zOffset);//bottom right

        glTexCoord3d(0.0f, 1.0f, 0.0f+z+zOffset);	
        glVertex3f(0.0f, 0.0f, 0.0f+zOffset); //bottom left

        glEnd();
    }

    void SplashScreen::timer(int updateRateMs)
    {
        if (!_wavefieldView->IsInitialized())
            return;

        const float thresh = 0.1f;
        if (_fadeout > thresh)
        {
            _fadeout*=0.8f;

            if (_fadeout<thresh)
            {
                IsVisible(false);
                _wavefieldView->IsVisible(true);
            }
        }
    }

    
    void SplashScreen::draw_impl()
    {
        if (_texID==0) //splashscreen loaded
        {
             IsVisible(false);
             return;
        }
        glLoadIdentity();
        glTranslatef(-1.0, -1.0, -1.0);
        //glMatrixMode(GL_MODELVIEW);
        float scale = 2.0f;
        glScalef(scale, scale, scale);
       // glColor4f(1.0f,0.0f,0.0f,1.0f);
        glColor4f(_fadeout,_fadeout,_fadeout,_fadeout);
        //glActiveTexture(GL_TEXTURE3);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _texID);

        addQuadslice(0);
        glDisable(GL_TEXTURE_2D);

    }
    }

    // Settings for Vim (http://www.vim.org/), please do not remove:
    // vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
    // vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='