#ifndef FAT_SUIT_EFFECT_H
#define FAT_SUIT_EFFECT_H

#include "ofTypes.h"
#include "ofTexture.h"

#include "ShaderEffect.h"

class ofShader;
class ofVboMesh;

struct Vertex
{
	float x, y, z;
};

struct ColorValue
{
	ColorValue(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) { }
	float r, g, b, a;
};

struct TexCoord
{
	TexCoord(float _s, float _t) : s(_s), t(_t) { }
	float s, t;
};

class FatSuitEffect: public ShaderEffect {
public:
	FatSuitEffect(int width, int height);
	virtual ~FatSuitEffect();
	virtual void addUI(CanvasPtr canvas);
	virtual void preDraw();
	virtual void draw();
protected:
	virtual void guiEvent(ofxUIEventArgs &e);
	void createMesh();
protected:
	int m_width;
	int m_height;
	bool m_isDirty;
	float m_chubFactor;
	ofPtr<ofVboMesh> m_mesh;
	float m_nearDepth;
	float m_farDepth;

	std::vector<Vertex> m_vertices;
	std::vector<ColorValue> m_colors;
	std::vector<TexCoord> m_texCoords;
	std::vector<ofIndexType> m_indices;
	GLuint m_vboId;
	GLuint m_colorId;
	GLuint m_texCoordId;
	GLuint m_iboId;
};

#endif