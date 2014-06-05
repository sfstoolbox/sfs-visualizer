/**
 * \class Shader
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

#ifndef SHADER
#define SHADER

#include "forwarddecl.h"
#include "stdincl.h"

namespace sfs_visualizer
{

    /**
    * The available shader types                                   
    **/
    enum ShaderType{Fragment,Vertex,Geometry,Compute};
    /**
    * The available uniform types                                   
    **/
    enum UniformType {
        INT,
        FLOAT1,
        FLOAT2,
        FLOAT3,
        FLOAT4
    };

    /**
    * A Uniform capsulates a Shader variable that is stored in graphicsmemory                                 
    **/
    struct Uniform {
        UniformType type;
        union {
            int i;
            float f[4];
        } value;
    };

/**
* class to load glsl-shaders and wraps communication with uniform-values, that are the variables of shaders                                   
**/
class Shader
{
public:
    Shader(std::string strFile,ShaderType type);
    ~Shader(void);

    bool hasUniform(const std::string &name);
    void setUniform(const std::string &name, int v1);
    void setUniform(const std::string &name, unsigned int v1);
    void setUniform(const std::string &name, float v1);
    void setUniform(const std::string &name, float v1, float v2);
    void setUniform(const std::string &name, float v1, float v2, float v3);
    void setUniform(const std::string &name, float v1, float v2, float v3, float v4);
    void setUniform(const std::string &name,int count, float *v1);
    void setUniformArray(const std::string &name,int count, float *v1);
    void setUniformVec4Array(const std::string &name,int count, float *v1);
    void setUniformArray(const std::string &name, int count, unsigned int *v1);
    void setUniformMatrix(const std::string &name, int count, float *v1);
    void updateUniforms();

    void load();
    void unLoad();
    void attachShader(GLuint shader_id);
    void createProgram(); 
    void linkProgram();
    void use(); 
    bool isActive();

public:
    
    GLuint _shaderId;

private:

    void printShaderInfoLog(GLuint obj);
    std::string readFile(const std::string& strFileName);
    void doPreprocessing(std::string& strText);

private:

    bool _isActive;
    std::string _strFile;
    ShaderType _eType;
    std::map<std::string, sfs_visualizer::Uniform> _uniforms;
    GLuint _programId;

};
}
#endif

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='