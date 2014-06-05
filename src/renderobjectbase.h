/**
 * \class RenderObjectBase
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

#ifndef RENDEROBJECTBASE_H
#define RENDEROBJECTBASE_H
#include "renderobject.h"
#include "stdincl.h"

namespace sfs_visualizer
{
    /**
    * Base for RenderObjects, which are objects, that can be shown in 3D.
    * Is used to share implementations for all RenderObjects.
    **/
    template <class TYPE>    
    class RenderObjectBase : public IRenderObject 
    {
    public:

        RenderObjectBase(void);
        ~RenderObjectBase(void);

        virtual bool IsVisible();
        virtual void IsVisible(bool bVisible);
        virtual bool IsInitialized();

        virtual void update();
        virtual void draw();
        virtual void key(unsigned char key, int x, int y);
        virtual void specialkey(int key, int x, int y);
        virtual void mouse(int button, int state, int x, int y);
        virtual void timer(int updateRateMs);
        virtual void rotateTranslate(float xRot, float yRot, float zRot, float xTrans, float yTrans, float zTrans);

        static bool checkGLErrors(const std::string& info);

    protected:

        virtual void draw_impl() =0;
        void drawBitmapText(const std::string& strValue,float x,float y,float z);
        ///Random float -0.5-0.5
        float random();
        void toggleLightning();
        void toggleAlphaBlending();
        float _zeilenabstand;
        bool _bIsInitialized;

    private:
        bool _bIsVisble;
        bool _bLightOn;
        bool _bAlphaBlendOn;
    };



    template <class TYPE>
    sfs_visualizer::RenderObjectBase<TYPE>::RenderObjectBase()
        :_bIsVisble(true),_bIsInitialized(false),_bLightOn(false), _bAlphaBlendOn(false),_zeilenabstand(0.05f)
    {}

    template <class TYPE>
    sfs_visualizer::RenderObjectBase<TYPE>::~RenderObjectBase()
    {}

    template <class TYPE>
    bool sfs_visualizer::RenderObjectBase<TYPE>::IsVisible()
    {
        return _bIsVisble;
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::IsVisible(bool bVisible)
    {
        _bIsVisble = bVisible;
    }

    template <class TYPE>
    bool sfs_visualizer::RenderObjectBase<TYPE>::IsInitialized()
    {
        return _bIsInitialized;
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::key(unsigned char key, int x, int y)
    {
        switch ( key ) {
        case 'l':
            toggleLightning();
            break;
        }

    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::specialkey(int key, int x, int y)
    {}


    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::update()
    {}


    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::timer(int updateRateMs)
    {}

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::draw()
    {
        if (_bIsVisble)
        {
            draw_impl();
        }
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::toggleAlphaBlending()
    {
        if (_bAlphaBlendOn)
        {
            glDisable(GL_BLEND);
            _bAlphaBlendOn= false;
        }
        else
        {
            glEnable(GL_BLEND);
            _bAlphaBlendOn= true;
        }
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::toggleLightning()
    {
        if (_bLightOn)
        {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            _bLightOn= 0;
        }
        else
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            _bLightOn= 1;
        }
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::rotateTranslate(float xRot, float yRot, float zRot, float xTrans, float yTrans, float zTrans)
    {
        glTranslatef(xTrans, yTrans, zTrans);

        glRotatef(xRot, 1, 0, 0);
        glRotatef(yRot, 0, 1, 0);
        glRotatef(zRot, 0, 0, 1);
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::mouse(int button, int state, int x, int y)
    {
    }

    template <class TYPE>
    void sfs_visualizer::RenderObjectBase<TYPE>::drawBitmapText(const std::string& strValue, float x, float y, float z)
    {  
        glPushMatrix();
        glLoadIdentity();
        glColor4f(1.0f, 1.0f, 1.0f,1.0f);
        glRasterPos3f(x, y,z);

        for (std::string::const_iterator it=strValue.cbegin();it!=strValue.cend();++it)
        {
            if ((*it) == '\n')
            {
                y-=_zeilenabstand;
                glRasterPos3f(x, y,z);
            }
            else
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *it);
        }

        glPopMatrix();
    }

    template <class TYPE>
    float sfs_visualizer::RenderObjectBase<TYPE>::random()
    {
        float randi = (float)rand()/RAND_MAX;
        return randi-0.5f;
    }


    template <class TYPE>
    bool sfs_visualizer::RenderObjectBase<TYPE>::checkGLErrors(const std::string& info)
    {
        GLenum e = glGetError();
        if (e != GL_NO_ERROR) {
            if (Application::instance().getCommandlineOptions().count("verbose"))
                fprintf(stderr, "OpenGL error in \"%s\": %s (%d)\n", info.c_str(), gluErrorString(e), e);
            return false;
        }
        return true;
    }

}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='