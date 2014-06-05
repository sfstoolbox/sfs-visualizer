/**
 * GLDispatcher
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

#ifndef GLDISPATCHER_H
#define GLDISPATCHER_H
#include "application.h"

namespace GLDispatcher //glut does not support pointers to c++-memberfunctions->delegate calls to singleton Application
{
  void glutDisplayFunc() { sfs_visualizer::Application::instance().getEngine().display();}
  void glutMotionFunc(int x, int y) {sfs_visualizer::Application::instance().getEngine().motion(x,y);}
  void glutReshapeFunc(int width, int height) {sfs_visualizer::Application::instance().getEngine().reshape(width,height);}
  void glutIdleFunc() {sfs_visualizer::Application::instance().getEngine().idle();}
  void glutMouseFunc(int button, int state, int x, int y) {sfs_visualizer::Application::instance().getEngine().mouse(button,state,x,y);}
  void glutKeyboardFunc(unsigned char key, int x, int y) {sfs_visualizer::Application::instance().getEngine().keyboard(key,x,y);}
  void glutSpecialFunc(int key, int x, int y) {sfs_visualizer::Application::instance().getEngine().specialkey(key,x,y);}
  void glutTimerFunc(int value) {sfs_visualizer::Application::instance().getEngine().timer(value);}
  void glutHighspeedTimerFunc(int value) {sfs_visualizer::Application::instance().getEngine().highspeedtimer(value);}
  void glutCloseFunc() {sfs_visualizer::Application::instance().getEngine().shutdown();}
    
}

#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='