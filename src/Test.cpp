#include "ofVboMesh.h"
#include "ofxKinect.h"

#include "Test.h"
#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

ofEasyCam easyCam;

TestEffect::TestEffect(int width, int height)
: VisualEffect("test_effect"), m_width(width), m_height(height),
m_isDirty(false), m_chubFactor(1.0), m_shader(new ofShader()), m_normalShader(new ofShader()),
m_mesh(new ofVboMesh()), m_nearDepth(500.f), m_farDepth(4000.f), m_shaderSetup(false)
{
	// disabled rectangle texture and fall back to tex_2d (pot textures)
	ofDisableArbTex();
	
	m_mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	m_mesh->setUsage(GL_DYNAMIC_DRAW);
	createMesh();

	m_fbo.allocate(width, height);
	m_colorTex.allocate(m_width, m_height, GL_RGB);

	m_displacementTex.allocate(width, height, GL_RGB);
}

TestEffect::~TestEffect() 
{
	// turn rectangle textures back on (npot textures)
	ofEnableArbTex();
}

void TestEffect::createMesh() 
{
	m_mesh->clear();

	float m_numRows = m_width/4.f;
	float m_numCols = m_height/4.f;
	std::vector<ofVec3f> vertices;
	for(int row=0; row < m_numRows; row++) {
		for (int col=0; col < m_numCols; col++) {
			// set current x y z
			int x = m_width * (col/(m_numCols-1));
			int y = m_height * (row/(m_numRows-1));
			int z = 0;

			// mesh
			// add color, vertex and texture coordinates
			m_mesh->addColor(ofFloatColor(1,1,1,1));
			m_mesh->addNormal(ofVec3f(0, 0, 1));
			m_mesh->addVertex(ofVec3f(x, y, z));
			m_mesh->addTexCoord(ofVec2f( col/(m_numCols-1), row/(m_numRows-1)) ); 
			//m_mesh->addTexCoord(ofVec2f(row/(m_numRows-1), col/(m_numCols-1)));
		}
	}

	// add indices to mesh for triangle strip generation
	int n = 0;  
	int colSteps = m_numCols * 2;  
	int rowSteps = m_numRows - 1;  
	std::vector<ofIndexType> indices;
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

void TestEffect::setupShader() 
{
	m_shader->load("shaders/test.vert", "shaders/test.frag");
	m_normalShader->load("shaders/default.vert", "shaders/normalmapv2.frag");

	m_shaderSetup = true;
}

void TestEffect::preDraw() 
{
	if (!m_shaderSetup) 
	{
		setupShader();
	}

	ofTexture texture;
	texture.allocate(m_width, m_height, GL_RGB);
	m_normalShader->begin();
	texture.loadData(m_parent->getKinectData().m_depthStream);
	texture.draw(1024-400, 768-200, 200, 200);
	m_normalShader->end();

	texture.unbind();

	m_displacementTex.draw(1024-200, 768-200, 200, 200);

	easyCam.begin();

	ofPushMatrix();
	ofTranslate(0, 0, -100);
	ofScale(1, -1, 1);  
	glTranslatef(-m_width*.5, -m_height*.5, 0);

	// draw normal map first into FBO
	m_fbo.begin();
	m_normalShader->begin();
	texture.loadData(m_parent->getKinectData().m_depthStream);
	texture.draw(0, 0);
	m_normalShader->end();
	m_fbo.end();

	m_shader->begin();
	m_displacementTex.loadData(m_parent->getKinectData().m_depthStream);
	m_colorTex.loadData(m_parent->getKinectData().m_videoStream);

	m_shader->setUniform1f("chub_factor", m_chubFactor);
	m_shader->setUniform1f("near_depth", m_nearDepth);
	m_shader->setUniform1f("far_depth", m_farDepth);
	m_shader->setUniform2f("normal_size", m_fbo.getWidth(), m_fbo.getHeight());
	m_shader->setUniformTexture("color_tex", m_colorTex, 1);
	m_shader->setUniformTexture("normal_tex", m_fbo.getTextureReference(), 2);
	m_shader->setUniformTexture("displacement_tex", m_displacementTex, 3);
}

void TestEffect::postDraw()
{
	m_shader->end();
}

void TestEffect::draw()
{
	KinectController::KinectInterfacePtr kinectInterface = m_parent->getKinectController()->getKinect();

	//m_colorTex.draw(0, 0);
	
	std::vector<ofVec3f>& vertices = m_mesh->getVertices();
	std::vector<ofFloatColor>& colors = m_mesh->getColors();
	for (int i=0; i < vertices.size(); ++i) {
		ofVec3f& vertex = vertices[i];
		ofFloatColor& color = colors[i];

		if(kinectInterface->getDistanceAt(vertex.x, vertex.y) > 0) 
		{
			//vertex.z = kinectInterface->getWorldCoordinateAt(vertex.x, vertex.y).z;
			color = kinectInterface->getColorAt(vertex.x, vertex.y);
		}
	}
	

	m_mesh->draw();
	easyCam.end();
}

void TestEffect::addUI(CanvasPtr canvas) 
{
	ofxUISlider* slider = new ofxUISlider("Chub Factor", -100.0f, 100.f, m_chubFactor, 100.0f, 25.0f);
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

	ofAddListener(*m_event,this,&TestEffect::guiEvent);

	canvas->loadSettings("GUI/TestEffectSettings.xml");
}

void TestEffect::guiEvent(ofxUIEventArgs &e)
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

	m_parent->getCanvas()->saveSettings("GUI/TestEffectSettings.xml");
}