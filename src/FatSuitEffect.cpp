#include "ofVboMesh.h"
#include "ofxKinect.h"

#include "FatSuitEffect.h"
#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

FatSuitEffect::FatSuitEffect(int width, int height, bool wireframe, bool useNormalColors)
: VisualEffect("test_effect"), m_width(width), m_height(height),
m_isDirty(false), m_chubFactor(1.0), m_shader(new ofShader()), m_normalShader(new ofShader()),
m_blurShader(new ofShader()), m_depthShader(new ofShader()), m_mesh(new ofVboMesh()), 
m_nearDepth(500.f), m_farDepth(4000.f), m_shaderSetup(false), m_drawWireframe(wireframe), 
m_clipValue(1.f), m_blurFactor(0.f), m_cullingValue(0.1f), m_transX(0.0), m_transY(0.0), 
m_transZ(0.0), m_meshStep(1.0), m_useNormalColor(useNormalColors)
{
	if (m_useNormalColor) 
	{
		m_meshStep = 4.0;
	}

	m_mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	m_mesh->setUsage(GL_DYNAMIC_DRAW);
	createMesh();

	ofDisableArbTex();
	ofEnableNormalizedTexCoords();
	m_fbo.allocate(m_width, m_height);
	m_blurHorizontal.allocate(m_width, m_height);
	m_blurVertical.allocate(m_width, m_height);
	m_colorTex.allocate(m_width, m_height, GL_RGBA);
	m_displacementTex.allocate(m_width, m_height, GL_RGB32F_ARB);
	m_depthTex.allocate(m_width, m_height, GL_RGBA);
	ofEnableArbTex();
	ofDisableNormalizedTexCoords();
}

FatSuitEffect::~FatSuitEffect() 
{

}

