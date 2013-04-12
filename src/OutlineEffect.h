#ifndef OUTLINE_EFFECT_H
#define OUTLINE_EFFECT_H

#include "VisualEffect.h"

class ofxCvContourFinder;

class OutlineEffect: public VisualEffect {
public:
	OutlineEffect(int width, int height);
	~OutlineEffect();

	virtual void draw();

	virtual void addUI(CanvasPtr canvas);

	virtual void guiEvent(ofxUIEventArgs &e);

protected:
	int m_width;
	int m_height;
	ofPtr<ofxCvContourFinder> m_contourFinder;
	float m_nearClip;
	float m_farClip;
	float m_smoothingWindow;
	float m_smoothingShape;
};

#endif