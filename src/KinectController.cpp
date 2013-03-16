#include "ofxKinect.h"

#include "KinectController.h"
#include "PixelEffect.h"

KinectController::KinectController()
:m_kinectInterface(new ofxKinect()) 
{
	// initialize the kinect interface
	m_kinectInterface->init(false, true, false);

	// open a connection to the first one found
	m_kinectInterface->open();
}

KinectController::~KinectController() 
{
	// must be called to properly
	// shutdown the interface
	m_kinectInterface->close();

	m_texture.clear();
}

void KinectController::addEffect(const PixelEffectPtr& effect) 
{
	m_effects.push_back(effect);
}

void KinectController::removeEffect(const std::string& effectName)
{
	for (PixelEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		if ((*iter)->getName() == effectName) {
			m_effects.erase(iter);
			break;
		}
	}
}

void KinectController::setup() 
{
	// NOTE: Must wait to do it here because when the constructor is run
	// the opengl context is not setup yet causing a bad texture
	// allocate our texture based on the width/height from the kinect
	m_texture.allocate(m_kinectInterface->getWidth(), m_kinectInterface->getHeight(), GL_RGB);
}

void KinectController::update() 
{
	m_kinectInterface->update();
	applyEffects();
}

void KinectController::applyEffects() 
{
	// store reference locally
	ofPixels& pixels = m_kinectInterface->getPixelsRef();

	// apply each effect to the pixels data from the kinect
	// NOTE: this will modify the pixel data in place, not copy
	for (PixelEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->apply(pixels);
	}
}

void KinectController::draw() 
{
	m_texture.loadData(m_kinectInterface->getPixelsRef());
	m_texture.draw(0, 0, m_kinectInterface->getWidth(), m_kinectInterface->getHeight());
}

bool KinectController::isConnected() const 
{
	return m_kinectInterface ? m_kinectInterface->isConnected() : false;
}
