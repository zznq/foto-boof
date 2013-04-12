#ifndef POINT_CLOUD_EFFECT_H
#define POINT_CLOUD_EFFECT_H

#include "ofTexture.h"

#include "ShaderEffect.h"

class ofVboMesh;

class PointCloudEffect: public ShaderEffect {
public:
	PointCloudEffect(int width, int height);
	~PointCloudEffect();

	virtual void draw();

	virtual void addUI(CanvasPtr canvas);

	virtual void guiEvent(ofxUIEventArgs &e);

protected:
	void createMesh();

protected:
	int m_width;
	int m_height;
	ofTexture m_colorTexture;
	ofPtr<ofVboMesh> m_mesh;
	float m_numRows;
	float m_numCols;
	bool m_meshDirty;
	float m_pointSize;
};

#endif