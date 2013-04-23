#include "OverlayView.h"
#include "ViewController.h"

OverlayView::OverlayView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, false)
{
	this->setOffset(0.0, 0.0, 0.0);

	m_canvas->setColorBack(ofColor(0.0f, 0.0F, 0.0F, 175.0f));
	
	m_screen.allocate(933, 700, OF_IMAGE_COLOR_ALPHA);

	m_background = ofImage("background.png");
	m_background.allocate(width, height, OF_IMAGE_COLOR);

	m_transition = ofImage("transition.png");
	m_transition.allocate(933, 700, OF_IMAGE_COLOR);

	m_counter_background = ofImage("counter_background.png");
	m_counter_background.allocate(24, 430, OF_IMAGE_COLOR_ALPHA);
	
	m_counter_top_cap = ofImage("counter_top_cap.png");
	m_counter_top_cap.allocate(18, 3, OF_IMAGE_COLOR_ALPHA);

	m_counter_bottom_cap = ofImage("counter_bottom_cap.png");
	m_counter_bottom_cap.allocate(18, 3, OF_IMAGE_COLOR_ALPHA);

	m_counter_fill = ofImage("counter_fill.png");
	m_counter_fill.allocate(18, 1, OF_IMAGE_COLOR_ALPHA);

	m_counter_label = ofImage("counter_label.png");
	m_counter_label.allocate(84, 7, OF_IMAGE_COLOR_ALPHA);

	m_indicatorActive = ofImage("indicator_active.png");
	m_indicatorActive.allocate(22, 22, OF_IMAGE_COLOR_ALPHA);

	m_indicatorInActive = ofImage("indicator_inactive.png");
	m_indicatorInActive.allocate(22, 22, OF_IMAGE_COLOR_ALPHA);

	m_indicators = std::vector<OverlayIndicator>(4);
	m_indicatorIndex = 0;
	m_indicators[0] = OverlayIndicator();
	m_indicators[1] = OverlayIndicator(IndicatorState::InActive, ofVec2f(30, 0));
	m_indicators[2] = OverlayIndicator(IndicatorState::InActive, ofVec2f(30, 0));
	m_indicators[3] = OverlayIndicator(IndicatorState::InActive, ofVec2f(30, 0));

	m_isTransitioning = false;
	m_transDuration = 3000.f; 
	m_transRunningTime = 0;
	m_transProgress = 0;

	m_isCountingDown = false;
	m_countDownDuration = 0;
	m_countDownRunningTime = 0;

	m_isFlashing = false;
	m_flashDuration = 1000.f;
	m_flashRunningTime = 0;

	m_shader = ofPtr<ofShader>(new ofShader());
	m_shader->load("shaders/default.vert", "shaders/indicator_blend.frag", "");

	m_fader = ofPtr<ofShader>(new ofShader());
	m_fader->load("shaders/default.vert", "shaders/fader.frag", "");

	
	m_countDownTimerThresehold = 3;
	m_countDownColor = ofColor(2,166,79);
	m_countDownColorSecondary = ofColor(144,15,15);
	m_codeFont.loadFont("CODE Bold.otf", 50);
}

void OverlayView::update(float delta)
{
	/*
	m_canvas->setVisible(true);

	ofxUIMinimalSlider *slider = new ofxUIMinimalSlider("Mini", 0, 10, 5, 25, 100);
	m_canvas->addWidget(slider);
	slider->setShowValue(true);
	*/

	View::update(delta);

	if(m_isTransitioning)
	{
		m_transRunningTime += delta;
		m_transProgress = m_transRunningTime / m_transDuration;
		if(m_transRunningTime >= m_transDuration)
		{
			m_isTransitioning = false;
			m_transRunningTime = 0;

			transitionFinishedFired();
		} else if(m_transProgress >= 0.5) {
			transitionHalfWayFired();
		}
	}

	if(m_isCountingDown) {
		m_countDownRunningTime += delta;
	}

	if(m_isFlashing) {
		m_flashRunningTime += delta;

		if(m_flashRunningTime >= m_flashDuration) {
			//m_isFlashing = false;
			m_flashRunningTime = 0;

			flashFinishedFired();
		}
	}
}

void OverlayView::doViewDraw()
{
	m_background.draw(0, 0, this->getWidth(), this->getHeight());
}

void OverlayView::drawForeground()
{
	if(m_isCountingDown)
	{
		drawCountdown();
	}

	if(m_isFlashing)
	{
		drawFlash();
	}

	if(m_isTransitioning)
	{
		drawTransition();
	}

	ofPushMatrix();

	ofTranslate(115, 608, 0);
	for (std::vector<OverlayIndicator>::iterator iter = m_indicators.begin(); iter != m_indicators.end(); ++iter) {
		ofTranslate((iter)->offset);
		drawIndicator((iter)->state);
	}

	ofPopMatrix();
}

