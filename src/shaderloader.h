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