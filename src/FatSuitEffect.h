#ifndef FATSUITEFFECT_H
#define FATSUITEFFECT_H

#include "ofTypes.h"
#include "ofTexture.h"
#include "ofFbo.h"
#include "VisualEffect.h"

class ofShader;
class ofVboMesh;

class FatSuitEffect: public VisualEffect {
public:
	FatSuitEffect(int width, int height, bool wireframe = false, bool useNormalColors = false);
	virtual ~FatSuitEffect();
	virtual void addUI(CanvasPtr canvas);
	virtual void preDraw();
	virtual void draw();
	virtual void postDraw();
protected:
	virtual void guiEvent(ofxUIEventArgs &e);
	void createMesh();
	void setupShader();
protected:
	int m_width;
	int m_height;
	bool m_isDirty;
	float m_chubFactor;
	ofTexture m_colorTex;
	ofTexture m_displacementTex;
	ofTexture m_depthTex;
	ofPtr<ofShader> m_shader;
	ofPtr<ofShader> m_normalShader;
	ofPtr<ofShader> m_blurShader;
	ofPtr<ofShader> m_depthShader;
	ofPtr<ofVboMesh> m_mesh;
	ofFbo m_fbo;
	ofFbo m_blurHorizontal;
	ofFbo m_blurVertical;
	float m_nearDepth;
	float m_farDepth;
	bool m_shaderSetup;
	bool m_drawWireframe;
	ofLight light;
	float m_clipValue;
	float m_blurFactor;
	float m_cullingValue;
	float m_transX;
	float m_transY;
	float m_transZ;
	float m_meshStep;
	bool m_useNormalColor;
};

#endif