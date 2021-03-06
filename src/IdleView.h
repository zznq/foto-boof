#ifndef BOOF_IDLEVIEW
#define BOOF_IDLEVIEW

#include "View.h"
#include "KinectController.h"

#include "ofxOpenCv.h"

// Use this class to display the color stream without timer
class IdleView : public View {
public:
	typedef std::vector<bool> HaarReadResults;
public:
	IdleView(KinectControllerPtr kinectController, int width, int height);

	virtual void setup();
	virtual void update(float delta);

	virtual void doViewDraw();

	void startActionFired();

private:
	int m_haarReadMax;
	int m_haarReadResultsIndex;
	int m_k;
	int m_threshold;

	ofImage m_handbox;
	ofRectangle m_handboxRect;
	ofxCvGrayscaleImage m_grayscale;

	ofxCvHaarFinder finder;
	HaarReadResults m_haarReadResults;
};

#endif