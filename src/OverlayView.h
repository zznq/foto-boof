#ifndef BOOF_OVERLAYVIEW
#define BOOF_OVERLAYVIEW

#include "View.h"
#include "KinectController.h"

struct OverlayIndicator;

// Use this class to display the color stream without timer
class OverlayView : public View {
public:
	enum IndicatorState
	{
		Active,
		InActive,
		TowardsActive,
		TowardsInActive
	};
private:
	bool m_isFlashing;
	float m_flashDuration;
	float m_flashRunningTime;

	bool m_isTransitioning;
	float m_transDuration;
	float m_transRunningTime;
	float m_transProgress;

	bool m_isCountingDown;
	int m_countDownTimerThresehold;
	float m_countDownDuration;
	float m_countDownRunningTime;

	ofImage m_screen;
	ofImage m_background;
	ofImage m_transition;

	ofImage m_counter_background;
	ofImage m_counter_top_cap;
	ofImage m_counter_bottom_cap;
	ofImage m_counter_fill;
	ofImage m_counter_label;
	
	ofImage m_indicatorActive;
	ofImage m_indicatorInActive;

	ofColor m_countDownColor;
	ofColor m_countDownColorSecondary;
	ofTrueTypeFont m_codeFont;

	int m_indicatorIndex;
	std::vector<OverlayIndicator> m_indicators;

	void transitionHalfWayFired();
	void transitionFinishedFired();
	void flashFinishedFired();

	void drawCountdown();
	void drawIndicator(IndicatorState indicatorState);
	void drawFlash();
	void drawTransition();

protected:
	ofPtr<ofShader> m_shader;
	ofPtr<ofShader> m_fader;
public:
	OverlayView(KinectControllerPtr kinectController, int width, int height);

	virtual void update(float delta);
	virtual void doViewDraw();

	void drawForeground();

	void startFlash();
	void stopFlash();

	void startEffectTransition();
	void stopEffectTransition();

	void startTimer(float duration);
	void stopTimer();

	void restart();

	ofImage getScreen();
};

struct OverlayIndicator {
	OverlayView::IndicatorState state;
	ofVec2f offset;
	
	OverlayIndicator() : state(OverlayView::IndicatorState::InActive), offset(ofVec2f(0, 0)) { }
	OverlayIndicator(OverlayView::IndicatorState _state, ofVec2f _offset) : state(_state), offset(_offset) { }
};
#endif