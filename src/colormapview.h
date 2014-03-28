/**
 * \class ColormapView
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

#ifndef COLORMAPVIEW_H
#define COLORMAPVIEW_H
#include "renderobject.h"
#include "renderobjectbase.h"

namespace sfs_visualizer
{
    /**
     * A RenderObject, that displays the colormap on the screen
     **/
    class ColormapView: public RenderObjectBase<ColormapView>
    {
    public:
        ColormapView(void);
        ~ColormapView(void);

        virtual void key(unsigned char key, int x, int y);

    protected:
        virtual void draw_impl();

    private:
        void addQuadslice2(float zOffset);
        int texID;
        GLint _tWidth;
        GLint _tHeight;
    };
}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='