#ifndef BOOF_OVERLAYVIEW
#define BOOF_OVERLAYVIEW

#include "View.h"
#include "KinectController.h"

// Use this class to display the color stream without timer
class OverlayView : public View {
private:
	ofTexture m_screen;

	bool m_isFlashing;
	float m_flashDuration;
	float m_flashRunningTime;

	bool m_isTransitioning;
	float m_transDuration;
	float m_transRunningTime;

	bool m_isCountingDown;
	float m_countDownDuration;
	float m_countDownRunningTime;
private:
	void transitionFinishedFired();
	void flashFinishedFired();
public:
	OverlayView(KinectControllerPtr kinectController, int width, int height);

	virtual void update(float delta);
	virtual void draw();

	void startFlash();
	void stopFlash();

	void startEffectTransition();
	void stopEffectTransition();

	void startTimer(float duration);
	void stopTimer();
};

#endif