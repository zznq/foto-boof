#include "OverlayView.h"
#include "ViewController.h"

OverlayView::OverlayView(KinectControllerPtr kinectController, int width, int height, int indicatorCount) : 
	View(kinectController, width, height, false),
	m_indicatorCount(indicatorCount), m_restartedIndicators(false), m_showIndicators(false)
{
	this->setOffset(0.0, 0.0, 0.0);

	m_shader = ofPtr<ofShader>(new ofShader());
	m_shader->load("shaders/default.vert", "shaders/indicator_blend.frag", "");

	m_fader = ofPtr<ofShader>(new ofShader());
	m_fader->load("shaders/default.vert", "shaders/fader.frag", "");

	m_canvas->setColorBack(ofColor(0.0f, 0.0F, 0.0F, 0.0f));
	
	m_screen.allocate(933, 700, OF_IMAGE_COLOR_ALPHA);

	m_background = ofPtr<BackgroundImageElement>(new BackgroundImageElement());
	m_background->begin();

	m_startSidebar = ofPtr<StartSideBarImageElement>(new StartSideBarImageElement("shaders/default.vert", "shaders/fader.frag", ""));
	m_startSidebar->begin();

	m_primarySidebar = ofPtr<PrimarySideBarImageElement>(new PrimarySideBarImageElement("shaders/default.vert", "shaders/fader.frag", ""));
	m_printing = ofPtr<PrintingImageElement>(new PrintingImageElement("shaders/default.vert", "shaders/fader.frag", ""));
	m_countDown = ofPtr<CountDownElement>(new CountDownElement("shaders/default.vert", "shaders/fader.frag", ""));

	elements = std::vector<ofPtr<Element>>();
	elements.push_back(m_background);
	elements.push_back(m_startSidebar);
	elements.push_back(m_primarySidebar);
	elements.push_back(m_printing);
	elements.push_back(m_countDown);

	m_transition = ofImage("graphics/transition.png");
	m_transition.allocate(933, 700, OF_IMAGE_COLOR);

	/**************************
	Indicator Images
	**************************/
	m_indicatorActive = ofImage("graphics/indicator_active.png");
	m_indicatorActive.allocate(22, 22, OF_IMAGE_COLOR_ALPHA);

	m_indicatorInActive = ofImage("graphics/indicator_inactive.png");
	m_indicatorInActive.allocate(22, 22, OF_IMAGE_COLOR_ALPHA);

	m_indicators = std::vector<OverlayIndicator>(m_indicatorCount -1);
	m_indicatorIndex = -1;
	for(int i = 0; i < m_indicatorCount; i ++)
	{
		// Represents Idle View
		if(i == 0)
		{
			continue;
		}
		ofVec2f offset(0, 0);
		if(i > 1) {
			offset.x = 30;
		}

		m_indicators[i -1] = OverlayIndicator(IndicatorState::InActive, offset);
	}

	m_isTransitioning = false;
	m_isTransitionHalfWay = false;
	m_showScreen = false;
	m_transDuration = 3000.f; 
	m_transRunningTime = 0;
	m_transProgress = 0;

	m_isFlashing = false;
	m_flashDuration = 1000.f;
	m_flashRunningTime = 0;

	m_isPrinting = false;
	m_printDuration = 45000;
	m_printLapse = 0;
}

void OverlayView::update(float delta)
{
	View::update(delta);

	if(m_isPrinting)
	{
		m_printLapse += delta;
		if(m_printLapse >= m_printDuration)
		{
			m_isPrinting = false;
			printFinishedFired();
		}
	}

	if(m_isTransitioning)
	{
		m_transRunningTime += delta;
		m_transProgress = ofClamp(m_transRunningTime / m_transDuration, 0, 1);

		if(m_transProgress >= 0.5)
		{
			if(!m_isTransitionHalfWay)
			{
				transitionHalfWayFired();
			}

			if(m_transProgress == 1)
			{
				m_isTransitioning = false;
				m_transRunningTime = 0;

				transitionFinishedFired();
			}
		}
	}

	if(m_isFlashing) {
		m_flashRunningTime += delta;

		if(m_flashRunningTime >= m_flashDuration) {
			m_isFlashing = false;
			m_flashRunningTime = 0;

			flashFinishedFired();
		}
	}

	for (std::vector<ofPtr<Element>>::iterator iter = elements.begin(); iter != elements.end(); ++iter)
	{
		(*iter)->update(delta);
	}
}

