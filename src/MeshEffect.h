#ifndef MESH_EFFECT_H
#define MESH_EFFECT_H

#include "ofTypes.h"

#include "VisualEffect.h"

class ofShader;
class KinectData;
class ofVboMesh;

// converts to black & white using a shader
class MeshEffect: public VisualEffect {
public:
	MeshEffect(int width, int height);
	virtual ~MeshEffect();
	virtual void draw();
	virtual void addUI(CanvasPtr canvas);
protected:
	void createMesh();
	virtual void guiEvent(ofxUIEventArgs &e);
protected:
	bool m_meshDirty;
	int m_width;
	int m_height;
	ofPtr<ofVboMesh> m_mesh;
	float m_numRows;
	float m_numCols;
	float m_thresholdZ;
};

#endif