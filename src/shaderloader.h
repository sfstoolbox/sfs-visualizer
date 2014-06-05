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

#ifndef SHADERLOADER
#define SHADERLOADER

#include "stdincl.h"
#include "shader.h"

namespace SoundFieldViewer
{
class ShaderLoader
{
public:
	ShaderLoader(void);
	~ShaderLoader(void);
	void AddShader(std::string strFile,ShaderType type);
	void Load();

private:
	std::vector<Shader> m_lShader;
};
}
#endif