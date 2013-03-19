#include "ofxKinect.h"

#include "KinectController.h"
#include "VisualEffect.h"

KinectController::KinectController(bool infrared, bool video, bool texture) 
: m_kinectInterface(new ofxKinect())
{
	// initialize the kinect interface
	m_kinectInterface->init(infrared, video, texture);

	// align depth with RGB image
	// NOTE: if this is not set a random crash is
	// observed inside ofxKinect where it is trying to
	// access a vector out of range. See issue here:
	// https://github.com/ofTheo/ofxKinect/issues/103#issuecomment-7780195
	m_kinectInterface->setRegistration(true);

	// open a connection to the first one found
	m_kinectInterface->open();
}

KinectController::~KinectController() 
{
	
}

void KinectController::setup() 
{

}

void KinectController::update() 
{
	m_kinectInterface->update();
}

bool KinectController::isConnected() const 
{
	return m_kinectInterface ? m_kinectInterface->isConnected() : false;
}

bool KinectController::isFrameNew() const
{
	return m_kinectInterface ? m_kinectInterface->isFrameNew() : false;
}

KinectData KinectController::getKinectData() {
	return KinectData(m_kinectInterface->getPixelsRef(), m_kinectInterface->getDepthPixelsRef());
}

float KinectController::getDataWidth() const {
	return m_kinectInterface->getWidth();
}

float KinectController::getDataHeight() const {
	return m_kinectInterface->getHeight();
}

