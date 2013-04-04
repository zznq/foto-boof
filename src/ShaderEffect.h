#ifndef SHADER_EFFECT_H
#define SHADER_EFFECT_H

#include "ofTypes.h"

#include "VisualEffect.h"

class ofShader;
class KinectData;

// converts to black & white using a shader
class ShaderEffect: public VisualEffect {
public:
	ShaderEffect(const std::string effectName, 
		const std::string& vertexShader="", 
		const std::string& fragmentShader="", 
		const std::string& geometryShader="");
	virtual ~ShaderEffect();
	virtual void preDraw();
	virtual void postDraw();

	ofPtr<ofShader>& getShader();

protected:
	ofPtr<ofShader> m_shader;
};

#endif