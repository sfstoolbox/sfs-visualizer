/* 
* \file shadersoundfieldviewergreen.cpp
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

#include "shadersoundfieldviewergreen.h"
#include "application.h"
#include "cameracontrol.h"
#include "matio_private.h"
#include "shader.h"
#include "texturefactory.h"
#include "fieldviewerbase.h"
#include <exception>
#include "SOIL.h"

namespace sfs_visualizer
{
    GLint _colorMapId;
    ShaderSoundfieldViewerGreen::ShaderSoundfieldViewerGreen(void)
        :
        _left(0.0f),
        _up (0.0f),
        _back(0.5f),
        _radiusScale(1.0),
        _bufferpointer(0.0),
        _bufferspeed(1.0f),
        _speedOfSound(343.0f),
        _sampleRate(44100.0f),
        _nonlinearSlicePower(1)
    {
        glActiveTexture(GL_TEXTURE0);
        newTexture();
        checkGLErrors("konstruktor0");
        _computeShader = new Shader("computeGreen.vert",ShaderType::Compute);
        checkGLErrors("konstruktor1");
        _computeShader->createProgram();
        checkGLErrors("konstruktor2");
        _computeShader->attachShader(_computeShader->_shaderId);
        checkGLErrors("konstruktor3");
        _computeShader->linkProgram();
        checkGLErrors("konstruktor4");
        _computeShader->setUniform("radiusScale",_radiusScale);
        _computeShader->setUniform("bufferpointer",_bufferpointer);
        _computeShader->setUniform("c",_speedOfSound);
        _computeShader->setUniform("fs",_sampleRate);
        _computeShader->setUniform("WIDTH", (unsigned int)WIDTH);
        _computeShader->setUniform("HEIGHT", (unsigned int)HEIGHT);
        _computeShader->setUniform("DEPTH", (unsigned int)DEPTH);
        _computeShader->setUniform("BUFFSIZE", (unsigned int)BUFFSIZE);
        _computeShader->setUniform("virtSource",_left,_up,_back);
        _computeShader->setUniform("haveCustomSources",(unsigned int)0);
            
        checkGLErrors("konstruktor5");

        toggleLightning();
        initProperties();       
        loadSourcesFromCSV();
       
    }

void ShaderSoundfieldViewerGreen::initProperties()
{
    Property& opacity = PropertyManager::instance().addShaderProperty("opacity",1e-2,_computeShader);
    _cmdManager.addMouseCommand("increment Opacity",19,"Mousewheel+Ctrl",opacity,PropertyCommand::Mul);
    _cmdManager.addMouseCommand("decrement Opacity",20,"Mousewheel+Ctrl",opacity,PropertyCommand::Div);

    Property& _sources = PropertyManager::instance().addShaderProperty("SOURCES",4.0f,_computeShader);
    _cmdManager.addCommand("increment number of sources",'+',"+",_sources,PropertyCommand::Add);
    _cmdManager.addCommand("decrement number of source",'*',"*",_sources,PropertyCommand::Sub);
    _sources.Step(1.0);
    _sources.Min(0.0);
   

    Property& prop1 = PropertyManager::instance().addShaderProperty("opacityCutted",1e-4,_computeShader);
    _cmdManager.addMouseCommand("increment opacity of cutted area",27,"Mousewheel+Shift+LCtrl",prop1,PropertyCommand::Mul);
    _cmdManager.addMouseCommand("decrement opacity of cutted area",28,"Mousewheel+Shift+LCtrl",prop1,PropertyCommand::Div);

    Property& multiplier = PropertyManager::instance().addShaderProperty("multiplier",1.0,_computeShader);
    _cmdManager.addCommand("increment multiplier for postamplification",'m',"m",multiplier,PropertyCommand::Mul);
    _cmdManager.addCommand("decrement multiplier for postamplification",'M',"M",multiplier,PropertyCommand::Div);


    Property& prop2 = PropertyManager::instance().addShaderProperty("sourceRadius",5e-3,_computeShader);
    _cmdManager.addCommand("increment Displayradius of source",'g',"g",prop2,PropertyCommand::Add);
    _cmdManager.addCommand("decrement Displayradius of source",'G',"G",prop2,PropertyCommand::Sub);
    prop2.Min(-1.0f);
    prop2.Step(0.001f);

    Property& _scale = PropertyManager::instance().addShaderProperty("scale",1.0f,_computeShader);
    _cmdManager.addCommand("increment metrical scaling ",'p',"p",_scale,PropertyCommand::Mul);
    _cmdManager.addCommand("decrement metrical scaling",'P',"P",_scale,PropertyCommand::Div);
   

    Property& minDB = PropertyManager::instance().addShaderProperty("MINDB",-45.0f,_computeShader);
    _cmdManager.addCommand("increment minimum dB value",'d',"d",minDB,PropertyCommand::Add);
    _cmdManager.addCommand("decrement maximum dB value",'D',"D",minDB,PropertyCommand::Sub);
    minDB.IsVisible(false);
    minDB.Min(-200);
    minDB.Max(0);

    Property& useDB = PropertyManager::instance().addShaderProperty("db",0.0f,_computeShader);
    _cmdManager.addCommand("use dB",'3',"3",useDB,PropertyCommand::Toggle);

    Property& doCut = PropertyManager::instance().addShaderProperty("doCut",0.0f,_computeShader);
    _cmdManager.addCommand("cut planes",'c',"c",doCut,PropertyCommand::Toggle);

    Property& colormode = PropertyManager::instance().addShaderProperty("colormode",0.0f,_computeShader);
    _cmdManager.addCommand("increment colormode",'2',"2",colormode,PropertyCommand::Add);
    _cmdManager.addCommand("decrement colormode",'"',"\"",colormode,PropertyCommand::Sub);
    colormode.IsVisible(false);
    colormode.Min(0.0);

    Property& freq = PropertyManager::instance().addShaderProperty("frequency",1e3,_computeShader);
    freq.Max(_sampleRate/2);
    freq.Min(0.0f);
    freq.Step(1.01f);
    _cmdManager.addMouseCommand("increment Frequency of source",3,"Mousewheel",freq,PropertyCommand::Mul);
    _cmdManager.addMouseCommand("decrement Frequency of source",4,"Mousewheel",freq,PropertyCommand::Div);

    Property& boost = PropertyManager::instance().addShaderProperty("boost",1.0,_computeShader);
    
    freq.Step(1.1f);

    _cmdManager.addCommand("increment Amplification of source",'b',"b",boost,PropertyCommand::Mul);
    _cmdManager.addCommand("decrement Amplification of source",'B',"B",boost,PropertyCommand::Div);

    Property& time = PropertyManager::instance().addShaderProperty("time",0.0f,_computeShader);
    time.Min(-100000.0f);
    time.Step(_bufferspeed);
    _cmdManager.addMouseCommand("increment Time",11,"Mousewheel+Shift",time,PropertyCommand::Add);
    _cmdManager.addMouseCommand("decrement Time",12,"Mousewheel+Shift",time,PropertyCommand::Sub);


    
    //controls the alphavalue for each calculated point 
    FieldViewerBase::initProperties();
}


    void ShaderSoundfieldViewerGreen::loadSourcesFromCSV()
    {
        if (Application::instance().getCommandlineOptions().count("sourcedefinition") == 0) 
        {
            std::cout << "no sourcedefinitions found-> using (default) sources on circle" <<std::endl;
            return;
        }

        std::string strFileName = Application::instance().getCommandlineOptions()["sourcedefinition"].as<std::string >();
        std::cout << "using Sourcedefitions from '"<< strFileName << "'" <<std::endl;
        std::string line;
        std::ifstream myfile (strFileName);
        std::stringstream outstream;
        std::vector<float> sources;
        char delimiter = ',';
        if (!myfile.is_open())
            throw std::bad_exception("Sourcedefitions not found");

        int lineCount =0;
        while ( getline (myfile,line) )
        {
            //read the entries
            std::stringstream ss(line);
            std::string item;
            std::vector<std::string> elems;
            while (std::getline(ss, item, delimiter)) {
                elems.push_back(item);
            }
            if (elems[0]!="x" && elems.size() == 7) //ignore header line and empty lines
            {
                sources.push_back((float) atof(elems[0].c_str()));
                sources.push_back((float) atof(elems[1].c_str()));
                sources.push_back((float) atof(elems[2].c_str()));
                sources.push_back((float) atof(elems[3].c_str()));
                sources.push_back((float) atof(elems[4].c_str()));
                sources.push_back((float) atof(elems[5].c_str()));
                sources.push_back((float) atof(elems[6].c_str()));
                //sources.push_back((float) atof(elems[7].c_str()));     
            }
        }
        myfile.close();
        fillSSBO(sources.data(), sources.size()*sizeof(float),2);

        PropertyManager::instance().setPropertyValue("SOURCES",sources.size());
        _computeShader->setUniform("haveCustomSources",(unsigned int)1);
        std::cout <<sources.size() << " source loaded" <<std::endl;
    }

    ShaderSoundfieldViewerGreen::~ShaderSoundfieldViewerGreen(void)
    {

    }

    static const float col[] = {1.0f, 1.0f, 1.0f, 1.0f};
    void ShaderSoundfieldViewerGreen::key(unsigned char key, int x, int y)
    {
        int mod = glutGetModifiers();
       if (mod && mod != GLUT_ACTIVE_SHIFT)//shift is ignored
            key&=mod;

        _cmdManager.dispatchKey(key);

        FieldViewerBase::key(key,x,y);
        float dAlpha = 0.0f;
        float dwaveLength = 0.0f;
        float dBoost = 0.0f;
        GLfloat mat_emission[] = {0.5, 0.5, 0.5, 1.0};
        GLfloat mat_no_emission[] = {0.0,0.0, 0.0, 0.0};

        switch ( key ) {
        
        case 's':
            _sliceDist*=1.01f;
            _info << "abstand=" << _sliceDist;
            printf("abstand %f                              \r",_sliceDist);
            break;
        case 'S':
            _sliceDist/=1.01f;
            _info << "abstand=" << _sliceDist;
            printf("abstand %f                              \r",_sliceDist);
            break;

        case '-':
            _bufferspeed*=1.1;

            _info << "bufferspeed=" << _bufferspeed;
            printf("bufferspeed %f                              \r",_bufferspeed);
            break;
        case '_':
            _bufferspeed*=0.9;

            _info << "bufferspeed=" << _bufferspeed;
            printf("bufferspeed %f                              \r",_bufferspeed);
            break;
       
        case '1':

            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

            break;
        case '!':
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            break;
          
          
        case 't':
            PropertyManager::instance().resetPropertyValue("time");
            _bufferpointer =0;
            break;
        case '0':

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
            break;
        case '=':

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_no_emission);
            break;
        default:
            // printf("Key %d corresponding to char '%c' is not assigned to any action.                              \r", key, key);
            break;
        }
    }
    
    void ShaderSoundfieldViewerGreen::specialkey(int key, int x, int y)
    {
int mod = glutGetModifiers();
        switch (key) {

        case GLUT_KEY_LEFT :
            _left -= 0.01f;
            printf("virtualsource x:%f y:%f z:%f\r",_left,_up,_back);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        case GLUT_KEY_RIGHT :
            _left += 0.01f;
            printf("virtualsource x:%f y:%f z:%f\r",_left,_up,_back);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        case GLUT_KEY_UP :
            if (mod==GLUT_ACTIVE_SHIFT)
                _back += 0.01f;
            else
                _up += 0.01f;
            printf("virtualsource x:%f y:%f z:%f\r",_left,_up,_back);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
        case GLUT_KEY_DOWN :
            if (mod==GLUT_ACTIVE_SHIFT)
                _back -= 0.01f;
            else
                _up -= 0.01f;
          
            printf("virtualsource x:%f y:%f z:%f\r",_left,_up,_back);
            _info.str("");
            _info << "l=" << _left << "u=" << _up;
            break;
       
        }
    _valueChanged = true;
    }

    void ShaderSoundfieldViewerGreen::mouse(int button, int state, int x, int y)
    {
        _cmdManager.dispatchMouse(button);
    }

    void ShaderSoundfieldViewerGreen::update()
    {
        
    }
    
    void ShaderSoundfieldViewerGreen::timer(int updateRateMs)
    {
        

        checkGLErrors("update0");
        
        checkGLErrors("update1");
        _computeShader->use();

        glBindImageTexture(0, _textures[_currenttexture], 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

        int _colorMapId = TextureFactory::Alltextures()[1];
        glBindTexture(GL_TEXTURE_2D, _colorMapId);
        PropertyManager::instance().writeShaderValues(_computeShader);
      
        _computeShader->setUniform("virtSource",_left,_up,_back);
       
        if (_animationRunning || _valueChanged || _cmdManager.checkNResetDirty())//prevent unnecessary recalculations
        {
            //call our compute shader
            glDispatchCompute(WIDTH/WORKGROUPSIZE, HEIGHT/WORKGROUPSIZE, DEPTH/WORKGROUPSIZE); 
           
            checkGLErrors("update2");
            if (_animationRunning)
            {
                float old = PropertyManager::instance().getPropertyValue("time");
                if (PropertyManager::instance().getPropertyValue("reverseTime"))
                    old-=updateRateMs*_bufferspeed;
                else
                    old+=updateRateMs*_bufferspeed;
                PropertyManager::instance().setPropertyValue("time",old);
            }

           
            _bIsInitialized = true;
        }
  
        FieldViewerBase::timer(updateRateMs);
       _valueChanged = false; 
    }

    void ShaderSoundfieldViewerGreen::draw_impl()
    {
        glActiveTexture(GL_TEXTURE0);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        FieldViewerBase::draw_impl();
    }

}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='