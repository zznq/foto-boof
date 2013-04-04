#include "ofVboMesh.h"
#include "ofxKinect.h"

#include "MeshEffect.h"
#include "KinectController.h"

MeshEffect::MeshEffect(int width, int height)
: VisualEffect("mesh_effect"), 
m_meshDirty(false), m_width(width), m_height(height), m_numRows(100), m_numCols(100),
m_thresholdZ(1000), m_mesh(new ofVboMesh())
{
	m_mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	m_mesh->setUsage(GL_DYNAMIC_DRAW);

	createMesh();
}

MeshEffect::~MeshEffect()
{

}

void MeshEffect::createMesh() 
{
	m_mesh->clear();

	for(int row=0; row < m_numRows; row++){
		for (int col=0; col < m_numCols; col++) {
			// set current x y z
			int x = m_width * (col/(m_numCols-1));
			int y = m_height * (row/(m_numRows-1));
			int z = 0;

			// mesh
			// add color, vertex and texture coordinates
			m_mesh->addColor(ofFloatColor(1,1,1,1));
			//mesh.addNormal(ofVec3f(0, 0, 1));
			m_mesh->addVertex(ofVec3f(x, y, z));

			m_mesh->addTexCoord(ofVec2f( col/(m_numCols-1), row/(m_numRows-1)) ); 
		}
	}

	// add indices to mesh for triangle strip generation
	int n = 0;  
	int colSteps = m_numCols * 2;  
	int rowSteps = m_numRows - 1;  
	vector<ofIndexType> indices;
	indices.reserve(rowSteps * colSteps);
	for ( int r = 0; r < rowSteps; r++ ) {  
		for ( int c = 0; c < colSteps; c++ ) {  
			int t = c + r * colSteps;  
			if ( c == colSteps - 1 ) {  
				indices.push_back(n);
			} else {  
				indices.push_back(n);
				( t%2 == 0 ) ? n += m_numCols : (r%2 == 0) ? n -= (m_numCols-1) : n -= (m_numCols+1);  
			}  
		}  
	}
	m_mesh->addIndices(indices);
}

void MeshEffect::draw()
{
	if (m_meshDirty) {
		createMesh();
		m_meshDirty = false;
	}

	glTranslatef(-m_width*.5, -m_height*.5, 0);
	KinectController::KinectInterfacePtr kinectInterface = m_parent->getKinectController()->getKinect();

	// offset z vertices by the distance
	ofFloatPixels& distanceValues = m_parent->getKinectData().m_distanceValues;
	std::vector<ofVec3f>& vertices = m_mesh->getVertices();
	std::vector<ofVec3f>::iterator iter;
	float prevZValue = 0;
	int count = 0;
	for (iter = vertices.begin(); iter != vertices.end(); ++iter) 
	{
		ofVec3f& vertex = *iter;
		vertex.z = kinectInterface->getDistanceAt(vertex.x, vertex.y);
		if  (vertex.z > m_thresholdZ || vertex.z <= 0) 
		{
			vertex.z = prevZValue;
			m_mesh->setColor(count, ofFloatColor(0, 0, 0, 255));
		}
		else 
		{
			prevZValue = vertex.z;
			m_mesh->setColor(count, kinectInterface->getColorAt(vertex.x, vertex.y));
		}
		++count;
	}
	m_mesh->drawWireframe();
}

void MeshEffect::addUI(CanvasPtr canvas)
{
	ofxUISlider* slider = new ofxUISlider("Z Threshold", 0.0f, 5000.f, m_thresholdZ, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	ofxUISpacer* spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	ofxUISlider* slider2 = new ofxUISlider("Num Cols", 0.0f, 300.f, m_numCols, 100.0f, 25.0f);
	canvas->addWidgetDown(slider2);
	m_widgets.push_back(slider2);

	m_event = &canvas->newGUIEvent;

	ofAddListener(*m_event, this, &MeshEffect::guiEvent);

	canvas->loadSettings("GUI/MeshEffectSettings.xml");
}

void MeshEffect::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 

	if (name == "Z Threshold")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if (m_thresholdZ != slider->getScaledValue()) {
			m_thresholdZ = slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_thresholdZ << endl;
		}
	}  

	if (name == "Num Cols") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if (m_numCols != slider->getScaledValue()) {
			m_numCols = slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_numCols << endl;

			m_meshDirty = true;
		}
	}

	m_parent->getCanvas()->saveSettings("GUI/MeshEffectSettings.xml");
}
