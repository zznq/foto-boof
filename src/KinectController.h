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

	const KinectInterfacePtr& getKinect() const;

	KinectData getKinectData();
	float getDataWidth() const;
	float getDataHeight() const;

	void setDepthClipping(float nearClip, float farClip);
private:
	bool m_newFrame;
	KinectInterfacePtr m_kinectInterface;
	KinectData m_kinectData;
};

#endif