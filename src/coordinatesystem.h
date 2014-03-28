/**
 * \class CoordinateSystem
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

#ifndef COORDINATESYSTEM_H
#define COORDINATESYSTEM_H
#include "renderobject.h"
#include "renderobjectbase.h"

namespace sfs_visualizer
{
    /**
     * A RenderObject, that displays a coordinate system on the screen
     **/
    class CoordinateSystem:public RenderObjectBase<CoordinateSystem>
    {
    public:
        CoordinateSystem();
        ~CoordinateSystem(void);

    protected:
        virtual void draw_impl();


    };

}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='