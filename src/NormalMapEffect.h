#ifndef NORMAL_MAP_EFFECT_H
#define NORMAL_MAP_EFFECT_H

#include "ofTypes.h"
#include "ofTexture.h"

#include "ShaderEffect.h"

class ofShader;

class NormalMapEffect: public ShaderEffect {
public:
	NormalMapEffect(int width, int height);
	virtual ~NormalMapEffect();
	virtual void addUI(CanvasPtr canvas);
protected:
	virtual void guiEvent(ofxUIEventArgs &e);
protected:
	int m_width;
	int m_height;
};

#endif