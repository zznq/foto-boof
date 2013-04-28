#include "IdleView.h"
#include "ViewController.h"


//TODO: Adjust m_handbox to accomidate for larger display
IdleView::IdleView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true), 
	m_threshold(50), m_haarReadMax(10), m_k(1), m_haarReadResultsIndex(0)
{
	m_canvas->setColorBack(ofColor(0.0f, 0.0F, 0.0F, 0.0f));

	m_grayscale.allocate(width, height);

	finder.setup("haarcascade_hand.xml");
	finder.setScaleHaar(1.3);

	m_haarReadResults = std::vector<bool>(m_haarReadMax);

	m_handbox = ofImage("graphics/start_box.png");
	m_handbox.allocate(270, 270, ofImageType::OF_IMAGE_COLOR_ALPHA);

	// since we are displaying the grid x1.5 larger then the kinect data stream is the handboxRect should be a little smaller then displayed graphic
	m_handboxRect = ofRectangle(0, 0, 270, 270);
}

void IdleView::setup() {
	View::setup();

	m_canvas->setVisible(false);

	this->setKinectDepthClipping(500, 1500);
}

void IdleView::update(float delta) {
	View::update(delta);

	m_grayscale.setFromPixels(getKinectData().m_depthStream);
	
	// make our fingers a little skinnier so the haar cascade works better
	m_grayscale.erode();

	// Improves performance a ton by specifiying a ROI Rect
	finder.findHaarObjects(m_grayscale, m_handboxRect);

	int haarFoundCount = 0;

	for (HaarReadResults::iterator iter = m_haarReadResults.begin(); iter != m_haarReadResults.end(); ++iter) {
		if((* iter)) {
			haarFoundCount++;
		}
	}

	// Laplace smoothing
	// Sometimes we get a false negative so we take an average of all the results and smooth out the result 
	// This means that if we don't have a high number of true's then this won't pass
	float ls = (haarFoundCount + m_k) / (float)(m_haarReadResults.size() + m_k * 2);

	if(ls > .6) {
		startActionFired();
		m_haarReadResults = std::vector<bool>(m_haarReadMax);
	}
}

void IdleView::doViewDraw()
{
	// The base class will draw the kinect stream already flipped
	View::doViewDraw();

	m_handbox.draw(625, 50); // pixel offset of box based on comps

	// Results isn't filled out until the draw call for some reason
	m_haarReadResults[m_haarReadResultsIndex++ % m_haarReadResults.size()] = finder.blobs.size() > 0;
	
	/*
	 * Uncomment to Debug 
	m_grayscale.draw(0, 0);

	finder.draw(0,0);

	ofNoFill();
	ofRect(m_handboxRect);*/
}

void IdleView::startActionFired()
{
	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::STARTING);
	}
}