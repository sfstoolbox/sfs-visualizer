/**
 * \class IRenderObject
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

#ifndef IRENDEROBJECT_H
#define IRENDEROBJECT_H

namespace sfs_visualizer
{
     /**
     * Interface of all objects, that can be shown by the renderengine. they handle mouse- and keyevents
     * A timer is called from the renderthread to do regular tasks like recalculations for animations.
     **/
    class IRenderObject
    {
    public:

        virtual ~IRenderObject(void);

    public: ///methods

        virtual void update() =0;
        virtual void draw() =0;
        virtual void timer(int updateRateMs) =0;

        ///user inputs
        virtual void key(unsigned char key, int x, int y) =0;
        virtual void specialkey(int key, int x, int y) =0;
        virtual void mouse(int button, int state, int x, int y)=0;
    
    public: ///mutators and informators
       
        virtual bool IsVisible()=0;
        virtual void IsVisible(bool) = 0;
        virtual bool IsInitialized()=0;

    };

}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='