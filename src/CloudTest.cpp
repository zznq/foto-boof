#include "ofxKinect.h"

#include "CloudTest.h"

#include "KinectController.h"

CloudTest::CloudTest(int width, int height)
: VisualEffect("cloud_test"), m_width(width), m_height(height) 
{
	nearTh = 350;
	farTh = 1150;
	leftTh = 28.8;
	rightTh = 640;
	topTh = 21.6;
	bottomTh = 468;
	step = 8;
	dof24_focalDepth = 6.225;
	dof24_focalLength = 4.15;
	dof24_fstop = 3.48;
	dof24_maxblur = 1.2;
	dof24_showFocus = 0;
	directionLightX = 180;
	directionLightY = 50;
	directionLightZ = 260;
	cubeSizeMin = 1.5;
	cubeSizeMax = 6;
	depth_Far = 700;
	depth_Near = 1;

	ofSetFrameRate(60);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetSmoothLighting(true);
	ofEnableAlphaBlending();
	
	// - Shaders
	dofShader.load("shaders/dofPass");
	depthShader.load("shaders/depthPass");
	
	// - FBO
	depthFbo.allocate(ofGetWidth(), ofGetHeight());
	colorFbo.allocate(ofGetWidth(), ofGetHeight());
	
	// initialize FBOs
	depthFbo.begin();
	ofClear(0,0,0,0);
	depthFbo.end();
	
	colorFbo.begin();
	ofClear(0,0,0,0);
	colorFbo.end();

	// lights
	light.setDirectional();
	light.setOrientation(ofVec3f(0,90,0));

	// - GUI
	/*
	gui.addPage("Kinect DOF");
	gui.addSlider("Near Th", nearTh, 0, 10000);
	gui.addSlider("Far Th", farTh, 0, 10000);
	gui.addSlider("Left Th", leftTh, 0, 640);
	gui.addSlider("Right Th", rightTh, 0, 640);
	gui.addSlider("Top Th", topTh, 0, 480);
	gui.addSlider("Bottom Th", bottomTh, 0, 480);
	gui.addSlider("Simplification", simplifaction, 1, 16);
	
	gui.addSlider("dof24 Focal Depth", dof24_focalDepth, 1.0,20.0);
	gui.addSlider("dof24 Focal Length", dof24_focalLength, 0, 10);
	gui.addSlider("dof24 fstop", dof24_fstop, 0, 24);
	gui.addSlider("dof24_maxblur", dof24_maxblur, 0,10.0);
	gui.addToggle("dof24 show", dof24_showFocus);
	
	gui.addSlider("Light X", directionLightX, -1000, 1000);
	gui.addSlider("Light Y", directionLightY, -1000, 1000);
	gui.addSlider("Light Z", directionLightZ, -2000, 2000);
	
	gui.addSlider("cubeSize Min", cubeSizeMin, 0, 100);
	gui.addSlider("cubeSize Max", cubeSizeMax, 0, 100);
	
	gui.addSlider("Depth Far", depth_Far, 0,10000);
	gui.addSlider("Depth Near", depth_Near, 0, 100);
	
	gui.loadFromXML();
	*/

}

//--------------------------------------------------------------
void CloudTest::preDraw(){	
	light.setOrientation(ofVec3f(directionLightX, directionLightY, directionLightZ));

	/*
	if(gui.isOn()) {
		cam.disableMouseInput();
	} else {
		cam.enableMouseInput();
	}
	*/
	
}


//--------------------------------------------------------------
void CloudTest::drawBoxes() {
	KinectController::KinectInterfacePtr kinect = m_parent->getKinectController()->getKinect();

	for(int x=0; x<m_width; x+=step) {
		for(int y=0; y<m_height; y+=step) {

			float depth = kinect->getDistanceAt(x, y);

			if(depth > nearTh && depth < farTh) {
				ofVec3f wc = kinect->getWorldCoordinateAt(x, y);
				ofColor c = kinect->getColorAt(x, y);

				c *= 60.0;
				ofSetColor(c);
				//ofSphere(wc.x, wc.y,wc.z,  ofMap(ofNoise(x,y),0,1.0,cubeSizeMin,cubeSizeMax));
				ofBox(wc.x,wc.y,wc.z, ofMap(ofNoise(x,y),0,1.0,cubeSizeMin,cubeSizeMax));
			}
		}
	}
}
//--------------------------------------------------------------
void CloudTest::drawScene() {
	//cam.begin();
	//ofPushMatrix();
	//ofTranslate(0,0, 1800);
	//ofScale(2,2,-2);
	drawBoxes();
	//ofPopMatrix();
	//cam.end();
}

//--------------------------------------------------------------
void CloudTest::draw(){
	glEnable(GL_DEPTH_TEST);
	ofEnableAlphaBlending();
	// depth pass
	depthFbo.begin();
	ofClear(0,0,0,0);
	depthShader.begin();
	depthShader.setUniform1f("near", depth_Near);
	depthShader.setUniform1f("far", depth_Far);
	drawScene();
	depthShader.end();
	depthFbo.end();
	
	// color Pass
	ofEnableLighting();
	light.enable();
	colorFbo.begin();
	ofClear(0,0,0,0);
	drawScene();
	colorFbo.end();
	light.disable();
	ofDisableLighting();

	
	ofBackgroundGradient(ofColor(0,0,0), ofColor(80,80,80));

	dofShader.begin();
	dofShader.setUniformTexture("bgl_RenderedTexture", colorFbo.getTextureReference(), 0);
	dofShader.setUniformTexture("bgl_DepthTexture", depthFbo.getTextureReference(), 1);
	dofShader.setUniform1f("bgl_RenderedTextureWidth", ofGetWidth());
	dofShader.setUniform1f("bgl_RenderedTextureHeight", ofGetHeight());
	dofShader.setUniform1f("focalDepth", dof24_focalDepth);
	dofShader.setUniform1f("focalLength", dof24_focalLength);
	dofShader.setUniform1f("fstop", dof24_fstop);
	dofShader.setUniform1f("maxblur", dof24_maxblur);
	dofShader.setUniform1i("showFocus", dof24_showFocus);
	colorFbo.draw(0, 0);
	dofShader.end();

	
	ofDisableAlphaBlending();
	// GUI
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//gui.draw();
	//glPopAttrib();
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}