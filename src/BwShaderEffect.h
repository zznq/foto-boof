#ifndef BW_SHADER_EFFECT_H
#define BW_SHADER_EFFECT_H

#include "ofTypes.h"

#include "VisualEffect.h"

class ofShader;
class KinectData;

// converts to black & white using a shader
class BwShaderEffect: public VisualEffect {
public:
	BwShaderEffect();
	virtual ~BwShaderEffect();
	virtual void preDraw();
	virtual void postDraw();

private:
	ofPtr<ofShader> m_shader;
};

#endif