void FatSuitEffect::createMesh() 
{
	m_mesh->clear();

	float m_numRows = m_width / m_meshStep;
	float m_numCols = m_height / m_meshStep;
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

void FatSuitEffect::setupShader() 
{
	m_shader->load("shaders/mainVert.glsl", "shaders/mainFrag.glsl", "shaders/default.geom");
	m_normalShader->load("shaders/default.vert", "shaders/normalmap.frag");
	m_blurShader->load("shaders/default.vert", "shaders/blurFrag.glsl");
	m_depthShader->load("shaders/default.vert", "shaders/depthFrag.glsl");
	m_shaderSetup = true;
}

void FatSuitEffect::preDraw() 
{
	m_parent->setKinectDepthClipping(m_nearDepth, m_farDepth);

	// disabled rectangle texture and fall back to tex_2d (pot textures)
	ofDisableArbTex();
	ofEnableNormalizedTexCoords();

	if (!m_shaderSetup) 
	{
		setupShader();
	}
}

void FatSuitEffect::postDraw()
{
	// turn rectangle textures back on (npot textures)
	ofEnableArbTex();
	ofDisableNormalizedTexCoords();
}

void FatSuitEffect::draw()
{
	KinectController::KinectInterfacePtr kinectInterface = m_parent->getKinectController()->getKinect();

	m_displacementTex.loadData(m_parent->getKinectController()->getKinect()->getDistancePixelsRef());
	m_depthTex.loadData(m_parent->getKinectData().m_depthStream);

	// generate normal map
	m_fbo.begin();
	m_normalShader->begin();
	m_normalShader->setUniform1f("xOffset", 1.f/m_width);
	m_normalShader->setUniform1f("yOffset", 1.f/m_height);
	m_normalShader->setUniform1f("scaleFactor", m_clipValue);
	m_displacementTex.draw(0, 0);
	m_normalShader->end();
	m_fbo.end();

	// blur variables
	int blurSize = 7;
	int sigma = 3;
	float kernelRes = 21.f;
	float invKernelRes = 1.f/kernelRes;

	// load blur kernel
	ofImage blurKernel;
	blurKernel.loadImage("graphics/blur_kernel.png");

	// vertical blur pass
	m_blurVertical.begin();
	m_blurShader->begin();
	m_blurShader->setUniform1i("horizontalPass", 0);
	m_blurShader->setUniform1i("blurSize", blurSize);
	m_blurShader->setUniform1f("sigma", sigma);
	m_blurShader->setUniform1f("pixelSize", 1.f/m_blurVertical.getHeight());
	m_blurShader->setUniformTexture("kernelTex", blurKernel.getTextureReference(), 1);
	m_blurShader->setUniform1f("kernelRes", kernelRes);
	m_blurShader->setUniform1f("invKernelRes", invKernelRes);
	m_blurShader->setUniform2f("orientationVector", 0.f, 1.f/m_fbo.getHeight());
	m_blurShader->setUniform1f("blurAmt", m_blurFactor);
	m_fbo.draw(0, 0);
	m_blurShader->end();
	m_blurVertical.end();

	// horizontal blur pass
	m_blurHorizontal.begin();
	m_blurShader->begin();
	m_blurShader->setUniform1i("horizontalPass", 1);
	m_blurShader->setUniform1i("blurSize", blurSize);
	m_blurShader->setUniform1f("sigma", sigma);
	m_blurShader->setUniform1f("pixelSize", 1.f/m_blurHorizontal.getWidth());
	m_blurShader->setUniformTexture("kernelTex", blurKernel.getTextureReference(), 1);
	m_blurShader->setUniform1f("kernelRes",kernelRes);
	m_blurShader->setUniform1f("invKernelRes", invKernelRes);
	m_blurShader->setUniform2f("orientationVector", 1.f/m_blurVertical.getWidth(), 0.f);
	m_blurShader->setUniform1f("blurAmt", m_blurFactor);
	m_blurVertical.draw(0, 0);
	m_blurShader->end();
	m_blurHorizontal.end();

	// create merged color and depth texture
	// (color is rgb and depth is a in new texture)
	ofFbo depthFbo;
	depthFbo.allocate(m_width, m_height);
	depthFbo.begin();
	m_depthShader->begin();
	m_depthShader->setUniform1f("scale", 1.f);
	m_depthShader->setUniformTexture("video_tex", m_colorTex, 1);
	m_depthShader->setUniformTexture("depth_tex", m_depthTex, 2);
	m_colorTex.draw(0, 0);
	m_depthShader->end();
	depthFbo.end();

	m_colorTex.loadData(m_parent->getKinectData().m_videoStream);
	
	//Flip image and re-adjust
	ofScale(-1, 1, 1);
	ofTranslate(-this->m_width, 0, 0);

	ofFbo finalPass;
	finalPass.allocate(m_width, m_height);
	finalPass.begin();
	ofTranslate(m_transX, m_transY, m_transZ);
	m_shader->begin();
	m_shader->setUniform3f("eyePos", 0, 0 , 1);
	m_shader->setUniform1f("clip", m_clipValue);
	m_shader->setUniform1f("chub_factor", m_chubFactor);
	m_shader->setUniform1f("near_depth", m_nearDepth);
	m_shader->setUniform1f("far_depth", m_farDepth);
	m_shader->setUniform1f("cullingValue", m_cullingValue);
	m_shader->setUniform1i("useNormalColor", m_useNormalColor ? 1 : 0);
	m_shader->setUniformTexture("normal_tex", m_blurHorizontal.getTextureReference(), 2);
	m_shader->setUniformTexture("displacement_tex", m_depthTex, 3);
	m_shader->setUniformTexture("depth_tex", depthFbo.getTextureReference(), 1);

	if (!m_drawWireframe)
	{
		m_mesh->draw();
	}
	else 
	{
		m_mesh->drawWireframe();
	}
	
	m_shader->end();
	finalPass.end();

	finalPass.draw(0, 0, m_width, m_height);
}

void FatSuitEffect::addUI(CanvasPtr canvas) 
{
	ofxUISlider* slider = new ofxUISlider("Chub Factor", -200.f, 200.0f, m_chubFactor, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	ofxUISpacer* spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Blur Factor", 0.0f, 0.1f, m_chubFactor, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
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

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Clip", 0.f, 1.f, m_clipValue, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Culling Value", 0.f, 1.f, m_cullingValue, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Trans X", 0.f, 500.f, m_transX, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Trans Y", 0.f, 500.f, m_transY, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Trans Z", 0.f, 500.f, m_transZ, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	slider = new ofxUISlider("Mesh Step", 0.f, 10.f, m_transZ, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	/*
	std::vector<std::string> radioOptions;
	radioOptions.push_back("Fill");
	radioOptions.push_back("Wireframe");
	ofxUIRadio* radio = new ofxUIRadio("Draw Mode", radioOptions, OFX_UI_ORIENTATION_VERTICAL, 10, 10);
	canvas->addWidgetDown(radio);
	m_widgets.push_back(radio);

	spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	std::vector<std::string> normalOptions;
	normalOptions.push_back("Raw");
	normalOptions.push_back("Normals");
	ofxUIRadio* normalRadio = new ofxUIRadio("Color Options", normalOptions, OFX_UI_ORIENTATION_VERTICAL, 10, 10);
	canvas->addWidgetDown(normalRadio);
	m_widgets.push_back(normalRadio);
	*/

	m_event = &canvas->newGUIEvent;

	ofAddListener(*m_event,this,&FatSuitEffect::guiEvent);

	if (m_drawWireframe) {
		canvas->loadSettings("GUI/FatSuitWireSettings.xml");
	}
	else {
		canvas->loadSettings("GUI/FatSuitSettings.xml");
	}
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

	if(name == "Blur Factor")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_blurFactor != slider->getScaledValue()) {
			m_blurFactor = slider->getScaledValue();
		}
	}

	if(name == "Near Depth")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_nearDepth != slider->getScaledValue()) {
			m_nearDepth = slider->getScaledValue();
			m_parent->setKinectDepthClipping(m_nearDepth, m_farDepth);
		}
	}

	if(name == "Far Depth")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_farDepth != slider->getScaledValue()) {
			m_farDepth = slider->getScaledValue();
			m_parent->setKinectDepthClipping(m_nearDepth, m_farDepth);
		}
	}

	if(name == "Clip")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_clipValue != slider->getScaledValue()) {
			m_clipValue = slider->getScaledValue();
		}
	}

	if(name == "Culling Value")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_cullingValue != slider->getScaledValue()) {
			m_cullingValue = slider->getScaledValue();
		}
	}

	if (name == "Trans X")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_transX != slider->getScaledValue()) {
			m_transX = slider->getScaledValue();
		}
	}

	if (name == "Trans Y")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_transY != slider->getScaledValue()) {
			m_transY = slider->getScaledValue();
		}
	}


	if (name == "Trans Z")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_transZ != slider->getScaledValue()) {
			m_transZ = slider->getScaledValue();
		}
	}

	if (name == "Mesh Step")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_meshStep != slider->getScaledValue()) {
			m_meshStep = slider->getScaledValue();
		}
	}

	/*
	if(name == "Raw")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		bool toggled = toggle->getValue();
		m_useNormalColor = false;
	}

	if (name == "Normals")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		bool toggled = toggle->getValue();
		m_useNormalColor = true;
	}

	if(name == "Fill")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		bool toggled = toggle->getValue();
		m_drawWireframe = !toggled;
	}

	if (name == "Wireframe")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
		bool toggled = toggle->getValue();
		m_drawWireframe = toggled;
	}
	*/

	if (m_drawWireframe) {
		m_parent->getCanvas()->saveSettings("GUI/FatSuitWireSettings.xml");
	}
	else {
		m_parent->getCanvas()->saveSettings("GUI/FatSuitSettings.xml");
	}
}