void OverlayView::drawFlash()
{
	ofPushMatrix();
	ofTranslate(338, 10);
	m_screen.draw(0, 0);
	ofPopMatrix();
}

void OverlayView::drawTransition()
{
	ofPushMatrix();
	m_fader->begin();
	
	m_fader->setUniformTexture("tex", m_transition, 1);

	float start = 0.0f;
	float stop = 1.f;

	if(m_transProgress > 0.5f) {
		start = 1.f;
		stop = 0.0f;
		m_transProgress = m_transProgress - 0.5;
	}
	
	m_fader->setUniform1f("fade_value", ofLerp(start, stop, m_transProgress * 2));

	m_transition.draw(338.0, 10.0);

	m_fader->end();

	ofPopMatrix();
}

void OverlayView::drawCountdown() {
	
	int timeLeftMS = (m_countDownDuration - m_countDownRunningTime);
	int timeLeft = timeLeftMS / 1000;

	float countdown_height_max = 420;
	
	float countdown_height = m_countDownDuration == 0 ? 0 : countdown_height_max * (timeLeftMS / m_countDownDuration);

	countdown_height = ofClamp(countdown_height, 0, countdown_height_max);
	
	ofPushMatrix();

	m_counter_background.draw(115, 155);
	m_counter_label.draw(153, 512);

	ofTranslate(118, 155, 0);

	if(timeLeft <= m_countDownTimerThresehold) {
		ofSetColor(m_countDownColorSecondary);
	} else {
		ofSetColor(m_countDownColor);
	}

	m_counter_top_cap.draw(0, countdown_height_max - countdown_height);
	m_counter_fill.draw(0, countdown_height_max - countdown_height + 3, 18, countdown_height);
	m_counter_bottom_cap.draw(0, countdown_height_max + 3);

	ofTranslate(31, 426, 0);

	ostringstream convert;
	convert << timeLeft;

	m_codeFont.drawString(":", 0, 0);
	m_codeFont.drawString(convert.str(), 22, 0);

	ofSetColor(255,255,255);

	ofPopMatrix();
}

void OverlayView::drawIndicator(IndicatorState indicatorState) {

	switch(indicatorState) {
	case Active:
		m_indicatorActive.draw(0, 0, 22, 22);
		break;
	case InActive:
		m_indicatorInActive.draw(0, 0, 22, 22);
		break;
	case TowardsActive:
	case TowardsInActive:
		float start = 0;
		float end = 1;

		if(indicatorState == IndicatorState::TowardsInActive) {
			start = 1;
			end = 0;
		}

		m_shader->begin();
	
		m_shader->setUniformTexture("active", m_indicatorActive, 1);
		m_shader->setUniformTexture("inactive", m_indicatorInActive, 2);

		m_shader->setUniform1f("blend_level", ofLerp(start, end, m_transRunningTime / m_transDuration));

		m_indicatorActive.draw(0, 0, 22, 22);
		m_indicatorInActive.draw(0, 0, 22, 22);

		m_shader->end();

		break;
	}
}

void OverlayView::flashFinishedFired()
{
	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::FLASH_FINISHED);
	}
}

void OverlayView::transitionHalfWayFired()
{
	m_isFlashing = false;

	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::TRANSITION_HALF_WAY);
	}
}

void OverlayView::transitionFinishedFired()
{
	m_indicators[m_indicatorIndex].state = Active;
	m_indicatorIndex = ++m_indicatorIndex % m_indicators.size();

	if(m_indicatorIndex == 0) {
		for (std::vector<OverlayIndicator>::iterator iter = m_indicators.begin(); iter != m_indicators.end(); ++iter) {
			(iter)->state = OverlayView::IndicatorState::InActive;
		}
	}

	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::TRANSITION_FINISHED);
	}
}

void OverlayView::startEffectTransition()
{
	if(m_indicatorIndex == (m_indicators.size() - 1)) {
		restart();
	} else {
		m_indicators[m_indicatorIndex].state = TowardsActive;
	}

	m_isTransitioning = true;
}

void OverlayView::stopEffectTransition()
{
	m_screen.clear();
}

void OverlayView::startTimer(float duration)
{
	m_isCountingDown = true;
	m_countDownDuration = duration;
}

void OverlayView::stopTimer()
{
	m_isCountingDown = false;
	m_countDownDuration = 0;
	m_countDownRunningTime = 0;
}

void OverlayView::startFlash()
{
	m_isFlashing = true;
	m_flashRunningTime = 0;

	m_screen.grabScreen(338, 10, 933, 700);
}

void OverlayView::stopFlash()
{
	//m_isFlashing = false;
	m_flashRunningTime = 0;
}

void OverlayView::restart()
{
	for (std::vector<OverlayIndicator>::iterator iter = m_indicators.begin(); iter != m_indicators.end(); ++iter) {
		(iter)->state = OverlayView::IndicatorState::TowardsInActive;
	}
}

ofImage OverlayView::getScreen()
{
	return m_screen;
}