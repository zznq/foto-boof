#include "ofAppGlutWindow.h"

#include "ofxUI.h"

#include "boof.h"
#include "Timer.h"
#include "KinectController.h"
#include "ViewController.h"

Boof::Boof(int windowWidth, int windowHeight)
: m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_window(new ofAppGlutWindow()), 
m_kinectController(new KinectController(false, true, false)), m_viewController(new ViewController())
{
	// setup opengl context and window
	ofSetupOpenGL(m_window, windowWidth, windowHeight, OF_WINDOW);

	ofSetFullscreen(true);
}

Boof::~Boof() {

}

//--------------------------------------------------------------
void Boof::setup(){

	// target 30 frames/second, this
	// must be called after window is created
	ofSetFrameRate(30);

	// need to call this here to initialize the internals
	// of the controller properly
	m_kinectController->setup();

	m_viewController->setup(m_kinectController);

	Timer::Initialize(true, false);
}

//--------------------------------------------------------------
void Boof::update() {
	// update the kinect controller here, before any views are updated
	m_kinectController->update();

	m_viewController->update(Timer::TimerUpdate());
}

//-------------------------------------------------------------
void Boof::draw() {
	m_viewController->draw();
}

void Boof::exit() {
	m_kinectController.reset();
}

//--------------------------------------------------------------
void Boof::keyPressed(int key)
{
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