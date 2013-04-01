#ifndef KINECT_DATA_H
#define KINECT_DATA_H

#include "ofPixels.h"

struct KinectStreamType {
	enum Enum {
		VideoStream = 0,
		DepthStream,
		DistanceStream,

		Max,
		Invalid = -1
	};
};
struct KinectData {
public:
	KinectData(ofPixels& videoStream, ofPixels& depthStream, ofFloatPixels& distanceValues)
		: m_videoStream(videoStream), m_depthStream(depthStream), m_distanceValues(distanceValues)
	{

	}

	KinectData& operator=(const KinectData& rhs) {
		m_videoStream = rhs.m_videoStream;
		m_depthStream = rhs.m_depthStream;
		m_distanceValues = rhs.m_distanceValues;

		return *this;
	}

	ofPixels& m_videoStream;
	ofPixels& m_depthStream;
	ofFloatPixels& m_distanceValues;
};

#endif