#include "ofVboMesh.h"
#include "ofxKinect.h"

#include "Test.h"
#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

#include <gl/GLU.h>

ofEasyCam easyCam;
ofFbo blurColor1;
ofFbo blurColor2;

ofPtr<ofShader> cullShader;

void CreateCullShader() 
{
	cullShader.reset(new ofShader());
	cullShader->load("shaders/cull.vert", "shaders/default.frag", "shaders/default.geom");
}

TestEffect::TestEffect(int width, int height)
: VisualEffect("test_effect"), m_width(width), m_height(height),
m_isDirty(false), m_chubFactor(1.0), m_shader(new ofShader()), m_normalShader(new ofShader()),
m_blurShader(new ofShader()), m_depthShader(new ofShader()), m_mesh(new ofVboMesh()), 
m_nearDepth(500.f), m_farDepth(4000.f), m_shaderSetup(false), m_drawWireframe(false), 
m_clipValue(1.f), m_blurFactor(0.f), m_cullingValue(0.1f)
{
	// disabled rectangle texture and fall back to tex_2d (pot textures)
	ofDisableArbTex();
	ofEnableNormalizedTexCoords();
	
	m_mesh->setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	m_mesh->setUsage(GL_DYNAMIC_DRAW);
	createMesh();

	m_fbo.allocate(m_width, m_height);
	m_blurHorizontal.allocate(m_width, m_height);
	m_blurVertical.allocate(m_width, m_height);
	m_colorTex.allocate(m_width, m_height, GL_RGBA);
	m_displacementTex.allocate(m_width, m_height, GL_RGB32F_ARB);
	m_depthTex.allocate(m_width, m_height, GL_RGBA);

	CreateCullShader();
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
	m_shader->load("shaders/mainVert.glsl", "shaders/mainFrag.glsl", "shaders/default.geom");
	m_normalShader->load("shaders/default.vert", "shaders/normalmap.frag");
	m_blurShader->load("shaders/default.vert", "shaders/blurFrag.glsl");
	m_depthShader->load("shaders/default.vert", "shaders/depthFrag.glsl");
	m_shaderSetup = true;
}

void TestEffect::preDraw() 
{
	if (!m_shaderSetup) 
	{
		setupShader();
	}

	//ofEnableAlphaBlending();
	//m_normalShader->begin();
	//texture.loadData(m_parent->getKinectData().m_depthStream);
	//texture.draw(1024-400, 768-200, 200, 200);
	//m_normalShader->end();
}

void TestEffect::postDraw()
{

}

