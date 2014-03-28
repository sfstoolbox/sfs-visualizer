#include "shader.h"
#include "stdincl.h"
#include "application.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>

namespace sfs_visualizer
{

    Shader::Shader(std::string strFile,ShaderType type)
        :_strFile(strFile), _eType(type),_isActive(false)
    {
        load();
    }

    Shader::~Shader(void)
    {
    }

    void Shader::doPreprocessing(std::string& strText)
    {
        //replace all #includes
        boost::regex regx("#include <(.*)\>$");// a regex to find all includes
        boost::sregex_iterator reg_it(strText.begin(), strText.end(), regx,boost::regex_constants::match_not_dot_newline);
        boost::sregex_iterator end;
        std::map<std::string,std::string> includes;
        for (; reg_it != end; ++reg_it) {
            boost::smatch m = (*reg_it);
            std::string strIncludeFilename = Application::instance().getCommandlineOptions()["glslfolder"].as<std::string >()+ "/" +m[1];
            std::string	strInclude = readFile(strIncludeFilename);
            includes[m.str()]=strInclude;
        }

        for ( std::map<std::string,std::string>::const_iterator it = includes.begin();
            it != includes.end(); ++it) {
                boost::replace_all(strText,  it->first, it->second);
        }

        //replace Workgroupsize
        int ws = Application::instance().getCommandlineOptions()["workgroupsize"].as<int>();
        std::stringstream tmp;
        tmp << ws <<std::flush;

        boost::replace_all(strText, "$WS", tmp.str());
    }

    std::string Shader::readFile(const std::string& strFileName)
    {
        std::string line;
        std::ifstream myfile (strFileName);
        std::stringstream outstream;

        if (myfile.is_open())
        {
            while ( std::getline (myfile,line) )
            {
                outstream << line << std::endl;
            }
            myfile.close();
        }

        std::string result = outstream.str();
        doPreprocessing(result);
        return result;
    }


    // it prints out shader info (debugging!)
    void Shader::printShaderInfoLog(GLuint obj)
    {
        int infologLength = 0;
        int charsWritten  = 0;
        char *infoLog;
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
        if (infologLength > 1)
        {
            infoLog = (char *)malloc(infologLength);
            glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);

            printf("ShaderInfoLog: %s\n",infoLog);
            free(infoLog);
        }
    }
    void printProgramInfoLog(GLuint obj)
    {
        int infologLength = 0;
        int charsWritten  = 0;
        char *infoLog;
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
        if (infologLength > 1)
        {
            infoLog = (char *)malloc(infologLength);
            glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);

            printf("ProgramInfoLog: %s\n",infoLog);
            free(infoLog);
        }
    }


    int ShaderType2Const(ShaderType type)
    {
        switch(type)
        {
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Compute:
            return GL_COMPUTE_SHADER;
        }
        return -1;
    }

    void Shader::unLoad() 
    {
        if (_isActive)
        {
            glDetachShader(_programId,_shaderId);
            glDeleteShader(_shaderId);
            _isActive = false; 
        }
    }



//Setup shaders
void Shader::load() 
	{
		
		//First, create our shaders 
		_shaderId = glCreateShader(ShaderType2Const(_eType));
		
		//Read in the programs
    std::string strGlslFile = Application::instance().getCommandlineOptions()["glslfolder"].as<std::string >()+ "/" +_strFile.c_str();

	  std::string	vs = readFile(strGlslFile);
		
		//Setup a few constant pointers for below
		const char * vv = vs.c_str();

		glShaderSource(_shaderId, 1, &vv, NULL);
		
		glCompileShader(_shaderId);
		
		printShaderInfoLog(_shaderId);
		
		
	}

bool Shader::isActive()
{
    return _isActive;
}
void Shader::attachShader(GLuint shader_id)
{
    glAttachShader(_programId,shader_id);
}
void Shader::use()
{
    glUseProgram(_programId);
    _isActive = true;
}

void Shader::createProgram()
{
_programId = glCreateProgram();


printProgramInfoLog(_programId);
}

