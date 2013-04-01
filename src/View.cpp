#include "View.h"
#include "KinectController.h"
#include "ViewDelegate.h"

View::View(KinectControllerPtr kinectController, int width, int height) 
: m_kinectController(kinectController),  m_width(width), m_height(height), m_timeInterval(5000), m_runningTime(0)
{
	m_canvas = View::CanvasPtr(new ofxUICanvas());
	m_canvas->setColorBack(ofColor(87.0f, 87.0F, 87.0F, 197.0f));

	m_texture.allocate(m_kinectController->getDataWidth(), m_kinectController->getDataHeight(), GL_RGB);
}

View::View(KinectControllerPtr kinectController, int width, int height, bool useDepth) 
	: m_kinectController(kinectController),  m_width(width), m_height(height), m_timeInterval(5000), m_runningTime(0), m_useDepth(useDepth)
{
	m_canvas = View::CanvasPtr(new ofxUICanvas());
	m_canvas->setColorBack(ofColor(87.0f, 87.0F, 87.0F, 197.0f));

	m_texture.allocate(m_kinectController->getDataWidth(), m_kinectController->getDataHeight(), GL_RGB);
}

View::~View()
{
	m_texture.clear();
}

void View::setup()
{
	m_canvas->setVisible(true);
}

void View::close()
{
	m_canvas->setVisible(false);
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

	// effects pre-draw
	doEffectsPreDraw();

	// effects draw
	doEffectsDraw();

	// view draw
	doViewDraw();

	// effects post-draw
	doEffectsPostDraw();
}

void View::doViewDraw() 
{
	m_texture.loadData(m_useDepth ? getKinectData().m_depthStream : getKinectData().m_videoStream);
	m_texture.draw(0, 0);
}

void View::doEffectsPreDraw() 
{
	for (VisualEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->preDraw();
	}
}

void View::doEffectsDraw() 
{
	// apply each effect to the kinect data
	// NOTE: this will modify the data in place, not copy
	for (VisualEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->draw();
	}
}

void View::doEffectsPostDraw() 
{
	for (VisualEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
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

int View::getWidth() const {
	return m_width;
}

int View::getHeight() const {
	return m_height;
}

int View::getViewInterval()
{
	return m_timeInterval;
}

const KinectControllerPtr& View::getKinectController() const
{
	return m_kinectController;
}

KinectData View::getKinectData() const
{
	return m_kinectController->getKinectData();
}

void View::setKinectDepthClipping(float nearClip, float farClip) const
{
	m_kinectController->setDepthClipping(nearClip, farClip);
}

View::CanvasPtr View::getCanvas() const
{
	return m_canvas;
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