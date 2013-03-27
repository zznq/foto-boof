#ifndef COLOR_DEPTH_SHADER_EFFECT_H
#define COLOR_DEPTH_SHADER_EFFECT_H

#include "VisualEffect.h"
#include "ShaderEffect.h"

class ofShader;
class KinectData;

// converts to black & white using a shader
class ColorDepthShaderEffect : public ShaderEffect {
private:
	ofPixels m_lookup;
	ofTexture m_lookupTexture;
public:
	ColorDepthShaderEffect(const std::string effectName,
								const int width,
								const int height,
								const std::string& vertexShader="", 
								const std::string& fragmentShader="", 
								const std::string& geometryShader="");

	virtual ~ColorDepthShaderEffect();
	virtual void preDraw();
	virtual void postDraw();

	virtual void addUI(CanvasPtr canvas);
	virtual void guiEvent(ofxUIEventArgs &e);

private:
	float m_saturationValue;
	float m_brightnessValue;
	int m_lookupStepSize;
	int m_lookupOffset;

	float m_nearClip;
	float m_farClip;

	void buildLookUpTable();
};

#endif