void TestEffect::draw()
{
	KinectController::KinectInterfacePtr kinectInterface = m_parent->getKinectController()->getKinect();

	/*
	ofImage image;
	image.loadImage("images/normal_map_1.jpg");
	//image.draw(1024-200, 768-200, 200, 200);
	//image.loadImage("images/lines.jpg");

	ofImage image2;
	image2.loadImage("images/height_map_1.jpg");

	ofImage image3;
	image3.loadImage("images/base_1.jpg");
	*/

	m_displacementTex.loadData(m_parent->getKinectController()->getKinect()->getDistancePixelsRef());
	m_depthTex.loadData(m_parent->getKinectData().m_depthStream);

	int blurSize = 7;
	int sigma = 3;
	float kernelRes = 21.f;
	float invKernelRes = 1.f/kernelRes;

	m_fbo.begin();
	//ofEnableAlphaBlending();
	m_normalShader->begin();
	m_normalShader->setUniform1f("xOffset", 1.f/m_width);
	m_normalShader->setUniform1f("yOffset", 1.f/m_height);
	//texture.loadData(m_parent->getKinectController()->getKinect()->getDistancePixelsRef());
	//texture.draw(0, 0);
	//m_blurHorizontal.draw(0, 0);
	m_displacementTex.draw(0, 0);
	//m_depthTex.draw(0, 0);
	m_normalShader->end();
	//ofClearAlpha();
	m_fbo.end();

	ofImage blurKernel;
	blurKernel.loadImage("images/kernel.png");

	// load displacement map
	// vertical blur pass
	m_blurVertical.begin();
	//ofEnableAlphaBlending();
	m_blurShader->begin();
	m_blurShader->setUniform1i("horizontalPass", 0);
	m_blurShader->setUniform1i("blurSize", blurSize);
	m_blurShader->setUniform1f("sigma", sigma);
	m_blurShader->setUniform1f("pixelSize", 1.f/m_blurVertical.getHeight());
	//m_blurShader->setUniformTexture("fboTex", m_fbo.getTextureReference(), 0);
	m_blurShader->setUniformTexture("kernelTex", blurKernel.getTextureReference(), 1);
	m_blurShader->setUniform1f("kernelRes", kernelRes);
	m_blurShader->setUniform1f("invKernelRes", invKernelRes);
	m_blurShader->setUniform2f("orientationVector", 0.f, 1.f/m_fbo.getHeight());
	m_blurShader->setUniform1f("blurAmt", m_blurFactor);

	//m_displacementTex.draw(0, 0);
	m_fbo.draw(0, 0);
	m_blurShader->end();
	//ofClearAlpha();
	m_blurVertical.end();

	// horizontal pass
	m_blurHorizontal.begin();
	//ofEnableAlphaBlending();
	glPushAttrib(GL_ALL_ATTRIB_BITS);  
    glEnable(GL_BLEND);  
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	m_blurShader->begin();
	m_blurShader->setUniform1i("horizontalPass", 1);
	m_blurShader->setUniform1i("blurSize", blurSize);
	m_blurShader->setUniform1f("sigma", sigma);
	m_blurShader->setUniform1f("pixelSize", 1.f/m_blurHorizontal.getWidth());
	//m_blurShader->setUniformTexture("fboTex", m_blurVertical.getTextureReference(), 0);
	m_blurShader->setUniformTexture("kernelTex", blurKernel.getTextureReference(), 1);
	m_blurShader->setUniform1f("kernelRes",kernelRes);
	m_blurShader->setUniform1f("invKernelRes", invKernelRes);
	m_blurShader->setUniform2f("orientationVector", 1.f/m_blurVertical.getWidth(), 0.f);
	m_blurShader->setUniform1f("blurAmt", m_blurFactor);
	m_blurVertical.draw(0, 0);
	m_blurShader->end();
	glDisable(GL_BLEND);
	glPopAttrib();
	//ofClearAlpha();
	m_blurHorizontal.end();

	ofEnableAlphaBlending();
	m_blurHorizontal.draw(1024-200, 768-200, 200, 200);
	ofDisableAlphaBlending();

	//m_blurHorizontal.draw(1024-200, 768-200, 200, 200);
	//m_displacementTex.draw(0, 0);
	//ofSaveScreen("images/test.png");
	
	//ofTexture texture;
	//texture.allocate(m_width, m_height, GL_LUMINANCE32F_ARB);
	// draw normal map first into FBO

	//m_fbo.draw(1024-200, 768-200, 200, 200);

	/*
	blurColor1.allocate(m_width, m_height);
	blurColor1.begin();
	//ofEnableAlphaBlending();
	m_blurShader->begin();
	m_blurShader->setUniform1i("horizontalPass", 0);
	m_blurShader->setUniform1i("blurSize", blurSize);
	m_blurShader->setUniform1f("sigma", sigma);
	m_blurShader->setUniform1f("pixelSize", 1.f/m_blurVertical.getHeight());
	//m_displacementTex.draw(0, 0);
	m_colorTex.loadData(m_parent->getKinectData().m_videoStream);
	m_colorTex.draw(0, 0);
	m_blurShader->end();
	//ofClearAlpha();
	blurColor1.end();

	blurColor2.allocate(m_width, m_height);
	// horizontal pass
	blurColor2.begin();
	//ofEnableAlphaBlending();
	m_blurShader->begin();
	m_blurShader->setUniform1i("horizontalPass", 1);
	m_blurShader->setUniform1i("blurSize", blurSize);
	m_blurShader->setUniform1f("sigma", sigma);
	m_blurShader->setUniform1f("pixelSize", 1.f/m_blurHorizontal.getWidth());
	blurColor1.draw(0, 0);
	m_blurShader->end();
	//ofClearAlpha();
	blurColor2.end();
	*/

	ofFbo depthFbo;
	depthFbo.allocate(m_width, m_height);
	depthFbo.begin();
	//ofEnableAlphaBlending();
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glEnable(GL_BLEND);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	m_depthShader->begin();
	m_depthShader->setUniform1f("scale", 1.f);
	m_depthShader->setUniformTexture("video_tex", m_colorTex, 1);
	m_depthShader->setUniformTexture("depth_tex", m_depthTex, 2);
	m_colorTex.draw(0, 0);
	//glDisable(GL_BLEND);
	//glPopAttrib();
	//ofClearAlpha();
	m_depthShader->end();
	depthFbo.end();

	ofEnableAlphaBlending();
	depthFbo.draw(1024-400, 768-200, 200, 200);
	ofDisableAlphaBlending();

	easyCam.begin();
	easyCam.getPosition();
	ofPushMatrix();
	ofTranslate(0, 0, -100);
	ofScale(1, -1, 1);
	glTranslatef(-m_width*.5, -m_height*.5, 0);

	m_colorTex.loadData(m_parent->getKinectData().m_videoStream);

	/*
	cullShader->begin();
	cullShader->setUniform1f("cullingValue", 1.f - m_cullingValue);
	cullShader->setUniform1i("cullBg", 0);
	cullShader->setUniformTexture("depth_tex", depthFbo.getTextureReference(), 1);
	m_colorTex.draw(0, 0);
	cullShader->end();
	*/

	
	m_shader->begin();
	ofVec3f eyePos = easyCam.getPosition();
	m_shader->setUniform3f("eyePos", eyePos.x, eyePos.y, eyePos.z);
	m_shader->setUniform1f("clip", m_clipValue);
	m_shader->setUniform1f("chub_factor", m_chubFactor);
	m_shader->setUniform1f("near_depth", m_nearDepth);
	m_shader->setUniform1f("far_depth", m_farDepth);
	m_shader->setUniform1f("cullingValue", m_cullingValue);
	//m_shader->setUniformTexture("color_tex", depthFbo.getTextureReference(), 1);
	m_shader->setUniformTexture("normal_tex", m_blurHorizontal.getTextureReference(), 2);
	m_shader->setUniformTexture("displacement_tex", m_depthTex, 3);
	m_shader->setUniformTexture("depth_tex", depthFbo.getTextureReference(), 1);
	/*
	m_shader->setUniformTexture("color_tex", image3.getTextureReference(), 1);
	m_shader->setUniformTexture("normal_tex", image.getTextureReference(), 2);
	m_shader->setUniformTexture("displacement_tex", image2.getTextureReference(), 3);
	*/	

	if (!m_drawWireframe)
	{
		m_mesh->draw();
	}
	else 
	{
		m_mesh->drawWireframe();
	}
	m_shader->end();
	
	easyCam.end();
}

void TestEffect::addUI(CanvasPtr canvas) 
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

	std::vector<std::string> radioOptions;
	radioOptions.push_back("Fill");
	radioOptions.push_back("Wireframe");
	ofxUIRadio* radio = new ofxUIRadio("Draw Mode", radioOptions, OFX_UI_ORIENTATION_VERTICAL, 10, 10);
	canvas->addWidgetDown(radio);
	m_widgets.push_back(radio);

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

	m_parent->getCanvas()->saveSettings("GUI/TestEffectSettings.xml");
}