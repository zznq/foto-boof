#ifndef BOOF_COUNTDOWN_ELEMENT
#define BOOF_COUNTDOWN_ELEMENT

#include "Element.h"
#include "ofMain.h"

class CountDownElement : public Element
{
private:
	bool m_isCountingDown;
	int m_countDownTimerThresehold;
	float m_countDownDuration;
	float m_countDownRunningTime;

	ofImage m_counter_background;
	ofImage m_counter_top_cap;
	ofImage m_counter_bottom_cap;
	ofImage m_counter_fill;
	ofImage m_counter_label;

	ofColor m_countDownColor;
	ofColor m_countDownColorSecondary;
	ofTrueTypeFont m_codeFont;

	void initialize();
public:
	CountDownElement();
	CountDownElement(const std::string& vertexShader, 
							   const std::string& fragmentShader, 
							   const std::string& geometryShader);
	~CountDownElement();

	void startCountDown();
	void setDuration(float duration);

	virtual void update(float delta);

	virtual void preDraw();
	virtual void onDraw();
};

#endif