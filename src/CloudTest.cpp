#include "ofxKinect.h"

#include "CloudTest.h"

#include "KinectController.h"
const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;


ofEasyCam cloudEasyCam;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
		(y / Yres - .5f) * z * YtoZ,
		z);
}

CloudTest::CloudTest(int width, int height)
: VisualEffect("cloud_test"), m_width(width), m_height(height)
{

}

void CloudTest::draw() {
	ofBackground(0);

	ofSetColor(255);
	ofTexture texture;
	texture.allocate(m_width, m_height, GL_LUMINANCE);
	texture.loadData(m_parent->getKinectData().m_depthStream.getPixels(), m_width, m_height, GL_LUMINANCE);
	texture.draw(0, 0, 400, 300);

	cloudEasyCam.begin();

	ofScale(1, -1, -1); // orient the point cloud properly
	//if(useProjective) {
		//ofTranslate(-m_width / 2, -m_height / 2); // center the projective point cloud
	//} else {
		ofTranslate(0, 0, -150); // rotate about z = 150 cm
	//}
	unsigned short* distancePixels = m_parent->getKinectData().m_distanceValues.getPixels(); // distance in centimeters
	ofMesh cloud;
	cloud.setMode(OF_PRIMITIVE_POINTS);
	for(int y = 0; y < m_height; y++) {
		for(int x = 0; x < m_width; x++) {
			if (x % 2 == 0) {
				int i = y * m_width + x;
				float z = distancePixels[i];
				if(z != 0) { // ignore empty depth pixels
					//if(useProjective) {
						//cloud.addVertex(ofVec3f(x, y, z));
					//} else {
						cloud.addVertex(m_parent->getKinectController()->getKinect()->getWorldCoordinateAt(x, y, z));
					//}
				}
			}
		}
	}
	cloud.drawVertices();
	cloudEasyCam.end();

	ofSetColor(255);
	//ofDrawBitmapString(useProjective ? "projective point cloud" : "real world point cloud", 10, 20);
}
