#ifndef MESH_TEST_EFFECT_H
#define MESH_TEST_EFFECT_H

#include "ofTexture.h"
#include "ofLight.h"
#include "ofVboMesh.h"

#include "VisualEffect.h"

class MeshTest: public VisualEffect {
public:
	MeshTest(int width, int height);

	virtual void draw();

	//virtual void addUI(CanvasPtr canvas);

	//virtual void guiEvent(ofxUIEventArgs &e);

protected:
	//void createMesh();

protected:
	int m_width;
	int m_height;
	ofTexture m_colorTexture;
	ofVboMesh mesh;
	ofLight light;
	float m_numRows;
	float m_numCols;
	ofEasyCam easyCam;
};

#endif