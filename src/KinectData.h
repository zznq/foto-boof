#ifndef KINECT_DATA_H
#define KINECT_DATA_H

#include "ofPixels.h"

struct KinectData {
public:
	KinectData(ofPixels& videoStream, ofPixels& depthStream)
	: m_videoStream(videoStream), m_depthStream(depthStream)
	{

	}

	ofPixels& m_videoStream;
	ofPixels& m_depthStream;
};

#endif