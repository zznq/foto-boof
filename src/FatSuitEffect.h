#ifndef FAT_SUIT_EFFECT_H
#define FAT_SUIT_EFFECT_H

#include "ofTypes.h"
#include "ofTexture.h"

#include "ShaderEffect.h"

class ofShader;

class FatSuitEffect: public ShaderEffect {
public:
	FatSuitEffect(int width, int height);
	virtual ~FatSuitEffect();
	virtual void addUI(CanvasPtr canvas);
	virtual void preDraw();
protected:
	virtual void guiEvent(ofxUIEventArgs &e);
protected:
	int m_width;
	int m_height;
	bool m_isDirty;
	float m_chubFactor;
};

#endif