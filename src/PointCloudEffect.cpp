#include "ofVboMesh.h"
#include "ofxKinect.h"

#include "PointCloudEffect.h"
#include "KinectController.h"

ofTexture texture;
void CreateSolidTexture() {
	ofPixels pixels = ofPixels();
	pixels.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);

	ofColor c;
	for(int i = 0; i < 640; i++) {
		for (int j=0; j < 480; ++j) {
			c.r = 1.0;
			c.g = 0.0;
			c.b = 0.0;
			c.a = 1.0;
			pixels.setColor(i, j, c);
		}
	}

	texture.allocate(640, 480, GL_RGB);
}

PointCloudEffect::PointCloudEffect(int width, int height)
: ShaderEffect("point_cloud", "shaders/pointcloud.vert", "shaders/pointcloud.frag"),
m_width(width), m_height(height), m_mesh(new ofVboMesh()), m_numRows(100), m_numCols(100), 
m_meshDirty(false), m_pointSize(1), m_nearDepth(500), m_farDepth(4000)
{
	m_colorTexture.allocate(m_width, m_height, GL_RGB);
	m_mesh->setMode(OF_PRIMITIVE_POINTS);
	m_mesh->setUsage(GL_DYNAMIC_DRAW);

	createMesh();

	CreateSolidTexture();
}

PointCloudEffect::~PointCloudEffect()
{
	m_colorTexture.clear();
}

void PointCloudEffect::createMesh()
{
	m_mesh->clear();
	
	for(int row=0; row < m_numRows; row++) {
		for (int col=0; col < m_numCols; col++) {
			// set current x y z
			int x = m_width * (col/(m_numCols-1));
			int y = m_height * (row/(m_numRows-1));
			int z = 0;

			// mesh
			// add color, vertex and texture coordinates
			//m_mesh->addColor(ofFloatColor(1,1,1,1));
			//mesh.addNormal(ofVec3f(0, 0, 1));
			m_mesh->addVertex(ofVec3f(x, y, z));

			//m_mesh->addTexCoord(ofVec2f( col/(m_numCols-1), row/(m_numRows-1)) ); 
		}
	}
}

void PointCloudEffect::draw()
{
	// generate point mesh
	if (m_meshDirty)
	{
		createMesh();
		m_meshDirty = false;
	}
	
	glTranslatef(-m_width*.5, -m_height*.5, 0);
	KinectController::KinectInterfacePtr kinectInterface = m_parent->getKinectController()->getKinect();

	// offset z vertices by the distance
	ofFloatPixels& distanceValues = m_parent->getKinectData().m_distanceValues;
	std::vector<ofVec3f>& vertices = m_mesh->getVertices();
	std::vector<ofVec3f>::iterator iter;
	for (iter = vertices.begin(); iter != vertices.end(); ++iter) 
	{
		ofVec3f& vertex = *iter;
		vertex.z = kinectInterface->getDistanceAt(vertex.x, vertex.y);
	}

	// bind color texture
	m_colorTexture.loadData(m_parent->getKinectData().m_videoStream);

	m_shader->setUniform1f("near_depth", m_nearDepth);
	m_shader->setUniform1f("far_depth", m_farDepth);
	m_shader->setUniformTexture("color_lookup", texture/*m_colorTexture*/, 5);
	glPointSize(m_pointSize);
	//m_mesh->draw();
	texture.draw(0, 0);

	ShaderEffect::draw();
}

void PointCloudEffect::addUI(CanvasPtr canvas) 
{
	ofxUISlider* slider = new ofxUISlider("Point Size", 0.0f, 10.f, m_pointSize, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	ofxUISpacer* spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Near Depth", 0.0f, 500.f, m_nearDepth, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	 spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);
	
	slider = new ofxUISlider("Far Depth", 501.0f, 8000.f, m_farDepth, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	m_event = &canvas->newGUIEvent;

	ofAddListener(*m_event, this, &PointCloudEffect::guiEvent);

	canvas->loadSettings("GUI/PointCloudEffectSettings.xml");
}

void PointCloudEffect::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 

	if(name == "Point Size")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_pointSize != slider->getScaledValue()) {
			m_pointSize = slider->getScaledValue();
		}
	}

	if(name == "Near Depth")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_nearDepth != slider->getScaledValue()) {
			m_nearDepth = slider->getScaledValue();
		}
	}

	if(name == "Far Depth")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_farDepth != slider->getScaledValue()) {
			m_farDepth = slider->getScaledValue();
		}
	}


	m_parent->getCanvas()->saveSettings("GUI/PointCloudEffectSettings.xml");
}
