#ifndef KINECT_CONTROLLER_H
#define KINECT_CONTROLLER_H

#include <vector>

#include "ofTypes.h"
#include "ofTexture.h"

class ofxKinect;
class PixelEffect;

class KinectController {
public:
	// convenience typedefs
	typedef ofPtr<PixelEffect> PixelEffectPtr;
	typedef std::vector<PixelEffectPtr> PixelEffects;
	typedef ofPtr<ofxKinect> KinectInterfacePtr;
public: 
	KinectController();
	~KinectController();

	void addEffect(const PixelEffectPtr& effect);
	void removeEffect(const std::string& effectName);
	void applyEffects();

	void setup();
	void update();
	void draw();
	bool isConnected() const;
private:
	PixelEffects m_effects;
	KinectInterfacePtr m_kinectInterface;
	ofTexture m_texture;
};

#endif