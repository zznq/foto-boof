#include "ofAppGlutWindow.h"

#include "boof.h"
#include "KinectController.h"
#include "View.h"
#include "BwView.h"

Boof::Boof()
: m_window(new ofAppGlutWindow()), m_kinectController(new KinectController())
{

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

	m_views = Views();
	//Set up new Views here
	View* firstView = new View(m_kinectController);
	addView(firstView);

	BwView* secondView = new BwView(m_kinectController);
	addView(secondView);

	firstView->start();

	m_viewUpdateInterval = 0;
	m_viewIndex = 0;
	m_lastElapsedTime = 0;
}

//--------------------------------------------------------------
void Boof::update() {
	unsigned long long elapsedTime = ofGetElapsedTimeMillis();
	m_viewUpdateInterval += (elapsedTime - m_lastElapsedTime);

	if(m_viewUpdateInterval > this->getCurrentView()->getViewInterval()) {
		this->nextView();
	}

	//m_kinectController->update();
	this->getCurrentView()->update();

	m_lastElapsedTime = elapsedTime;
}

void Boof::exit() {
	m_kinectController.reset();
}

//--------------------------------------------------------------
void Boof::draw() {
	//m_kinectController->draw();
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

void Boof::addView(View* view) {
	m_views.push_back(view);
}

View* Boof::getCurrentView() {
	return m_views.at(m_viewIndex);
}

void Boof::nextView() {
	m_viewUpdateInterval = 0;
	if(m_viewIndex < m_views.size() -1) {
		m_viewIndex++;

		this->getCurrentView()->start();
	}
}