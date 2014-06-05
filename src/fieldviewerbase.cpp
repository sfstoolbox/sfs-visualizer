/* 
* \file fieldviewerbase.cpp
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

#include "fieldviewerbase.h"
#include "shader.h"
#include "texturefactory.h"
#include "application.h"
#include "cameracontrol.h"
#include "matlabfileadapter.h"

namespace sfs_visualizer
{

    FieldViewerBase::FieldViewerBase(void)
        : _currenttexture(0),_alphatest(0.1f),_alphablendfunction(0),_alphablendmode(0),z (0.0f),
        WIDTH(64),
        HEIGHT(64),
        DEPTH(64),
        _sources(30),
        _valueChanged (true),
        _animationRunning(0),
        _sliceDist(1),
        _bDrawBorder(false)
    {
        if (Application::instance().getCommandlineOptions().count("gridWidth"))
            WIDTH = Application::instance().getCommandlineOptions()["gridWidth"].as<int>();

        if (Application::instance().getCommandlineOptions().count("gridHeight"))
            HEIGHT = Application::instance().getCommandlineOptions()["gridHeight"].as<int>();

        if (Application::instance().getCommandlineOptions().count("gridDepth"))
            DEPTH = Application::instance().getCommandlineOptions()["gridDepth"].as<int>();
          
        WORKGROUPSIZE = Application::instance().getCommandlineOptions()["workgroupsize"].as<int>();

        checkGLErrors("FieldViewerBase::konstruktor2");

        toggleAlphaBlending();
        blend(_alphablendmode);
        toggleLightning();
        checkGLErrors("FieldViewerBase::konstruktor3");
       
        initSSBO(1,true);

        checkGLErrors("FieldViewerBase::konstruktor4");
        _fragmentShader = new Shader("raytrace.vert",ShaderType::Fragment);
        _vertexShader = new Shader("simple.vert",ShaderType::Vertex);
    }

    void FieldViewerBase::initProperties()
    {
        Property& useRayTracer = PropertyManager::instance().addProperty("UseRayTracer",0.0f);
        _cmdManager.addCommand("use Raytracer",'4',"4",useRayTracer,PropertyCommand::Toggle);
        useRayTracer.IsVisible(false);
        
        Property& rayTraceThresh = PropertyManager::instance().addShaderProperty("thresh",1.0,_fragmentShader);
        _cmdManager.addCommand("increment Treshold of raytracer",'y',"y",rayTraceThresh,PropertyCommand::Mul);
        _cmdManager.addCommand("decrement Treshold of raytracer",'Y',"Y",rayTraceThresh,PropertyCommand::Div); 
        rayTraceThresh.IsVisible(false);

        Property& _RayTraceStepwidth = PropertyManager::instance().addShaderProperty("stepwidth",0.005,_fragmentShader);
        _cmdManager.addCommand("increment stepwidth of raytracer",'x',"x",_RayTraceStepwidth,PropertyCommand::Mul);
        _cmdManager.addCommand("decrement stepwidth of raytracer",'X',"X",_RayTraceStepwidth,PropertyCommand::Div); 
        _RayTraceStepwidth.IsVisible(false);

        Property& _RayTraceAlphaboost = PropertyManager::instance().addShaderProperty("alphaboost",1.0,_fragmentShader);
        _cmdManager.addCommand("increment alphaboost of raytracer",'<',"<",_RayTraceAlphaboost,PropertyCommand::Mul);
        _cmdManager.addCommand("decrement alphaboost of raytracer",'>',">",_RayTraceAlphaboost,PropertyCommand::Div); 
        _RayTraceAlphaboost.IsVisible(false);

        Property& _RayTraceBlur = PropertyManager::instance().addShaderProperty("blur",0.0,_fragmentShader);
        _cmdManager.addCommand("increment blur of raytracer",'n',"n",_RayTraceBlur,PropertyCommand::Add);
        _cmdManager.addCommand("decrement blur of raytracer",'N',"N",_RayTraceBlur,PropertyCommand::Sub); 
        _RayTraceBlur.IsVisible(false);
        _RayTraceBlur.Step(0.001);

        Property& _reverseTime = PropertyManager::instance().addProperty("reverseTime",0.0);
        _cmdManager.addCommand("reverse animation",'u',"u",_reverseTime,PropertyCommand::Toggle);        
   
         Property& _RayTraceQuality = PropertyManager::instance().addShaderProperty("quality",1.0,_fragmentShader);
        _cmdManager.addCommand("increment quality of raytracer",'v',"v",_RayTraceQuality,PropertyCommand::Add);
        _cmdManager.addCommand("decrement quality of raytracer",'V',"V",_RayTraceQuality,PropertyCommand::Sub); 
        _RayTraceQuality.Step(1.0);
        _RayTraceQuality.Min(1.0);
        _RayTraceQuality.IsVisible(false);

        Property& nslices = PropertyManager::instance().addProperty("NSLIZES",128);
        _cmdManager.addCommand("increment number of slices",'r',"r",nslices,PropertyCommand::Add);
        _cmdManager.addCommand("decrement number of slices",'R',"R",nslices,PropertyCommand::Sub); 
        nslices.Step(1);
        nslices.Min(1);

        Property& yRotSpeedProp = PropertyManager::instance().getProperty("yRotSpeed");
        _cmdManager.addCommand("increment speed of autorotation around y-axis",'o',"o",yRotSpeedProp,PropertyCommand::Add,false);
        _cmdManager.addCommand("increment speed of autorotation around y-axis",'O',"O",yRotSpeedProp,PropertyCommand::Sub,false); 

        Property& xRotSpeedProp = PropertyManager::instance().getProperty("xRotSpeed");
        _cmdManager.addCommand("increment speed of autorotation around x-axis",'o'&GLUT_ACTIVE_CTRL,"o + left ctrl",xRotSpeedProp,PropertyCommand::Add,false);
        _cmdManager.addCommand("increment speed of autorotation around x-axis",3,"O + left ctrl",xRotSpeedProp,PropertyCommand::Sub,false); 

        Property& zRotSpeedProp = PropertyManager::instance().getProperty("zRotSpeed");
        _cmdManager.addCommand("increment speed of autorotation around z-axis",'o'&GLUT_ACTIVE_ALT,"o + left alt",zRotSpeedProp,PropertyCommand::Add,false);
        _cmdManager.addCommand("increment speed of autorotation around z-axis",5,"O + left alt",zRotSpeedProp,PropertyCommand::Sub,false); 
     
        PropertyManager::instance().load();//Load defaults etc
        PropertyManager::instance().setPropertyValue("NSLIZES",128.0f);
        _oldnSlizes = PropertyManager::instance().getPropertyValue("NSLIZES");

    }

    FieldViewerBase::~FieldViewerBase(void)
    {
        delete _fragmentShader; _fragmentShader = NULL;
        delete _vertexShader; _vertexShader = NULL;
    }

   void FieldViewerBase::IsVisible(bool bVisible)
    {
        _cmdManager.dispatchKey('b');//HACK to trigger a refresh
        _cmdManager.dispatchKey('B');
        RenderObjectBase<FieldViewerBase>::IsVisible(bVisible);
        toggleRenderMode();
    }

    

    void FieldViewerBase::newTexture(TextureFactory::TYPE type)
    {
       GLuint texID = TextureFactory::build_texture3D(type, WIDTH, HEIGHT, DEPTH);
       _textures.push_back(texID);
       checkGLErrors("Texture initialisierung2");       
    }

    std::map<GLuint,GLuint> ind2ssb;
 
    void FieldViewerBase::fillSSBO(const GLvoid* buffer, GLsizeiptr sizeInBytes,GLuint index)
    {
        GLuint ssb;
        if(ind2ssb.find(index) != ind2ssb.end())
        {
            ssb = ind2ssb[index];
            glDeleteBuffers(1,&ssb);
            glGenBuffers(1, &ssb);
        }
        else
        {
            glGenBuffers(1, &ssb);
            ind2ssb[index]=ssb;
        }

        // Get a handle
        // Create buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssb);

        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeInBytes, buffer, GL_DYNAMIC_COPY_ARB);
        // Now bind the buffer to the shader
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssb);
    }

    void FieldViewerBase::initSSBO(GLuint index,bool bRand=false)
    {
        std::array <GLfloat, BUFFSIZE> _buffer;

        for (int x = 0; x < BUFFSIZE; ++x)
        {
            // Set the buffer to random values (0...1)
            if (bRand)
                _buffer[x] =random()+0.5f;
            else
                _buffer[x] = 0.0f;
        }

        fillSSBO(_buffer.data(), BUFFSIZE*sizeof(float),index);
    }

    void FieldViewerBase::toggleRenderMode()
    {
        //toggeling this causes crashes :-/
        if (PropertyManager::instance().getPropertyValue("UseRayTracer"))
        {
            _alphablendmode=2; 
            _oldnSlizes = PropertyManager::instance().getPropertyValue("NSLIZES");
            PropertyManager::instance().setPropertyValue("NSLIZES",1.0f);

            _fragmentShader->load();
            _vertexShader->load();
            _fragmentShader->createProgram();
            _vertexShader->createProgram();

            checkGLErrors("FieldViewerBase::konstruktor4");
            _fragmentShader->attachShader(_fragmentShader->_shaderId);
            _fragmentShader->attachShader(_vertexShader->_shaderId);
            checkGLErrors("FieldViewerBase::konstruktor5");
            _fragmentShader->linkProgram();
            checkGLErrors("FieldViewerBase::konstruktor6");
            

            PropertyManager::instance().setPropertyValue("UseRayTracer",1.0f);
        }
        else
        {
            _alphablendmode=0; 
            PropertyManager::instance().setPropertyValue("UseRayTracer",0.0f);

            PropertyManager::instance().setPropertyValue("NSLIZES",_oldnSlizes);
            _fragmentShader->unLoad();
            _vertexShader->unLoad();
            checkGLErrors("unload shader");

        }
           
        blend(_alphablendmode);
    }

    void FieldViewerBase::key(unsigned char key, int x, int y)
    {
        _cmdManager.dispatchKey(key);

        _info.str("");
     
        float dAlpha = 0.0f;
        float dwaveLength = 0.0f;
        float dBoost = 0.0f;
        GLfloat mat_emission[] = {0.5, 0.5, 0.5, 1.0};
        GLfloat mat_no_emission[] = {0.0,0.0, 0.0, 0.0};

        switch ( key ) {
        case 'j':
            MatlabFileAdapter::writeTexture2Matlabfile(_textures[_currenttexture],WIDTH,HEIGHT,DEPTH,"data.mat");

            printf("                                        \rmatfile written");
            break;
        case 'J':
            TextureFactory::statistics(_textures[_currenttexture],WIDTH,HEIGHT,DEPTH);

            break;
        case '4':
            toggleRenderMode();
            break;
        case 'a':
            ++_alphablendmode%=10; 
            blend(_alphablendmode);
            printf("alphablendmode %i                              \r",_alphablendmode);
            _info << "alphablendmode=" << _alphablendmode;
            break;
        case 'A':
            ++_alphablendfunction %=2;
            switchAlpha();
            printf("alphablendfunction %i                              \r",_alphablendfunction);
            _info << "alphablendfunction=" << _alphablendfunction;
            glEnable(GL_ALPHA_TEST);
            break;
        case 'q':

            if (_alphatest <=1.0f)
            {
                if (_alphatest>0.9f )
                    dAlpha=0.001f;
                else
                    dAlpha=0.01f;
            }
            _alphatest+=dAlpha;

            switchAlpha();
            _info << "alphatest=" << _alphatest;
            printf("alphatest %f                              \r",_alphatest);
            break;
        case 'Q':
            if (_alphatest >=0.0f)
            {
                if (_alphatest < 0.1f)
                    dAlpha=-0.001f;
                else
                    dAlpha=-0.01f;
            }
            _alphatest+=dAlpha;
            switchAlpha();
            _info << "alphatest=" << _alphatest;
            printf("alphatest %f                              \r",_alphatest);
            break;
        case 'w':
            glDisable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            _alphablendfunction = 0;
            switchAlpha();
            break;
        case 'W':
            glEnable(GL_BLEND);
            glEnable(GL_ALPHA_TEST);
            _alphablendfunction = 1;
            switchAlpha();
            break;
        case '8':
            glEnable(GL_ALPHA_TEST);
            _alphablendfunction = 0;
            switchAlpha();

            break;
        case '(':
            glDisable(GL_ALPHA_TEST);

            break;
        case 'z':
            z-=0.01f;
            _info << "z=" << z;
            printf("z %f                              \r",z);
            break;
        case 'Z':
            z+=0.01f;
            _info << "z=" << z;
            printf("z %f                              \r",z);
            break;
        case '*':
            _sources--;
            _info << "_sources=" << _sources;
            printf("_sources %i                              \r",_sources);

            break;
        case '+':
            _sources++;
            _info << "_sources=" << _sources;
            printf("_sources %i                              \r",_sources);

            break;
        case ' ':

            _animationRunning = !_animationRunning;
            break;
        }
       
    }
    void line(float x0,float y0,float z0,float x1,float y1,float z1)
    {
        glBegin(GL_LINES);
        glColor4f(1.0f,1.0f,1.0f,0.1f);
        glVertex3f(x0, y0, z0); // from
        glVertex3f(x1, y1, z1); // to
        glEnd(); // GL_LINES

    }
    void FieldViewerBase::drawBorder()
    {
        if (!_bDrawBorder)
            return;

        glPushMatrix();
        glLoadIdentity();
        CameraControl cam = sfs_visualizer::Application::instance().getEngine().Camera();

        glRotatef(-cam.Spin_x(), 1, 0, 0);
        glRotatef(-cam.Spin_y(), 0, 1, 0);

        glTranslatef(-0.5,- 0.5,  -0.5);

        //back
        line(0.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f);
        line(1.0f, 0.0f, 0.0f,1.0f, 1.0f, 0.0f);
        line(1.0f, 1.0f, 0.0f,0.0f, 1.0f, 0.0f);
        line(0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f);

        //front
        line(0.0f, 0.0f, 1.0f,1.0f, 0.0f, 1.0f);
        line(1.0f, 0.0f, 1.0f,1.0f, 1.0f, 1.0f);
        line(1.0f, 1.0f, 1.0f,0.0f, 1.0f, 1.0f);
        line(0.0f, 1.0f, 1.0f,0.0f, 0.0f, 1.0f);

        //left
        line(0.0f, 0.0f, 0.0f,0.0f, 0.0f, 1.0f);
        line(0.0f, 0.0f, 1.0f,0.0f, 1.0f, 1.0f);
        line(0.0f, 1.0f, 1.0f,0.0f, 1.0f, 0.0f);
        line(0.0f, 1.0f, 0.0f,0.0f, 0.0f, 0.0f);

        //right
        line(1.0f, 0.0f, 0.0f,1.0f, 0.0f, 1.0f);
        line(1.0f, 0.0f, 1.0f,1.0f, 1.0f, 1.0f);
        line(1.0f, 1.0f, 1.0f,1.0f, 1.0f, 0.0f);
        line(1.0f, 1.0f, 0.0f,1.0f, 0.0f, 0.0f);
     
        glPopMatrix();


    }
    void FieldViewerBase::blend(int blend)
    {
        switch(blend)
        {
        case 0:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Transparenz
            break;
        case 1:
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);		

            break;
        case 2:
            glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
            break;
        case 3:
            glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
            break;
        case 4:
            //	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
            //  glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_ALPHA); 
            glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_DST_ALPHA); 
            break;
        case 5:

            glBlendFunc(GL_ONE,GL_ONE); //RIchtig geil mit alphatest = 0.03 alphablendfunction=1
            //glBlendFunc(GL_DST_COLOR ,GL_ZERO); //
            break;
        case 6:
            //glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
            glBlendFunc(GL_DST_ALPHA,GL_DST_COLOR);					// sehr künstlerisch :-)
            break;
        case 7:
            glBlendFunc(GL_SRC_ALPHA,GL_ZERO);					// langweilig
            break;
        case 8:
            glBlendFunc(GL_SRC_ALPHA,GL_DST_COLOR);	

            // ok
            break;
        case 9:
            glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);	//sieht eigentlich ziemlich gut aus mit Tasten l, 8 und 1,q
            // ok
            break;

        }
       
        _info << "Blend=" << blend;
    }
    void FieldViewerBase::switchAlpha()
    {
        switch(_alphablendfunction)
        {
        case 0:
            glAlphaFunc(GL_GREATER,_alphatest);
            break;
        case 1:
            glAlphaFunc(GL_LESS,_alphatest);
            break;
        }
    }

    const std::string FieldViewerBase::Info() const
    {
        return _info.str();
    }
   
    void FieldViewerBase::addQuadslice(float zOffset)
    {
        glBegin(GL_QUADS);
        
        glNormal3f( 0.0f, 1.0f, 0.0f); //lightning normals

        glTexCoord3d(0.0f, 0.0f, 0.0f+z+zOffset);	
        glVertex3f(0.0f, 0.0f, 0.0f+zOffset); //bottom left 

        glTexCoord3d(1.0f, 0.0f, 0.0f+z+zOffset);	
        glVertex3f(1.0f, 0.0f, 0.0f+zOffset);//bottom right 

        glTexCoord3d(1.0f, 1.0f, 0.0f+z+zOffset);	
        glVertex3f(1.0f, 1.0f, 0.0f+zOffset); //top right

        glTexCoord3d(0.0f, 1.0f, 0.0f+z+zOffset);	     
        glVertex3f(0.0f, 1.0f, 0.0f+zOffset);//top left

        glEnd();
   
    }
    void FieldViewerBase::timer(int updateRateMs)
    {
        if (PropertyManager::instance().getPropertyValue("UseRayTracer"))
        {
            _fragmentShader->use();
            PropertyManager::instance().writeShaderValues(_fragmentShader);

            CameraControl cam = sfs_visualizer::Application::instance().getEngine().Camera();
          
            _fragmentShader->setUniform("zoom",(float)(cam.Zoom()*0.01f));
            _fragmentShader->setUniform("WIDTH",(unsigned int)WIDTH);
            _fragmentShader->setUniform("HEIGHT",(unsigned int)HEIGHT);
            _fragmentShader->setUniform("DEPTH",(unsigned int)DEPTH);
           
            //calculate the current rotationmatrix for the raytracer
            glPushMatrix();
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glRotatef(-cam.Spin_y(), 0, 1, 0);
            glRotatef(-cam.Spin_x(), 1, 0, 0);
            glRotatef(cam.Spin_z(), 0, 0, 1);
            GLfloat matrix[16]; 
            glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
            _fragmentShader->setUniformMatrix("rotation",1,matrix);
            glPopMatrix();

            _fragmentShader->setUniform("width",(unsigned int)sfs_visualizer::Application::instance().getEngine().Width());
            _fragmentShader->setUniform("height",(unsigned int)sfs_visualizer::Application::instance().getEngine().Height());
        }    
    }
    void FieldViewerBase::draw_impl()
    {
        float y;
        float zOffset;
        float heightscale = 2.0f;
        int x;
        int i;
        const int BOX_SIZE = 7;

        GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 0.5f };
        GLfloat lightPos[] = {0.0f,0.0f,0.0f};
        GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat whiteSpecularMaterial[] = { 1.0, 1.0, 1.0 };
        GLfloat mShininess[] = { 128 };

        glLoadIdentity();

        CameraControl cam = sfs_visualizer::Application::instance().getEngine().Camera();

        // tell GL we want it to use this texture 
        if (_textures.size())
            glBindTexture(GL_TEXTURE_3D, _textures[0]);

        checkGLErrors("MatlabFieldViewer::draw_impl0");
       
        GLfloat global_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, global_ambient);

        GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 0.5f };
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        glEnable(GL_TEXTURE_3D);

        glTranslatef(-1.0, -1.0, -1.0);
       
        float scale = 2.0f;
        glScalef(scale, scale, scale);
     
        //here we draw our slices
        float nslizes = PropertyManager::instance().getPropertyValue("NSLIZES");
        for (i = 0; i < nslizes; i++)
        {
            addQuadslice(i*(_sliceDist / nslizes));
        }

        glDisable(GL_TEXTURE_3D);
        drawBorder();
    }


}