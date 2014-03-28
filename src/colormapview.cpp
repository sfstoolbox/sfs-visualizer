#include "colormapview.h"
#include "renderobjectbase.h"
#include "application.h"
#include "propertymanager.h"
#include "texturefactory.h"

namespace sfs_visualizer
{
    
    ColormapView::ColormapView(void)
    {
        IsVisible(true);
        glActiveTexture(GL_TEXTURE0); //colormap is bound to texture unit 0
        texID = TextureFactory::loadFile(Application::instance().getCommandlineOptions()["colormap"].as<std::string>());
       
        //check height of loaded texture to calculate proper stepwidth for switching
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_tWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_tHeight);

        PropertyManager::instance().getProperty("colormode").Step(1.0f/_tHeight);
        PropertyManager::instance().getProperty("colormode").Value(1.0f/(2*_tHeight));
    }

   


    ColormapView::~ColormapView(void)
    {
    }

    void ColormapView::key(unsigned char key, int x, int y)
    {
        switch (key) {

        case 'h':
            IsVisible(!IsVisible());
            break;

        default:
            break;
        }
    }

    void ColormapView::addQuadslice2(float zOffset)
    {
float normHeight = 1.0f/_tHeight;
float textureOffset = PropertyManager::instance().getPropertyValue("colormode");

        float z= 0.0f;
        glBegin(GL_QUADS);

        glNormal3f( 0.0f, 1.0f, 0.0f); //lightning

        glTexCoord3d(0.0f, 0.0f+textureOffset, 0.0f+z+zOffset);	
        glVertex3f(0.0f, 0.0f, 0.0f+zOffset); //bottom left

        glTexCoord3d(1.0f, 0.0f+textureOffset, 0.0f+z+zOffset);	
        glVertex3f(1.0f, 0.0f, 0.0f+zOffset);//botttom right

        glTexCoord3d(1.0f, textureOffset +normHeight/10, 0.0f+z+zOffset);	
        glVertex3f(1.0f, 1.0f, 0.0f+zOffset); //top right

        glTexCoord3d(0.0f, textureOffset+normHeight/10, 0.0f+z+zOffset);	
        glVertex3f(0.0f, 1.0f, 0.0f+zOffset);//top left

        glEnd();
    }
    void ColormapView::draw_impl()
    {
        if (PropertyManager::instance().getPropertyValue("UseRayTracer"))
            return;

        glMatrixMode(GL_TEXTURE);

        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(-0.5, -0.75, -1.0);
        float scale = 1.0f;
        glScalef(scale, 0.05*scale, scale);

        glColor4f(1.0f,1.0f,1.0f,1.0f);
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, texID);

        addQuadslice2(0);
        glDisable(GL_TEXTURE_2D);
        const float left = -0.55;
        const float right = 0.53;
        const float yOffset = -0.74;
        if (PropertyManager::instance().propertyExists("db")&& PropertyManager::instance().getPropertyValue("db"))
        {
            std::stringstream s;
            s<<PropertyManager::instance().getPropertyValue("MINDB")<<std::flush;
            drawBitmapText(s.str(),left,yOffset,-0.0001);
            drawBitmapText("0",right,yOffset,-0.0001);
        }
        else
        {
            drawBitmapText("-1",left,yOffset,-0.0001);
            drawBitmapText("0",0.0,yOffset,-0.0001);
            drawBitmapText("1",right,yOffset,-0.0001);
        }

    }
 
    }

    // Settings for Vim (http://www.vim.org/), please do not remove:
    // vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
    // vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='