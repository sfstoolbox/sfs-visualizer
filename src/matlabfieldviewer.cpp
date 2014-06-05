/* 
* \file matlabfieldviewer.cpp
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

#include "matlabfieldviewer.h"
#include "application.h"
#include "cameracontrol.h"
#include "matio_private.h"
#include "matlabfileadapter.h"
#include "shader.h"
#include "colormapview.h"
#include "texturefactory.h"
#include <time.h>
#include <boost/thread.hpp>
#include "SOIL.h"

namespace sfs_visualizer
{
    boost::thread thread_1;
   
    boost::exception_ptr error;
    ///the matlabfile is loaded from a background thread
    void MatlabFieldViewer::initTask(boost::exception_ptr& error)
    {
        try
        {
            std::vector<std::string> filename = Application::instance().getCommandlineOptions()["matlabfile"].as<std::vector<std::string> >();
            _matlabfilesCount =   filename.size();   
   
            for (std::vector<std::string>::iterator it = filename.begin();it!=filename.end();++it )
                readMatlabfile(it->c_str());
            error = boost::exception_ptr();
        }
        catch(...) //global Exceptionhandler
        {
            error = boost::current_exception();
        }
    }

    MatlabFieldViewer::MatlabFieldViewer(void)
        :
        _left(-2.0f),
        _up(2.0f),
        _radiusScale(1e1),
        _bCutMode(false),
        _bInitalLoad(true)
    {
        glActiveTexture(GL_TEXTURE0);
        if (_bInitalLoad)
            updateTexture();

        _shader = new Shader("computeMatlab.vert",ShaderType::Compute);
        checkGLErrors("konstruktor1");
        _shader->createProgram();
        checkGLErrors("konstruktor2");
        _shader->attachShader(_shader->_shaderId);
        checkGLErrors("konstruktor3");
        _shader->linkProgram();
        checkGLErrors("konstruktor4");
        
        _alphablendmode= 0;
        _alphatest=0.015f;
        toggleLightning();
        initProperties();
        checkGLErrors("konstruktor4");
    }

    void MatlabFieldViewer::initProperties()
    {
        Property& xCut = PropertyManager::instance().addShaderProperty("xCut",0.0f,_shader);
        _cmdManager.addMouseCommand("increment xCut",11,"wheel up and left shift pressed",xCut,PropertyCommand::Add);
        _cmdManager.addMouseCommand("decrement xCut",12,"wheel down and left shift pressed",xCut,PropertyCommand::Sub);
        xCut.Step(1.0);
        Property& yCut = PropertyManager::instance().addShaderProperty("yCut",0.0f,_shader);
        _cmdManager.addMouseCommand("increment yCut",20,"wheel up and left control pressed",yCut,PropertyCommand::Add);
        _cmdManager.addMouseCommand("decrement yCut",19,"wheel down and left control pressed",yCut,PropertyCommand::Sub);
        yCut.Step(1.0);

        Property& zCut = PropertyManager::instance().addShaderProperty("zCut",0.0f,_shader);
        _cmdManager.addMouseCommand("increment zCut",4,"wheel up",zCut,PropertyCommand::Add);
        _cmdManager.addMouseCommand("decrement zCut",3,"wheel down",zCut,PropertyCommand::Sub);
        zCut.Step(1.0);

        Property& prop = PropertyManager::instance().addShaderProperty("boost",1.0f,_shader);
        _cmdManager.addCommand("increment boost",'b',"b",prop,PropertyCommand::Mul);
        _cmdManager.addCommand("decrement boost",'B',"B",prop,PropertyCommand::Div);
        //prop.Step(1.01);

        Property& multiplier = PropertyManager::instance().addShaderProperty("multiplier",1.0,_shader);
        _cmdManager.addCommand("increment multiplier for postamplification",'m',"m",multiplier,PropertyCommand::Mul);
        _cmdManager.addCommand("decrement multiplier for postamplification",'M',"M",multiplier,PropertyCommand::Div);

        Property& colormode = PropertyManager::instance().addShaderProperty("colormode",0.0f,_shader);
        _cmdManager.addCommand("increment colormode",'2',"2",colormode,PropertyCommand::Add);
        _cmdManager.addCommand("decrement colormode",'"',"\"",colormode,PropertyCommand::Sub);
        colormode.IsVisible(false);
        colormode.Min(0.0f);
        colormode.Max(1.0f);
        Property& minDB = PropertyManager::instance().addShaderProperty("MINDB",-45.0f,_shader);
        _cmdManager.addCommand("increment minimum dB value",'d',"d",minDB,PropertyCommand::Add);
        _cmdManager.addCommand("decrement maximum dB value",'D',"D",minDB,PropertyCommand::Sub);
        minDB.IsVisible(false);
        minDB.Min(-200);
        minDB.Max(0);
        Property& useDB = PropertyManager::instance().addShaderProperty("db",0.0f,_shader);
        _cmdManager.addCommand("use dB",'3',"3",useDB,PropertyCommand::Toggle);

        Property& doCut = PropertyManager::instance().addShaderProperty("doCut",0.0f,_shader);
        _cmdManager.addCommand("cut planes",'c',"c",doCut,PropertyCommand::Toggle);

        //must call base for correct initialization
        FieldViewerBase::initProperties();
    }

    MatlabFieldViewer::~MatlabFieldViewer(void)
    {
    }
   

    int currentZ = -1;
    //Declaration on global heap has more space for big datastructures 300x300x300 -> stack overflow
    GLfloat* image2;

    float _fieldmax = 0;
    MatlabFileAdapter  _adapter;
    void MatlabFieldViewer::readMatlabfile(const char *file_name)
    {
        clock_t init, middle, final;
        init = clock();

        printf("Reading '%s'...",file_name); 

        _adapter.load(file_name);
        middle = clock() - init;
        printf("ok %fs\nUpdating texture...", (double)middle / ((double)CLOCKS_PER_SEC));
        _adapter.readRawFloats(&image2);

        WIDTH = _adapter.Xdim();
        HEIGHT = _adapter.Ydim();
        DEPTH = _adapter.Zdim();

        currentZ = z;

        //active waiting to sync with renderthread
        while (currentZ != -1)
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));

        final = clock() - middle;
        printf("ok %fs\n", (double)final / ((double)CLOCKS_PER_SEC));

        _valueChanged = true;

    }

    static int fileswitch = 0;
   
    void MatlabFieldViewer::updateTexture()
    {
        clock_t init, final;
        init = clock();
           
        thread_1 = boost::thread( boost::bind(&MatlabFieldViewer::initTask,this,boost::ref(error)) );

        checkGLErrors("updateTexure");

      
    }
   
    static const float col[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    void MatlabFieldViewer::key(unsigned char key, int x, int y)
    {
        FieldViewerBase::key(key,x,y);
        float dAlpha = 0.0f;
        float dwaveLength = 0.0f;
        float dBoost = 0.0f;
        GLfloat mat_emission[] = { 0.5, 0.5, 0.5, 0.5 };

        switch (key) {


        case 'h':
            IsVisible(! RenderObjectBase::IsVisible());
            break;

        case 's':
            _sliceDist *= 1.5f;
            _info << "abstand=" << _sliceDist;
            printf("abstand %f\r              ", _sliceDist);
            break;
        case 'S':
            _sliceDist *= 0.9f;
            _info << "abstand=" << _sliceDist;
            printf("abstand %f\r              ", _sliceDist);
            break;
           
        case 'p':
            _radiusScale *= 1.1;

            _shader->setUniform("radiusScale", _radiusScale);
            _info << "radiusScale=" << _radiusScale;
            printf("radiusScale %f\r", _radiusScale);
            break;
        case 'P':
            _radiusScale *= 0.9;

            _shader->setUniform("radiusScale", _radiusScale);
            _info << "radiusScale=" << _radiusScale;
            printf("radiusScale %f\r", _radiusScale);
            break;

        case '1':
           
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
            glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, col);

            break;
        case '!':
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            break;

        case 'c':
            _bCutMode =!_bCutMode;
            _valueChanged = true;
            break;
      
        case '0':
            glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
            break;
        default:
            //printf("Key %d corresponding to char '%c' is not assigned to any action.\r", key, key);
            break;
        }
    }



    void MatlabFieldViewer::specialkey(int key, int x, int y)
    {
        switch (key) {
        case GLUT_KEY_LEFT:
            if (_matlabfilesCount>1)
            {
                if (_currenttexture == 0)
                    _currenttexture =_textures.size()-1;

                _currenttexture--;

                _currenttexture %=(_textures.size()-1);
                _valueChanged = true;
            }
            //_left -= 0.1f;
            printf("l:%f u:%f\n", _left, _up);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        case GLUT_KEY_RIGHT:
            if (_matlabfilesCount>1)
            {
                _currenttexture++;

                _currenttexture %=(_textures.size()-1);
                _valueChanged = true;
            }
            _left += 0.1f;
            printf("l:%f u:%f\n", _left, _up);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        case GLUT_KEY_UP:
            _up += 0.1f;
            printf("l:%f u:%f\n", _left, _up);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        case GLUT_KEY_DOWN:
            _up -= 0.1f;
            printf("l:%f u:%f\n", _left, _up);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        }

    }
   
    void MatlabFieldViewer::mouse(int button, int state, int x, int y)
    {
        _cmdManager.dispatchMouse(button);

        _info.str("");
    }

    void MatlabFieldViewer::update()
    {

    }

    void MatlabFieldViewer::timer(int updateRateMs)
    {
     
        if (currentZ==DEPTH-1)//we need to initialize our texture from the rendering thread
        {   
            newTexture();
        }
        if (currentZ != -1)
        {
            glEnable(GL_TEXTURE_3D);

            if (_textures.size() ==0)
            {
                 newTexture(TextureFactory::FLOAT);
                 //our target texture that will be displayed is bound to texture unit 0
                 glActiveTexture(GL_TEXTURE0);
                      
                glBindTexture(GL_TEXTURE_3D, _textures[0]);
                 glTexSubImage3D(GL_TEXTURE_3D,0,0,0,0,WIDTH, HEIGHT,DEPTH,GL_RED,GL_FLOAT,(GLvoid*)image2);

                checkGLErrors("After inittexture");
            }

            newTexture(TextureFactory::RED);
                 
            glActiveTexture(GL_TEXTURE1);

            glBindTexture(GL_TEXTURE_3D,_textures.back());

            glTexSubImage3D(GL_TEXTURE_3D,0,0,0,0,WIDTH, HEIGHT,DEPTH,GL_RED,GL_FLOAT,(GLvoid*)image2);
            Statistics stats = TextureFactory::statistics(_textures.back(),WIDTH, HEIGHT,DEPTH);
            //autoadjust boost param to gain a rms of 0.75; 
            float autoboost = 0.75/stats.rms;
            if (PropertyManager::instance().getPropertyValue("boost")==1.0f)//no custom propertyvalue set
                PropertyManager::instance().setPropertyValue("boost",autoboost);

            std::stringstream sstream;
            sstream << "Read Slice"  << currentZ << std::flush;
            checkGLErrors(sstream.str());
            currentZ = -1;
            glDisable(GL_TEXTURE_3D);
         
            _bIsInitialized = true;
            _valueChanged = true;
        }
        if (_animationRunning && _textures.size()>0) //switch between textures
        {
            if (PropertyManager::instance().getPropertyValue("reverseTime"))
            {
                if (_currenttexture == 0)
                    _currenttexture =_textures.size()-1;

                _currenttexture--;
               
                _currenttexture %=(_textures.size()-1);
            }
            else
            {
                _currenttexture++;
                _currenttexture %=(_textures.size()-1);
            }
            
            _valueChanged = true;
        }

        if (_textures.size() > 1 && (_valueChanged || _cmdManager.checkNResetDirty()))
        {
            _shader->use();

            //very import step: bind the textures to the shader
            glBindImageTexture(0, _textures[0], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);
            glBindImageTexture(1, _textures[_currenttexture+1], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);


            PropertyManager::instance().writeShaderValues(_shader);
            _shader->setUniform("WIDTH", (unsigned int)WIDTH);
            _shader->setUniform("HEIGHT", (unsigned int)HEIGHT);
            _shader->setUniform("DEPTH", (unsigned int)DEPTH);
            _shader->setUniform("doCut",(unsigned int)_bCutMode);
            glDispatchCompute(WIDTH/WORKGROUPSIZE, HEIGHT/WORKGROUPSIZE, DEPTH/WORKGROUPSIZE); 
            _valueChanged = false;

            checkGLErrors("After compute");
        }

        FieldViewerBase::timer(updateRateMs);
    }

    void MatlabFieldViewer::draw_impl()
    { 
        if(error)
            boost::rethrow_exception(error);

        glActiveTexture(GL_TEXTURE0);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        FieldViewerBase::draw_impl();
    }

}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='