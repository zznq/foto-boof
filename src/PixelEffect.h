#ifndef PIXEL_EFFECT_H
#define PIXEL_EFFECT_H

#include "ofPixels.h"

enum KinectCaptureStream;

// serves as a base class for all effects
class PixelEffect {
public:
	PixelEffect(const std::string& name, KinectCaptureStream captureStreamType) : m_name(name), m_captureStreamType(captureStreamType) { }
	const std::string& getName() { return m_name; }

	virtual void apply(ofPixels& pixels) = 0;

	KinectCaptureStream m_captureStreamType;

private:
	std::string m_name;
};

#endif