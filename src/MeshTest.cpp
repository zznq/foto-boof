#include "ofxKinect.h"
#include "MeshTest.h"
#include "KinectController.h"

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

namespace {
	ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
		return ofVec3f((x / Xres - .5f) * z * XtoZ,
									 (y / Yres - .5f) * z * YtoZ,
									 z);
	}
}

ofVec3f getNormal(ofVec3f& a, ofVec3f& b, ofVec3f& c) {
	ofVec3f side1 = a - b;
	ofVec3f side2 = c - b;
	ofVec3f normal = side1.cross(side2);
	normal.normalize();
	return normal;
}

MeshTest::MeshTest(int width, int height) 
: VisualEffect("mesh_test"), m_width(width), m_height(height)
{
	//ofSetVerticalSync(true);
	light.setPosition(0,ofGetMouseX(), -ofGetMouseY());
	light.setDiffuseColor(ofColor(255,0,0));
	light.setAmbientColor(ofColor(0));
	light.enable();
}

void MeshTest::draw() {
	//ofBackground(0);
	
	KinectController::KinectInterfacePtr kinect = m_parent->getKinectController()->getKinect();

	float* distancePixels = m_parent->getKinectData().m_distanceValues.getPixels(); // distance in centimeters
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for(int y = 0; y < m_height - 1; y++) { // don't go to the end
		for(int x = 0; x < m_width - 1; x++) { // don't go to the end
			// get indices for each corner
			int nwi = (y + 0) * m_width + (x + 0);
			int nei = (y + 0) * m_width + (x + 1);
			int sei = (y + 1) * m_width + (x + 1);
			int swi = (y + 1) * m_width + (x + 0);
				
			// get z values for each corner
			float nwz = distancePixels[nwi];
			float nez = distancePixels[nei];
			float sez = distancePixels[sei];
			float swz = distancePixels[swi];
				
			if(nwz > 0 && nez > 0 && sez > 0 && swz > 0) { // ignore empty depth pixels
				// get real world locations for each corner
				//ofVec3f nwv = ConvertProjectiveToRealWorld(x + 0, y + 0, nwz);
				//ofVec3f nev = ConvertProjectiveToRealWorld(x + 1, y + 0, nez);
				//ofVec3f sev = ConvertProjectiveToRealWorld(x + 1, y + 1, sez);
				//ofVec3f swv = ConvertProjectiveToRealWorld(x + 0, y + 1, swz);
				
				ofVec3f nwv = kinect->getWorldCoordinateAt(x+0, y+0);
				ofVec3f nev = kinect->getWorldCoordinateAt(x + 1, y + 0);
				ofVec3f sev = kinect->getWorldCoordinateAt(x + 1, y + 1);
				ofVec3f swv = kinect->getWorldCoordinateAt(x + 0, y + 1);

				// compute normal for the upper left
				ofVec3f normal = getNormal(nwv, nev, swv);
				
				// add the upper left triangle
				mesh.addNormal(normal);
				mesh.addVertex(nwv);
				mesh.addNormal(normal);
				mesh.addVertex(nev);
				mesh.addNormal(normal);
				mesh.addVertex(swv);
					
				// add the bottom right triangle
				mesh.addNormal(normal);
				mesh.addVertex(nev);
				mesh.addNormal(normal);
				mesh.addVertex(sev);
				mesh.addNormal(normal);
				mesh.addVertex(swv);
			}
		}
	}

	easyCam.begin();
	ofScale(1, -1, -1); // orient the point cloud properly
	ofTranslate(0, 0, -150); // rotate about z = 150 cm
	glEnable(GL_DEPTH_TEST);
	ofSetColor(255);
	//if(drawWireframe) {
	//	ofDisableLighting();
	//	mesh.drawWireframe();
	//} else {
		ofEnableLighting();
		mesh.draw();
	//}
	easyCam.end();
	
	ofDisableLighting();
}