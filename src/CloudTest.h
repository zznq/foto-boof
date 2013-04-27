#ifndef CLOUD_EFFECT_H
#define CLOUD_EFFECT_H

#include "ofTexture.h"

#include "VisualEffect.h"

class ofVboMesh;

class CloudTest: public VisualEffect {
public:
	CloudTest(int width, int height);

	virtual void preDraw();
	virtual void draw();
	//virtual void addUI(CanvasPtr canvas);

	//virtual void guiEvent(ofxUIEventArgs &e);

protected:
	//void createMesh();
	void drawBoxes();
	void drawScene();

protected:
	ofLight light;
	ofShader dofShader;
	ofShader depthShader;
	ofFbo depthFbo;
	ofFbo colorFbo;
	int m_width;
	int m_height;
	float nearTh;
	float farTh;
	float leftTh;
	float rightTh;
	float topTh;
	float bottomTh;
	float step;
	float dof24_focalDepth;
	float dof24_focalLength;
	float dof24_fstop;
	float dof24_maxblur;
	float dof24_showFocus;
	float directionLightX;
	float directionLightY;
	float directionLightZ;
	float cubeSizeMin;
	float cubeSizeMax;
	float depth_Far;
	float depth_Near;
};

#endif