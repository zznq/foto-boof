#include "OverlayView.h"
#include "ViewController.h"

OverlayView::OverlayView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, false)
{
	m_canvas->setColorBack(ofColor(0.0f, 0.0F, 0.0F, 0.0f));

	m_screen.allocate(width, height, GL_RGBA);

	m_isTransitioning = false;
	m_transDuration = 1000; // 1 sec
	m_transRunningTime = 0;

	m_isCountingDown = false;
	m_countDownDuration = 0;
	m_countDownRunningTime = 0;

	m_isFlashing = false;
	m_flashDuration = 1000; // 1 sec
	m_flashRunningTime = 0;
}

void OverlayView::update(float delta)
{
	View::update(delta);

	if(m_isTransitioning)
	{
		m_transRunningTime += delta;

		if(m_transRunningTime >= m_transDuration)
		{
			m_isTransitioning = false;
			m_transRunningTime = 0;

			transitionFinishedFired();
		}
	}

	if(m_isCountingDown) {
		m_countDownRunningTime += delta;
	}

	if(m_isFlashing) {
		m_flashRunningTime += delta;

		if(m_flashRunningTime >= m_flashDuration) {
			m_isFlashing = false;
			m_flashRunningTime = 0;

			flashFinishedFired();
		}
	}
}

void OverlayView::draw()
{
	if(!m_kinectController->isFrameNew())
	{
		return;
	}


}

void OverlayView::flashFinishedFired()
{
	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(FLASH_FINISHED);
	}
}

void OverlayView::transitionFinishedFired()
{
	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(TRANSITION_FINISHED);
	}
}

void OverlayView::startEffectTransition()
{
	m_screen.loadScreenData(0, 0, m_width, m_height);

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
}

void OverlayView::stopFlash()
{
	m_isFlashing = false;
	m_flashRunningTime = 0;
}