void Shader::linkProgram()
{
 
	glProgramParameteriEXT(_programId,GL_GEOMETRY_INPUT_TYPE_EXT,GL_POINTS);
	//glProgramParameteriEXT(p,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_POINTS);
	glProgramParameteriEXT(_programId,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_TRIANGLE_STRIP);
	//glProgramParameteriEXT(p,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_TRIANGLES);
	//glProgramParameteriEXT(p,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_QUADS);

		int temp = 24;
	
		glProgramParameteriEXT(_programId,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
	
		glLinkProgram(_programId);
		glUseProgram(_programId);
		
		printProgramInfoLog(_programId);
}

void Shader::updateUniforms()
{
 for (std::map<std::string, Uniform>::iterator i = _uniforms.begin(); i != _uniforms.end(); ++i) {
        std::string name = i->first;
        Uniform uniform = i->second;
        GLint location = glGetUniformLocation(_programId, name.c_str());
        //assert(location >= 0);
        if (uniform.type == INT) 
            glUniform1i(location, uniform.value.i);
        if (uniform.type == FLOAT1)
            glUniform1f(location, uniform.value.f[0]);
        else if (uniform.type == FLOAT2)
            glUniform2f(location, uniform.value.f[0], uniform.value.f[1]);
        else if (uniform.type == FLOAT3)
            glUniform3f(location, uniform.value.f[0], uniform.value.f[1], uniform.value.f[2]);
        else if (uniform.type == FLOAT4)
            glUniform4f(location, uniform.value.f[0], uniform.value.f[1], uniform.value.f[2], uniform.value.f[3]);
		
    }
}


bool Shader::hasUniform(const std::string &name) {
    glUseProgram(_programId);
    GLint uniform = glGetUniformLocation(_programId, name.c_str());
    glUseProgram(0);
    return uniform >= 0;
	
}

void Shader::setUniform(const std::string &name,unsigned int v1) {
  
  GLint location = glGetUniformLocation(_programId, name.c_str());
  glUniform1i(location,(int) v1);
}

void Shader::setUniform(const std::string &name, int v1) {
    Uniform uniform;
    uniform.type = INT;
    uniform.value.i = v1;
    _uniforms[name] = uniform;
}

void Shader::setUniformMatrix(const std::string &name, int count, float *v1) {
    GLint location = glGetUniformLocation(_programId, name.c_str());

    glUniformMatrix4fv(location,count,false,v1);
}

void Shader::setUniform(const std::string &name, int count, float *v1) {
	 GLint location = glGetUniformLocation(_programId, name.c_str());
	 glUniform3fv(location,count,v1);
}
void Shader::setUniformVec4Array(const std::string &name, int count, float *v1) {
	 GLint location = glGetUniformLocation(_programId, name.c_str());
	 glUniform4fv(location,count,v1);
}
void Shader::setUniformArray(const std::string &name, int count, float *v1) {
	 GLint location = glGetUniformLocation(_programId, name.c_str());
	 glUniform1fv(location,count,v1);
}
void Shader::setUniformArray(const std::string &name, int count, unsigned int *v1) {
	 GLint location = glGetUniformLocation(_programId, name.c_str());
	 glUniform1uiv(location,count,v1);
}
void Shader::setUniform(const std::string &name, float v1) {
    Uniform uniform;
    uniform.type = FLOAT1;
    uniform.value.f[0] = v1;
    _uniforms[name] = uniform;
    GLint location = glGetUniformLocation(_programId, name.c_str());
    glUniform1f(location, v1);
}

void Shader::setUniform(const std::string &name, float v1, float v2) {
    Uniform uniform;
    uniform.type = FLOAT2;
    uniform.value.f[0] = v1;
    uniform.value.f[1] = v2;
    _uniforms[name] = uniform;
 GLint location = glGetUniformLocation(_programId, name.c_str());
glUniform2f(location,v1,v2);
}

void Shader::setUniform(const std::string &name, float v1, float v2, float v3) {
    Uniform uniform;
    uniform.type = FLOAT3;
    uniform.value.f[0] = v1;
    uniform.value.f[1] = v2;
    uniform.value.f[2] = v3;
    _uniforms[name] = uniform;
    GLint location = glGetUniformLocation(_programId, name.c_str());
    glUniform3f(location,v1,v2,v3);
}

void Shader::setUniform(const std::string &name, float v1, float v2, float v3, float v4) {
    Uniform uniform;
    uniform.type = FLOAT4;
    uniform.value.f[0] = v1;
    uniform.value.f[1] = v2;
    uniform.value.f[2] = v3;
    uniform.value.f[3] = v4;
    _uniforms[name] = uniform;
    GLint location = glGetUniformLocation(_programId, name.c_str());
    glUniform4f(location,v1,v2,v3,v4);
}



}

// Settings for Vim (http://www.vim.org/), please do not remove:
// vim:softtabstop=2:shiftwidth=2:expandtab:textwidth=80:cindent
// vim:fdm=expr:foldexpr=getline(v\:lnum)=~'/\\*\\*'&&getline(v\:lnum)!~'\\*\\*/'?'a1'\:getline(v\:lnum)=~'\\*\\*/'&&getline(v\:lnum)!~'/\\*\\*'?'s1'\:'='