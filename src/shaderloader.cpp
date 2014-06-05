/* 
* \file shaderloader.cpp
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