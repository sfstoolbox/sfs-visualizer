/**
 * \class FieldViewerBase
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

#ifndef FIELDVIEWERBASE_H
#define FIELDVIEWERBASE_H
#include "renderobject.h"
#include "renderobjectbase.h"
#include "stdincl.h"
#include "shader.h"
#include "infoprovider.h"
#include "property.h"
#include "command.h"
#include "commandmanager.h"
#include "propertymanager.h"
#include "texturefactory.h"

namespace sfs_visualizer
{
//fixed size for SSBOs
#define BUFFSIZE 22048

    /**
     * A Base class for classes that can show soundfields either with shearwarping or a raytracer                              
     **/
    class FieldViewerBase:public RenderObjectBase<FieldViewerBase>, public InfoProvider
    {
    public:
        FieldViewerBase(void);
        ~FieldViewerBase(void);

        virtual void key(unsigned char key, int x, int y);
        virtual void timer(int updateRateMs);
    public: ///Informators and Mutators

        virtual const std::string Info() const;
        virtual void IsVisible(bool bVisible);

    protected:

        virtual void draw_impl();
        virtual void newTexture(TextureFactory::TYPE type=TextureFactory::FLOAT);
        virtual void initProperties();

        void addQuadslice(float zOffset);
        void switchAlpha();
       
        void blend(int blend);
        ///Toggle between Raytracer and Shearwarping
        void toggleRenderMode();
        void drawBorder();
        ///Initializes and creates a Shader Storage Buffer
        void initSSBO(GLuint index,bool bRand);
        ///fill a Shader Storage Buffer with data
        void fillSSBO(const GLvoid* buffer, GLsizeiptr size,GLuint index);

    protected:

        std::stringstream _info;
        std::vector<GLuint> _textures;
        float z;
        Shader* _fragmentShader;
        Shader* _vertexShader;
        int _currenttexture;
        int _alphablendfunction;
        int _alphablendmode ;
        float _alphatest;
        int WIDTH;
        int HEIGHT;
        int DEPTH;
        int WORKGROUPSIZE;
        unsigned int _sources;
        bool _valueChanged;
        int _animationRunning;
        bool _bDrawBorder;
        CommandManager _cmdManager;
        float _sliceDist;

    private:
   
        int _oldnSlizes;

    };

} 
#endif 