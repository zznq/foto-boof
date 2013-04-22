#include "ofxKinect.h"

#include "KinectController.h"
#include "VisualEffect.h"

KinectController::KinectController(bool infrared, bool video, bool texture) 
: m_newFrame(false),
  m_kinectInterface(new ofxKinect()), 
  m_kinectData(m_kinectInterface->getPixelsRef(), m_kinectInterface->getDepthPixelsRef(), m_kinectInterface->getRawDepthPixelsRef())
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

	// store whether the frame is new here since calling isFrameNew() on the ofxKinect object
	// causes it to reset its state meaning we cannot check it more than once per update. :(
	m_newFrame = m_kinectInterface->isFrameNew();

	if (isFrameNew())
	{
		// update local copy of kinect data on a new frame
		m_kinectData = KinectData(m_kinectInterface->getPixelsRef(), m_kinectInterface->getDepthPixelsRef(), m_kinectInterface->getRawDepthPixelsRef());
	}
}

bool KinectController::isConnected() const 
{
	return m_kinectInterface ? m_kinectInterface->isConnected() : false;
}

bool KinectController::isFrameNew() const
{
	return m_newFrame;
}

const KinectController::KinectInterfacePtr& KinectController::getKinect() const
{
	return m_kinectInterface;
}

KinectData KinectController::getKinectData() {
	return m_kinectData;
}

float KinectController::getDataWidth() const {
	return m_kinectInterface->getWidth();
}

float KinectController::getDataHeight() const {
	return m_kinectInterface->getHeight();
}

void KinectController::setDepthClipping(float nearClip, float farClip) {
	
	m_kinectInterface->setDepthClipping(nearClip, farClip);
}

float KinectController::getNearClipping() {
	return m_kinectInterface->getNearClipping();
}

float KinectController::getFarClipping() {
	return m_kinectInterface->getFarClipping();
}