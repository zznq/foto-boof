#ifndef TEST_EFFECT_H
#define TEST_EFFECT_H

#include "ofTypes.h"
#include "ofTexture.h"
#include "ofFbo.h"
#include "VisualEffect.h"

class ofShader;
class ofVboMesh;

class TestEffect: public VisualEffect {
public:
	TestEffect(int width, int height);
	virtual ~TestEffect();
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
	ofPtr<ofShader> m_shader;
	ofPtr<ofShader> m_normalShader;
	ofPtr<ofShader> m_blurShader;
	ofPtr<ofShader> m_normalDrawShader;
	ofPtr<ofVboMesh> m_mesh;
	ofFbo m_fbo;
	ofFbo m_blurHorizontal;
	ofFbo m_blurVertical;
	float m_nearDepth;
	float m_farDepth;
	bool m_shaderSetup;
	bool m_drawWireframe;
	ofLight light;
};

#endif