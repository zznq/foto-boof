#ifndef CLOUD_EFFECT_H
#define CLOUD_EFFECT_H

#include "ofTexture.h"

#include "VisualEffect.h"

class ofVboMesh;

class CloudTest: public VisualEffect {
public:
	CloudTest(int width, int height);

	virtual void draw();

	//virtual void addUI(CanvasPtr canvas);

	//virtual void guiEvent(ofxUIEventArgs &e);

protected:
	//void createMesh();

protected:
	int m_width;
	int m_height;
	ofTexture m_colorTexture;
	ofPtr<ofVboMesh> m_mesh;
	float m_numRows;
	float m_numCols;
	bool m_meshDirty;
	float m_pointSize;
	float m_nearDepth;
	float m_farDepth;
};

#endif