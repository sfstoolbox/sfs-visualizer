/**
 * \class SplashScreen
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

#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H
#include "renderobject.h"
#include "renderobjectbase.h"

namespace sfs_visualizer
{
    /**
     * A RenderObject, that displays the splashcreen at startup
     **/
    class SplashScreen:public RenderObjectBase<SplashScreen>
    {
    public:
        SplashScreen(IRenderObject* wavefieldView);
        ~SplashScreen(void);

        virtual void key(unsigned char key, int x, int y);
        virtual void timer(int updateRateMs);

    protected:
        virtual void draw_impl();

    private:
        int _texID;
        float _fadeout;
        IRenderObject* _wavefieldView;
    };
}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='