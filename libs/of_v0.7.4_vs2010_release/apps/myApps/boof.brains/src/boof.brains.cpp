#include "boof.brains.h"

//--------------------------------------------------------------
void BoofBrains::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	kinect.setRegistration(true);

	kinect.init();

	kinect.open();

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);

	nearThreshold = 230;
	farThreshold = 70;
	bThreshWithOpenCV = true;

	ofSetFrameRate(60);

	angle = 20;
	kinect.setCameraTiltAngle(angle);

	bDrawPointCloud = false;
}

//--------------------------------------------------------------
void BoofBrains::update(){
	ofBackground(100, 100, 100);

	kinect.update();

	if(kinect.isFrameNew()) {
		
	}
}

//--------------------------------------------------------------
void BoofBrains::draw(){
	ofSetColor(255, 255, 255);

	// draw from the live kinect
	kinect.drawDepth(10, 10, 400, 300);
	kinect.draw(420, 10, 400, 300);
}

//--------------------------------------------------------------
void BoofBrains::keyPressed(int key){

}

//--------------------------------------------------------------
void BoofBrains::keyReleased(int key){

}

//--------------------------------------------------------------
void BoofBrains::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void BoofBrains::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BoofBrains::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void BoofBrains::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void BoofBrains::windowResized(int w, int h){

}

//--------------------------------------------------------------
void BoofBrains::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void BoofBrains::dragEvent(ofDragInfo dragInfo){ 

}