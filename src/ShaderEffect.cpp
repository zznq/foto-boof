#include "ofShader.h"

#include "ShaderEffect.h"

ShaderEffect::ShaderEffect(const std::string effectName, 
						   const std::string& vertexShader, 
						   const std::string& fragmentShader, 
						   const std::string& geometryShader)
: VisualEffect(effectName), m_shader(new ofShader())
{
	// load our shader files
	// shaders are located in data/shaders
	m_shader->load(vertexShader, fragmentShader, geometryShader);
}

ShaderEffect::~ShaderEffect()
{
}

void ShaderEffect::preDraw() 
{
	m_shader->begin();
}

void ShaderEffect::postDraw() 
{
	m_shader->end();
}

ofPtr<ofShader>& ShaderEffect::getShader()
{
	return m_shader;
}