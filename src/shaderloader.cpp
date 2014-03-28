#include "shaderloader.h"
#include "stdincl.h"
#include "shader.h"

namespace SoundFieldViewer
{

ShaderLoader::ShaderLoader(void)
{
}


ShaderLoader::~ShaderLoader(void)
{
}


void ShaderLoader::AddShader(std::string strFile,ShaderType type)
{
	m_lShader.push_back(Shader(strFile,type));
}

//Got this from http://www.lighthouse3d.com/opengl/glsl/index.php?oglinfo
	// it prints out shader info (debugging!)
	

GLuint p;
void ShaderLoader::Load()
{

	p = glCreateProgram();
	for (std::vector<Shader>::iterator it=m_lShader.begin();it!=m_lShader.end();++it)
	{
		glAttachShader(p,(*it).v);
	}	
		glProgramParameteriEXT(p,GL_GEOMETRY_INPUT_TYPE_EXT,GL_LINES);
		glProgramParameteriEXT(p,GL_GEOMETRY_OUTPUT_TYPE_EXT,GL_LINE_STRIP);

		int temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
		glProgramParameteriEXT(p,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

	
		glLinkProgram(p);
		glUseProgram(p);

		
}

}