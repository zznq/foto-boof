#include "View.h"
#include "KinectController.h"
#include "ViewController.h"
#include "ViewDelegate.h"

const float View::DEFAULT_X_OFFSET = 338.0;
const float View::DEFAULT_Y_OFFSET = 10.0;
const float View::DEFAULT_Z_OFFSET = 0.0;

View::View(KinectControllerPtr kinectController, int width, int height, bool useDepth) 
: m_kinectController(kinectController),  m_width(width), m_height(height), m_timeInterval(15000), 
m_runningTime(0), m_useDepth(useDepth), m_countDownRunning(false), m_offset(ofVec3f(DEFAULT_X_OFFSET, DEFAULT_Y_OFFSET, DEFAULT_Z_OFFSET))
{
	m_canvas = View::CanvasPtr(new ofxUICanvas());
	m_canvas->setColorBack(ofColor(87.0f, 87.0F, 87.0F, 197.0f));
	m_canvas->setVisible(false);

	m_texture.allocate(m_kinectController->getDataWidth(), m_kinectController->getDataHeight(), GL_RGB);

	m_nearClipping = m_kinectController->getNearClipping();
	m_farClipping = m_kinectController->getFarClipping();
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

	m_kinectController->setDepthClipping(m_nearClipping, m_farClipping);
}

void View::startCountDown()
{
	m_runningTime = 0;
	m_countDownRunning = true;
}

void View::stopCountDown()
{
	m_runningTime = 0;
	m_countDownRunning = false;
}

void View::update(float delta)
{
	if(m_countDownRunning) {
		m_runningTime += delta;

		if(m_runningTime > this->getViewInterval()) {
			// Tell Delegate to update 
			this->viewComplete();
		}
	}
}

void View::draw()
{
	if(!m_kinectController->isFrameNew())
	{
		return;
	}

	ofPushMatrix();
	ofTranslate(m_offset.x, m_offset.y, m_offset.z);

	// effects pre-draw
	doEffectsPreDraw();

	// effects draw
	doEffectsDraw();

	// view draw
	doViewDraw();

	// effects post-draw
	doEffectsPostDraw();

	ofPopMatrix();
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

void View::doViewDraw() 
{
	ofPushMatrix();
	
	//Flip image and re-adjust
	ofScale(-1, 1, 1);
	ofTranslate(-this->getWidth(), 0, 0);

	m_texture.loadData(m_useDepth ? getKinectData().m_depthStream : getKinectData().m_videoStream);
	m_texture.draw(0, 0, this->getWidth(), this->getHeight());
	
	ofPopMatrix();
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

int View::getWidth() const
{
	return m_width;
}

int View::getHeight() const
{
	return m_height;
}

ofVec3f View::getOffset() const
{
	return m_offset;
}

void View::setOffset(const ofVec3f& offset)
{
	m_offset = offset;
}

void View::setOffset(const float x, const float y, const float z)
{
	m_offset.x = x;
	m_offset.y = y;
	m_offset.z = z;
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
	m_countDownRunning = false;
	m_runningTime = 0;

	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::EFFECT_COUNTDOWN_FINISHED);
	}
}