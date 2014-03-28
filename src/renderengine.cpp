#include "renderengine.h"
#include "stdincl.h"
#include "matlabfieldviewer.h"
#include "shadersoundfieldviewergreen.h"
#include "helpoverlay.h" 
#include "infooverlay.h" 
#include "colormapview.h"
#include "splashscreen.h"
#include "coordinatesystem.h"
#include "renderobjectbase.h"
#include "gldispatcher.h"
#include "SOIL.h"

namespace sfs_visualizer
{
    //dummy exceptionobject
    std::exception nullException;
    RenderEngine::RenderEngine(void)
        :   _camera(), _lObjekte(), _pointsize(2.0f),_fpsCurrent(0),
            _fpsCount(0),_highspeedTimerUpdateRateMs(50),_error(),_bIsDisposed(false)
    {

    }

    RenderEngine::~RenderEngine(void)
    {
        //cleanup
        for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
        {
            delete (*it);
        }
        _lObjekte.clear();
    }


    void RenderEngine::init(void) {

        if (Application::instance().getCommandlineOptions().count("interval"))
            _highspeedTimerUpdateRateMs = Application::instance().getCommandlineOptions()["interval"].as<int>();
         
        //insert all our RenderObjects

        IRenderObject* wavefieldView = NULL;
        if (Application::instance().getCommandlineOptions().count("matlabfile"))
            wavefieldView = new MatlabFieldViewer();
        else
            wavefieldView = new ShaderSoundfieldViewerGreen();

        _lObjekte.push_back(new CoordinateSystem());

        _lObjekte.push_back(wavefieldView);
        
        _lObjekte.push_back(new ColormapView());

        _lObjekte.push_back(new SplashScreen(wavefieldView));
        
        _lObjekte.push_back(new HelpOverlay());
        
        _lObjekte.push_back(new InfoOverlay(*dynamic_cast<InfoProvider*>(wavefieldView)));

    }

    void checkGLRequirements()
    {

        printf("OpenGL 4.2 or higher with compute-shader support required. \nYou have OpenGL (%s): \n", glGetString(GL_VERSION));
        /*GLint val;

        glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS,&val);
        std::cout << "MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS,&val);
        std::cout << "MAX_COMPUTE_UNIFORM_BLOCKS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS,&val);
        std::cout << "MAX_COMPUTE_TEXTURE_IMAGE_UNITS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_IMAGE_UNIFORMS,&val);
        std::cout << "MAX_COMPUTE_IMAGE_UNIFORMS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE,&val);
        std::cout << "MAX_COMPUTE_SHARED_MEMORY_SIZE" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS,&val);
        std::cout << "MAX_COMPUTE_UNIFORM_COMPONENTS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS,&val);
        std::cout << "MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS" << val << std::endl;
        glGetIntegerv( GL_MAX_COMPUTE_ATOMIC_COUNTERS,&val);
        std::cout << "MAX_COMPUTE_ATOMIC_COUNTERS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS,&val);
        std::cout << "MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS,&val);
        std::cout << "MAX_COMPUTE_WORK_GROUP_INVOCATIONS" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT,&val);
        std::cout << "MAX_COMPUTE_WORK_GROUP_COUNT" << val << std::endl;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE,&val);
        std::cout << "MAX_COMPUTE_WORK_GROUP_SIZE" << val << std::endl;
        glGetIntegerv(GL_COMPUTE_WORK_GROUP_SIZE,&val);
        std::cout << "COMPUTE_WORK_GROUP_SIZE" << val << std::endl;*/

    }

    void RenderEngine::initOGL()
    {
        glutInitDisplayMode ( GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE);

        int argc = 1;
        char *argv[1] = {(char*)"DummyToInitGlut"};
        glutInit(&argc,argv);

        glutGameModeString("640x480:32@60");
        //glutGameModeString("800x600:32@60");
        // glutGameModeString("1440x900:32@60");


        if(glutGameModeGet(GLUT_GAME_MODE_POSSIBLE) && Application::instance().getCommandlineOptions().count("fullscreen"))
        {
            _w = glutGameModeGet(GLUT_GAME_MODE_WIDTH);
            _h = glutGameModeGet(GLUT_GAME_MODE_HEIGHT);
            printf("w:%i h:%i\n",_w,_h);
            _camera.init(_w,_h);
            glutEnterGameMode();
        }
        else
        {
            glutInitWindowPosition(50, 50);
            glutInitWindowSize(512, 512);
            glutCreateWindow(APP_TITLE);
            _w = glutGet(GLUT_WINDOW_WIDTH);
            _h = glutGet(GLUT_WINDOW_HEIGHT);

        }
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);//for gracefull exception handling

        glutDisplayFunc(GLDispatcher::glutDisplayFunc);
        glutReshapeFunc(GLDispatcher::glutReshapeFunc);
        glutKeyboardFunc(GLDispatcher::glutKeyboardFunc);
        glutMouseFunc(GLDispatcher::glutMouseFunc);
        glutMotionFunc(GLDispatcher::glutMotionFunc);
        glutIdleFunc(GLDispatcher::glutIdleFunc);
        glutSpecialFunc(GLDispatcher::glutSpecialFunc);
        glutCloseFunc(GLDispatcher::glutCloseFunc); 

        glewInit();

        checkGLRequirements();
      
