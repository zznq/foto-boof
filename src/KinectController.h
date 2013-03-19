#ifndef KINECT_CONTROLLER_H
#define KINECT_CONTROLLER_H

#include <vector>

#include "ofTypes.h"
#include "ofTexture.h"

#include "KinectData.h"

class ofxKinect;
class VisualEffect;

class KinectController {
public:
	// convenience typedefs
	typedef ofPtr<ofxKinect> KinectInterfacePtr;
public:
	KinectController(bool infrared = false, bool video = true, bool texture = false);
	~KinectController();

	void setup();
	void update();
	void draw();
	bool isConnected() const;
	bool isFrameNew() const;

	KinectData getKinectData();
	float getDataWidth() const;
	float getDataHeight() const;

private:
	KinectInterfacePtr m_kinectInterface;
};

#endif