#include "ofAppGlutWindow.h"

#include "boof.h"
#include "KinectController.h"
#include "PixelEffect.h"
#include "BwEffect.h"

Boof::Boof()
: m_window(new ofAppGlutWindow()), m_kinectController(new KinectController()), 
m_bwEffect(new BwEffect()), m_bwEffectEnabled(false)
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
			m_bwEffectEnabled ? m_kinectController->removeEffect(m_bwEffect->getName()) : m_kinectController->addEffect(m_bwEffect);
			m_bwEffectEnabled = !m_bwEffectEnabled;
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