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