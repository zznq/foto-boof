#include "ofAppGlutWindow.h"

#include "boof.h"
#include "KinectController.h"
#include "View.h"
#include "BwView.h"

Boof::Boof(int windowWidth, int windowHeight)
: m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_window(new ofAppGlutWindow()), 
m_kinectController(new KinectController())
{
	// setup opengl context and window
	ofSetupOpenGL(m_window, windowWidth, windowHeight, OF_WINDOW);
}

Boof::~Boof() {

}

//--------------------------------------------------------------
void Boof::setup(){

	// target 60 frames/second, this
	// must be called after window is created
	ofSetFrameRate(60);

	// need to call this here to initialize the internals
	// of the controller properly
	m_kinectController->setup();

	//Set up new Views here
	ViewPtr firstView(new View(m_kinectController, m_kinectController->getDataWidth(), m_kinectController->getDataHeight()));
	addView(firstView);

	ofPtr<BwView> secondView(new BwView(m_kinectController, m_kinectController->getDataWidth(), m_kinectController->getDataHeight()));
	addView(secondView);

	m_viewUpdateInterval = 0;
	m_viewIndex = 0;
	m_lastElapsedTime = 0;
}

//--------------------------------------------------------------
void Boof::update() {
	unsigned long long elapsedTime = ofGetElapsedTimeMillis();
	m_viewUpdateInterval += (elapsedTime - m_lastElapsedTime);


	// update the kinect controller here, before any views are updated
	m_kinectController->update();

	if(m_viewUpdateInterval > this->getCurrentView()->getViewInterval()) {
		this->nextView();
	}

	this->getCurrentView()->update();

	m_lastElapsedTime = elapsedTime;
}

void Boof::exit() {
	m_kinectController.reset();
}

//--------------------------------------------------------------
void Boof::draw() {
	this->getCurrentView()->draw();

	// set color to black for text
	ofSetColor(0, 0, 0);

	// draw a string for the effect options
	ofDrawBitmapString("Press 1 to toggle Black and White effect", 20, 620);

	// reset color to white
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void Boof::keyPressed(int key){
	switch (key) {
		case '1': 
		{
			//m_bwEffectEnabled ? m_kinectController->removeEffect(m_bwEffect->getName()) : m_kinectController->addEffect(m_bwEffect);
			//m_bwEffectEnabled = !m_bwEffectEnabled;
			break;
		}
	}
}

//--------------------------------------------------------------
void Boof::keyReleased(int key){

}

//--------------------------------------------------------------
void Boof::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void Boof::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void Boof::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void Boof::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void Boof::windowResized(int w, int h){
	m_windowWidth = w;
	m_windowHeight = h;
}

//--------------------------------------------------------------
void Boof::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void Boof::dragEvent(ofDragInfo dragInfo){ 

}

Boof::WindowPtr Boof::getWindow() const {
	return m_window;
}

void Boof::addView(const ViewPtr& view) {
	m_views.push_back(view);
}

Boof::ViewPtr Boof::getCurrentView() {
	if (m_viewIndex < m_views.size() && m_viewIndex >= 0) {
		return m_views[m_viewIndex];
	}

	return ViewPtr();
}

void Boof::nextView() {
	m_viewUpdateInterval = 0;
	if (m_viewIndex < m_views.size()-1) {
		m_viewIndex++;
	}
}