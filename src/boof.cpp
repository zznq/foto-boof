#include "ofAppGlutWindow.h"

#include "ofxUI.h"

#include "boof.h"
#include "Timer.h"
#include "KinectController.h"

ViewPtr CreateView(const ViewType::Enum& type, const KinectControllerPtr& kinectController, int width, int height);

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

	// target 30 frames/second, this
	// must be called after window is created
	ofSetFrameRate(30);

	// need to call this here to initialize the internals
	// of the controller properly
	m_kinectController->setup();

	// add all view types
	for (int i=0; i < ViewType::Max; ++i) {
		ViewPtr view = CreateView(static_cast<ViewType::Enum>(i), m_kinectController, m_kinectController->getDataWidth(), m_kinectController->getDataHeight());
		view->setViewDelegate(View::ViewDelegatePtr(this));
		if(i ==0) {
			view->setup();
		}
		
		addView(view);
	}
	
	m_viewIndex = 0;

	Timer::Initialize(true, false);

	m_viewsIterator = m_views.begin();
}

//--------------------------------------------------------------
void Boof::update() {
	// update the kinect controller here, before any views are updated
	m_kinectController->update();

	this->getCurrentView()->update(Timer::TimerUpdate());
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

void Boof::addView(const ViewPtr& view) {
	m_views.push_back(view);
}

Boof::ViewPtr Boof::getCurrentView() {
	return (* m_viewsIterator);
}

void Boof::viewStart()
{
}

void Boof::viewComplete()
{
	this->getCurrentView()->close();

	m_viewsIterator++;

	if(m_viewsIterator == m_views.end()) {
		m_viewsIterator = m_views.begin();
	}

	this->getCurrentView()->setup();
}

void Boof::viewCountdownStarted() 
{

}