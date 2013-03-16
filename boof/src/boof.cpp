#include "ofAppGlutWindow.h"

#include "boof.h"
#include "KinectController.h"

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
}

//--------------------------------------------------------------
void Boof::update() {
	m_kinectController->update();
}

void Boof::exit() {
	m_kinectController.reset();
}

//--------------------------------------------------------------
void Boof::draw(){
	m_kinectController->draw();
}

//--------------------------------------------------------------
void Boof::keyPressed(int key){

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