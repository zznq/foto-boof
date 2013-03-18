#include "ofShader.h"

#include "BwShaderEffect.h"

BwShaderEffect::BwShaderEffect()
: VisualEffect("bw_shader_effect"), m_shader(new ofShader())
{
	// load our shader files
	// shaders are located in data/shaders
	m_shader->load("shaders/default.vert", "shaders/grayscale.frag");
}

BwShaderEffect::~BwShaderEffect()
{

}

void BwShaderEffect::preDraw() 
{
	m_shader->begin();
}

void BwShaderEffect::postDraw() 
{
	m_shader->end();
}