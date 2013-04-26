#ifndef BOOF_ELEMENT
#define BOOF_ELEMENT

#include "ofTypes.h"

class ofShader;

class Element
{
protected:
	bool m_isTransitioning;
	bool m_isEnding;
	float m_transitionLapse;
	float m_lapse;

	ofPtr<ofShader> m_shader;
public:
	Element(float transitionLapse);
	Element(float transitionLapse, 
			const std::string& vertexShader, 
			const std::string& fragmentShader, 
			const std::string& geometryShader);
	~Element();

	virtual void begin();
	virtual void end();
	bool isFinished() const;
	// Return a value between 0 and 1 of progress of transition
	float progress() const;

	virtual void update(float delta);
	virtual void draw(int x, int y);

	virtual void preDraw();
	virtual void onDraw();
	virtual void postDraw();

	ofPtr<ofShader>& getShader();
};

#endif