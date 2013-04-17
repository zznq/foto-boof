#include "ofVboMesh.h"
#include "ofxKinect.h"

#include "glut.h"

#include "NormalMapEffect.h"

#include "FatSuitEffect.h"
#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

FatSuitEffect::FatSuitEffect(int width, int height)
: ShaderEffect("fat_suit", "shaders/fatsuit.vert"), 
  m_width(width), m_height(height), m_isDirty(false), m_chubFactor(1.0), m_mesh(new ofVboMesh()),
  m_nearDepth(500.f), m_farDepth(4000.f)
{
	m_mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	m_mesh->setUsage(GL_DYNAMIC_DRAW);
	createMesh();

	// bind attribute and relink program
	//glBindAttribLocation(m_shader->getProgram(), 0, "in_Position");
	//m_shader->load("shaders/fatsuit.vert", "shaders/fatsuit.frag", "");
}

FatSuitEffect::~FatSuitEffect() 
{

}

void FatSuitEffect::createMesh() 
{
	m_mesh->clear();

	m_vertices.clear();

	float m_numRows = m_width/4.f;
	float m_numCols = m_height/4.f;
	m_vertices.reserve(m_numRows * m_numCols);
	m_colors.reserve(m_numRows * m_numCols);
	for(int row=0; row < m_numRows; row++) {
		for (int col=0; col < m_numCols; col++) {
			// set current x y z
			int x = m_width * (col/(m_numCols-1));
			int y = m_height * (row/(m_numRows-1));
			int z = 0;

			Vertex v;
			v.x = x;
			v.y = y;
			v.z = z;
			m_vertices.push_back(v);
			// mesh
			// add color, vertex and texture coordinates
			m_colors.push_back(ColorValue(1, 1, 1, 1));
			//m_mesh->addColor(ofFloatColor(1,1,1,1));
			//mesh.addNormal(ofVec3f(0, 0, 1));
			//m_mesh->addVertex(ofVec3f(x, y, z));
			m_texCoords.push_back(TexCoord(col/(m_numCols-1), row/(m_numRows-1)));
			//m_mesh->addTexCoord(ofVec2f( col/(m_numCols-1), row/(m_numRows-1)) ); 
		}
	}

	// add indices to mesh for triangle strip generation
	int n = 0;  
	int colSteps = m_numCols * 2;  
	int rowSteps = m_numRows - 1;  
	m_indices.reserve(rowSteps * colSteps);
	for ( int r = 0; r < rowSteps; r++ ) {  
		for ( int c = 0; c < colSteps; c++ ) {  
			int t = c + r * colSteps;  
			if ( c == colSteps - 1 ) {  
				m_indices.push_back(n);
			} else {  
				m_indices.push_back(n);
				( t%2 == 0 ) ? n += m_numCols : (r%2 == 0) ? n -= (m_numCols-1) : n -= (m_numCols+1);  
			}  
		}  
	}
	//m_mesh->addIndices(indices);

	glGenBuffers(1, &m_vboId);
	glGenBuffers(1, &m_iboId);
	glGenBuffers(1, &m_colorId);
	glGenBuffers(1, &m_texCoordId);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord)*m_texCoords.size(), &m_texCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ofIndexType) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void FatSuitEffect::preDraw() 
{
	ShaderEffect::preDraw();
	m_shader->setUniform1f("chub_factor", m_chubFactor);
	m_shader->setUniform1f("near_depth", m_nearDepth);
	m_shader->setUniform1f("far_depth", m_farDepth);
}

void FatSuitEffect::draw()
{
	glTranslatef(-m_width*.5, -m_height*.5, 0);
	
	KinectController::KinectInterfacePtr kinectInterface = m_parent->getKinectController()->getKinect();

	for (int i=0; i < m_vertices.size(); ++i) {
		Vertex& vertex = m_vertices[i];

		if(kinectInterface->getDistanceAt(vertex.x, vertex.y) > 0) 
		{
			vertex.z = kinectInterface->getWorldCoordinateAt(vertex.x, vertex.y).z;
			ofColor ofc = kinectInterface->getColorAt(vertex.x,vertex.y);
			ColorValue c(ofc.r/255.f, ofc.g/255.f, ofc.b/255.f, ofc.a/255.f);
			m_colors[i] = c;
		}
	}

	GLint position = m_shader->getAttributeLocation("in_Position");
	GLint color = m_shader->getAttributeLocation("in_Color");
	GLint texCoord = m_shader->getAttributeLocation("in_TexCoord");

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex)*3, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColorValue) * m_colors.size(), &m_colors[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, sizeof(ColorValue)*4, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordId);
	glEnableVertexAttribArray(texCoord);
	glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(TexCoord)*2, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices.size());
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(texCoord);
	/*
	// offset z vertices by the distance
	ofPixels& distanceValues = m_parent->getKinectData().m_depthStream;
	std::vector<ofVec3f>& vertices = m_mesh->getVertices();
	std::vector<ofFloatColor>& colors = m_mesh->getColors();
	std::vector<ofVec3f>::iterator iter;
	float prevZValue = 0;
	int index = 0;
	for (iter = vertices.begin(); iter != vertices.end(); ++iter) 
	{
		ofVec3f& vertex = *iter;

		if(kinectInterface->getDistanceAt(vertex.x, vertex.y) > 0) 
		{
			colors[index] = kinectInterface->getColorAt(vertex.x,vertex.y);
			vertex.z = kinectInterface->getWorldCoordinateAt(vertex.x, vertex.y).z;
		}
		++index;
	}
	*/
	//m_mesh->drawWireframe();
}

void FatSuitEffect::addUI(CanvasPtr canvas) 
{
	ofxUISlider* slider = new ofxUISlider("Chub Factor", 1.0f, 500.0f, m_chubFactor, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	slider = new ofxUISlider("Near Depth", 0.0f, 500.f, m_nearDepth, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	ofxUISpacer* spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Far Depth", 501.0f, 8000.f, m_farDepth, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	m_event = &canvas->newGUIEvent;

	ofAddListener(*m_event,this,&FatSuitEffect::guiEvent);

	canvas->loadSettings("GUI/FatSuitSettings.xml");
}

void FatSuitEffect::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 

	if(name == "Chub Factor")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_chubFactor != slider->getScaledValue()) {
			m_chubFactor = slider->getScaledValue();
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

	m_parent->getCanvas()->saveSettings("GUI/FatSuitSettings.xml");
}