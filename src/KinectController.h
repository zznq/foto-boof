#ifndef KINECT_CONTROLLER_H
#define KINECT_CONTROLLER_H

#include <vector>

#include "ofTypes.h"
#include "ofTexture.h"

class ofxKinect;
class PixelEffect;

enum KinectCaptureStream { 
	CaptureStream_RGB,
	CaptureStream_BW,
	CaptureStream_DEPTH
};

class KinectController {
public:
	// convenience typedefs
	typedef ofPtr<PixelEffect> PixelEffectPtr;
	typedef std::vector<PixelEffectPtr> PixelEffects;
	typedef ofPtr<ofxKinect> KinectInterfacePtr;
public:
	KinectController(bool infrared = false, bool video = true, bool texture = false);
	~KinectController();

	void addEffect(const PixelEffectPtr& effect);
	void removeEffect(const std::string& effectName);
	void clearEffects();
	void applyEffects();

	void setup();
	void update();
	void draw();
	bool isConnected() const;

	ofPixels & getPixelsRef();

	PixelEffects m_effects;
	KinectInterfacePtr m_kinectInterface;
	ofTexture m_texture;

	KinectCaptureStream m_captureStream;
};

#endif