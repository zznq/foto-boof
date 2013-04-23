#include "IdleView.h"
#include "ViewController.h"


//TODO: Adjust m_handbox to accomidate for larger display
IdleView::IdleView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, false), 
	m_threshold(50), m_haarReadMax(10), m_k(1), m_haarReadResultsIndex(0)
{
	m_canvas->setColorBack(ofColor(0.0f, 0.0F, 0.0F, 0.0f));

	m_grayscale.allocate(width, height);
	m_color.allocate(width, height);

	finder.setup("haarcascade_hand.xml");
	finder.setScaleHaar(1.3);

	m_haarReadResults = std::vector<bool>(m_haarReadMax);

	m_handbox = ofRectangle(40, 40, width * .50, height * .55);
}

void IdleView::setup() {
	View::setup();

	this->setKinectDepthClipping(500, 1000);
}

void IdleView::update(float delta) {
	View::update(delta);

	m_grayscale.setFromPixels(getKinectData().m_depthStream);
	m_color.setFromPixels(getKinectData().m_videoStream);
	
	m_grayscale.blurGaussian();
	m_grayscale.threshold(m_threshold, false);

	// Improves performance a ton by specifiying a ROI Rect
	finder.findHaarObjects(m_grayscale, m_handbox);

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
		std::cout << "ls: " << ls << "\tSTART!!!!!!!!!!!!!!!!!!!\n";
		startActionFired();
		m_haarReadResults = std::vector<bool>(m_haarReadMax);
	}
}

void IdleView::doViewDraw()
{
	m_haarReadResults[m_haarReadResultsIndex++ % m_haarReadResults.size()] = finder.blobs.size() > 0;

	//m_color.draw(0, 0, this->getWidth(), this->getHeight());
	m_grayscale.draw(0, 0, this->getWidth(), this->getHeight());

	finder.draw(0,0);

	ofNoFill();
	ofRect(m_handbox);
}

void IdleView::startActionFired()
{
	if(this->m_delegate)
	{
		this->m_delegate->handleViewAction(ViewAction::STARTING);
	}
}