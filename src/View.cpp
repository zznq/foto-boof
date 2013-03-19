#include "View.h"
#include "KinectController.h"
#include "ViewDelegate.h"

View::View(KinectControllerPtr kinectController, int width, int height, CanvasPtr canvas) 
: m_kinectController(kinectController),  m_width(width), m_height(height), m_canvas(canvas), m_timeInterval(5000), m_runningTime(0)
{
	m_texture.allocate(m_kinectController->getDataWidth(), m_kinectController->getDataHeight(), GL_RGB);
}

View::~View()
{
	m_texture.clear();
}

void View::setup()
{
	VisualEffects::iterator iter;
	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->addUI(m_canvas);
	}
}

void View::close()
{
	VisualEffects::iterator iter;
	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->removeUI(m_canvas);
	}
}

void View::update(float delta)
{
	m_runningTime += delta;

	if(m_runningTime > this->getViewInterval()) {
		// Tell Delegate to update 
		this->viewComplete();
	}
}

void View::draw()
{
	if(!m_kinectController->isFrameNew())
	{
		return;
	}

	VisualEffects::iterator iter;
	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->preDraw();
	}

	// apply each effect to the kinect data
	// NOTE: this will modify the data in place, not copy
	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->draw();
	}

	m_texture.loadData(getKinectData().m_videoStream);
	m_texture.draw(0, 0, m_width, m_height);

	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->postDraw();
	}
}

void View::addEffect(const VisualEffectPtr& effect) 
{
	effect->setParent(this);
	m_effects.push_back(effect);
}

void View::clearEffects() 
{
	m_effects.clear();
}

void View::removeEffect(const std::string& effectName)
{
	for (VisualEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter)
	{
		if ((*iter)->getName() == effectName)
		{
			m_effects.erase(iter);
			break;
		}
	}
}

int View::getViewInterval()
{
	return m_timeInterval;
}

KinectData View::getKinectData() const
{
	return m_kinectController->getKinectData();
}

void View::setViewDelegate(View::ViewDelegatePtr delegate) 
{
	m_delegate = delegate;
}

void View::viewComplete()
{
	m_runningTime = 0;

	if(this->m_delegate)
	{
		this->m_delegate->viewComplete();
	}
}

void View::viewStart()
{
	if(this->m_delegate)
	{
		this->m_delegate->viewStart();
	}
}

void View::viewCountdownStarted()
{
	if(this->m_delegate)
	{
		this->m_delegate->viewCountdownStarted();
	}
}