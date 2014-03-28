#include "helpoverlay.h"
#include "renderobjectbase.h"
#include "application.h"

namespace sfs_visualizer
{

    HelpOverlay::HelpOverlay(void)
    {
        IsVisible(false);
    }


    HelpOverlay::~HelpOverlay(void)
    {
    }

    void HelpOverlay::key(unsigned char key, int x, int y)
    {
        switch (key) {

        case 'h':
            IsVisible(!IsVisible());
            break;

        default:
            break;
        }
    }

    void HelpOverlay::draw_impl()
    {
        glLoadIdentity();
        bool bIsMatlabFile =	(Application::instance().getCommandlineOptions().count("matlabfile")==1);
        std::stringstream ss;

        if (bIsMatlabFile)
        {

            ss	<< "textureblend options:" 
                <<std::endl<< "---------------------------------"
                <<std::endl<< "  mouse: left=rotation, wheel:  move z-cut-plane"
                <<std::endl<< "    wheel + left umsch:     x-cut-plane"
                <<std::endl<< "    wheel + left Ctrl:     y-cut-plane"
              
                <<std::endl<< "  r|R:     change number of textures"

                <<std::endl<<std::endl<< "raytrace options"
                <<std::endl<< "---------------------------------"
                <<std::endl<< "  mouse: left=rotation, right:  zoom"
                <<std::endl<< "  y|Y: threshold"
                <<std::endl<< "  m|M: brightness"
                <<std::endl<< "  x|X: quality (very expensive)"
                <<std::endl;
                      
        }
        else
        {
            ss	<< "textureblend options:" 
                <<std::endl<< "---------------------------------"
                <<std::endl<< "  mouse: left=rotation,wheel=frequency"

                <<std::endl<< "  m|M:     brightness"
                <<std::endl<< "  b|B:     amplification"
                <<std::endl<< "  r|R:     change number of textures"


                <<std::endl<<std::endl<< "raytrace options"
                <<std::endl<< "---------------------------------"
                <<std::endl<< "  mouse: left=rotation, right:  zoom"
                <<std::endl<< "  y|Y: threshold"
                <<std::endl<< "  m|M: brightness"
                <<std::endl<< "  x|X: quality (very expensive)"
                <<std::endl

                <<std::endl<< "calculation options"
                <<std::endl<< "---------------------------------"
                <<std::endl<< "  +|*:   add/remove source"
                <<std::endl<< "  Arrowkeys:     move virtual source"
                <<std::endl<< "  p|P:    size scaling"
                <<std::endl<< "  mousewheel + left umsch:     Time"
                <<std::endl<<   "mousewheel + left Ctrl:     Opacity"
                <<std::endl<<   "mousewheel + left Ctrl + umsch:     Opacity of cutted area"
                <<std::endl;

        }

        ss <<std::endl<< "general options"
            <<std::endl<< "---------------------------------"
            <<std::endl<< "  2|\":  change colormapping"
            <<std::endl<< "  4:     toggle raytrace (may crash sometime...)"
            <<std::endl<< "  5:     screenshot"
            <<std::endl<< "  6:     reset camera"
            <<std::endl<< "  7:     save current state to properties.csv"
            <<std::endl<< "  j:     export current field to data.mat"
            <<std::endl<< "  b|B:   amplification"
            <<std::endl<< "  c:     cut-planes/full"
            <<std::endl<< "  i|I:   show info";


        ss <<std::endl<< "  ESC:   Exit"
           <<std::endl<< "  .|: : change calculation-updaterate" 
           <<std::endl<< "  -|_ : change calculation-timestepsize" 
           <<std::endl<< "  u|U: reverse Animation" 
           <<std::endl<< "  SPACE: Stop/Start Animation"; 
glDisable(GL_BLEND);
        drawBitmapText(ss.str(),-0.6,0.7,-0.0001);
glEnable(GL_BLEND);
    }
    }

    // Settings for Vim (http://www.vim.org/), please do not remove:
    // vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
    // vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='