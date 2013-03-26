#ifndef KINECT_DATA_H
#define KINECT_DATA_H

#include "ofPixels.h"

struct KinectData {
public:
	KinectData(ofPixels& videoStream, ofPixels& depthStream, ofFloatPixels& depthValues)
		: m_videoStream(videoStream), m_depthStream(depthStream), m_depthValues(depthValues)
	{

	}

	ofFloatPixels& m_depthValues;
	ofPixels& m_videoStream;
	ofPixels& m_depthStream;
};

#endif