void OverlayView::doViewDraw()
{
	for (std::vector<ofPtr<Element>>::iterator iter = elements.begin(); iter != elements.end(); ++iter)
	{
		(*iter)->draw(0, 0);
	}
}

void OverlayView::drawForeground()
{
	if(m_showScreen)
	{
		drawScreen();
	}

	if(m_isTransitioning)
	{
		drawTransition();
	}

	if(m_showIndicators)
	{
		ofPushMatrix();

		ofTranslate(115, 608, 0);
		for (std::vector<OverlayIndicator>::iterator iter = m_indicators.begin(); iter != m_indicators.end(); ++iter) {
			ofTranslate((iter)->offset);
			drawIndicator((iter)->state);
		}

		ofPopMatrix();
	}
}

void OverlayView::drawScreen()
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
	
	m_fader->setUniform1f("progress_value", ofLerp(start, stop, m_transProgress * 2));

	m_transition.draw(338.0, 10.0);

	m_fader->end();

	ofPopMatrix();
}

void OverlayView::drawCountdown()
{
	
}

void OverlayView::drawIndicator(IndicatorState indicatorState)
{
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
	if(!m_isTransitionHalfWay)
	{
		m_isTransitionHalfWay = true;
		m_showScreen = false;

		if(this->m_delegate)
		{
			this->m_delegate->handleViewAction(ViewAction::TRANSITION_HALF_WAY);
		}
	}
}

void OverlayView::transitionFinishedFired()
{
	m_isTransitionHalfWay = false;
	
	if(m_indicatorIndex == -1)
	{
		setIndicatorsState(OverlayView::IndicatorState::InActive);
	}

	if(!m_restartedIndicators)
	{
		m_indicatorIndex = m_indicatorIndex++;

		m_indicators[m_indicatorIndex].state = Active;
	}
	else
	{
		m_restartedIndicators = false;
	}

	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::TRANSITION_FINISHED);
	}
}

void OverlayView::printFinishedFired()
{
	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::PRINT_FINISHED);
	}
}

void OverlayView::startEffectTransition()
{
	if(m_indicatorIndex == (m_indicators.size() - 1))
	{
		restart();
	}
	else
	{
		m_indicators[m_indicatorIndex + 1].state = TowardsActive;
	}

	m_isTransitioning = true;

	starting();
}

void OverlayView::stopEffectTransition()
{
	m_screen.clear();
}

void OverlayView::setTimer(float duration)
{
	m_countDown->setDuration(duration);
	//m_countDown->begin();
}

void OverlayView::startTimer()
{
	std::cout << "OverlayView Start Timer\n";
	m_countDown->startCountDown();
}

void OverlayView::stopTimer()
{
}

void OverlayView::startFlash()
{
	m_isFlashing = true;
	m_showScreen = true;
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
	m_indicatorIndex = -1;
	m_restartedIndicators = true;
	setIndicatorsState(OverlayView::IndicatorState::TowardsInActive);
}

void OverlayView::setIndicatorsState(OverlayView::IndicatorState state)
{
	for (std::vector<OverlayIndicator>::iterator iter = m_indicators.begin(); iter != m_indicators.end(); ++iter) {
		(iter)->state = state;
	}
}

ofImage OverlayView::getScreen()
{
	return m_screen;
}

void OverlayView::starting()
{
	m_startSidebar->end();
}

void OverlayView::started()
{
	m_showIndicators = true;
	m_primarySidebar->begin();
	m_countDown->begin();
}

void OverlayView::printing()
{
	m_isPrinting = true;
	m_showIndicators = false;
	m_countDown->end();
	m_primarySidebar->end();
	m_printing->begin();
}

void OverlayView::printed()
{
	m_printing->end();
	m_startSidebar->begin();
	m_printLapse = 0;
}