        //glEnable(GL_DITHER);
        glEnable( GL_MULTISAMPLE );
        glEnable(GL_NORMALIZE);
        glShadeModel(GL_SMOOTH); 
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }

    void RenderEngine::timer(int val) {
        try
        {
            if (_bIsDisposed)
                return;
            char title[120];
            _fpsCount = _fpsCurrent;
            _fpsCurrent = 0;

            sprintf(title, "%s. FPS: %d Cam: (%d° %d° %d° %d)", APP_TITLE, _fpsCount,_camera.Spin_x(),_camera.Spin_y(),_camera.Spin_z(),_camera.Zoom());
            glutSetWindowTitle(title);
            glutTimerFunc(1000, GLDispatcher::glutTimerFunc, 1);
        }
        catch(...)
        {
            _error = boost::current_exception();
            glutLeaveMainLoop ();
        }


    }


    void RenderEngine::highspeedtimer(int val) 
    {
        try
        {
            if (_bIsDisposed)
                return;

            for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
            {
                if ((*it)->IsVisible())
                    (*it)->timer(val);
            }

            glutTimerFunc(_highspeedTimerUpdateRateMs, GLDispatcher::glutHighspeedTimerFunc, _highspeedTimerUpdateRateMs);
        }
        catch(...)
        {
            _error = boost::current_exception();
            glutLeaveMainLoop ();
        }
    }

    void RenderEngine::shutdown(void)
    {
         _bIsDisposed = true;
        glutLeaveMainLoop ();
    }

    void RenderEngine::start(void)
    {
        initOGL();
        init();
        timer(0);
        highspeedtimer(0);
        glutMainLoop();
        _bIsDisposed = true;
        if( _error )
            boost::rethrow_exception(_error);
       
    }


    void RenderEngine::reshape(int width, int height) {
        _w = width;
        _h = height;
        _camera.init(_w,_h);
    }
    bool stop = false;
    int screenshot = 0;
    void RenderEngine::takeScreenshot() {
        int x=0;
        int y=0; 
        int w=_w;
        int h=_h;
        stop =true;

        std::vector<  unsigned char> rgbflipped(3*w*h);
        std::vector< unsigned char > rgb(3*w*h);
        glReadPixels(x, 0, w, y + h, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);

        //flip image
        for (int y=0;y<h-1;y++)//all lines
        {
            for (int x=0;x<3*w;x++)
            {
                rgbflipped[x+y*3*w]=rgb[x+(h-1-y)*3*w];
            }
        }

        //generate a unique filename
        time_t rawtime;
        struct tm * timeinfo;
        char buffer [255];

        time (&rawtime);
        timeinfo = localtime (&rawtime);

        strftime (buffer,255,"%Y-%m-%d_%H-%M-%S",timeinfo);
        puts (buffer);


        std::stringstream fName;
        fName << "screenshot_" << buffer << ".bmp";
        int save_result = SOIL_save_image
            (
            fName.str().c_str(),
            SOIL_SAVE_TYPE_BMP,
            w, h, 3,
            rgbflipped.data()
            );
        stop =false;
    }
    void RenderEngine::display(void) {

        if (stop)
            return;
        try
        {
            glClearColor(0.0f,0.0f,0.0f,1.0f);
            //glClearColor(1.0f,1.0f,1.0f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();

            _camera.apply();

            for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
            {
                (*it)->draw();
            }

            glPopMatrix();

            _fpsCurrent++;

            glutSwapBuffers();
        }
        catch(...)
        {
            _error = boost::current_exception();
            glutLeaveMainLoop ();
        }
    }


    void RenderEngine::idle(void)
    {
        for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
        {
            (*it)->update();
        }

        glutPostRedisplay();
    }


    void RenderEngine::specialkey(int key, int x, int y)
    {
        for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
        {
            (*it)->specialkey(key,x,y);
        }
    }


    void RenderEngine::end(int code)
    {
        _bIsDisposed =true;
        glutLeaveMainLoop ();
    }

    void RenderEngine::mouse(int button, int state, int x, int y)
    {
        for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
        {
            (*it)->mouse(button,state,x,y);
        }

        _camera.mouse(button,state,x,y);

        glutPostRedisplay();
    }

 void RenderEngine::motion(int x, int y) {

     _camera.motion(x,y);

     glutPostRedisplay();
 }

 void RenderEngine::keyboard(unsigned char key, int x, int y)
 {
     static int old_x = 50;
     static int old_y = 50;
     static int old_width = 512;
     static int old_height = 512;

     //dispatch keyevent to all renderobjects
     for (std::vector<IRenderObject*>::iterator it = _lObjekte.begin();it!=_lObjekte.end();++it)
     {
         (*it)->key(key,x,y);
     }

     switch (key) {
     case 27:
         end(0);
         break;
     case '.':
         _highspeedTimerUpdateRateMs +=1;
         printf("highspeedTimerUpdateRateMs %i ms\r",_highspeedTimerUpdateRateMs);
         break;
     case ':':
         _highspeedTimerUpdateRateMs -=1;
         if (_highspeedTimerUpdateRateMs<1)
             _highspeedTimerUpdateRateMs = 1;
         printf("highspeedTimerUpdateRateMs %i ms\r",_highspeedTimerUpdateRateMs);
         break;
     case '5':
         takeScreenshot();
         break;
     case '6':
         _camera.reset();
         break;
     case '7':
         PropertyManager::instance().save();
         break;
     default:
         break;

     }
 }




}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='