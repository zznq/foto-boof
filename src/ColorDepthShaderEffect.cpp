#include "ColorDepthShaderEffect.h"

ColorDepthShaderEffect::ColorDepthShaderEffect(const std::string effectName,
								const int width,
								const int height,
								const std::string& vertexShader,
								const std::string& fragmentShader,
								const std::string& geometryShader)
											   : ShaderEffect(effectName, vertexShader, fragmentShader, geometryShader)
{
	m_lookup = ofPixels();
	m_lookup.allocate(256, 1, OF_IMAGE_COLOR_ALPHA);

	m_lookupOffset = 0;
	m_lookupStepSize = 50;
	m_saturationValue = 255.0f;
	m_brightnessValue = 255.0f;

	m_nearClip = 500.0f;
	m_farClip = 4000.0f;

	buildLookUpTable();

	m_lookupTexture.allocate(256, 1, GL_RGB);
}

ColorDepthShaderEffect::~ColorDepthShaderEffect()
{
	ShaderEffect::~ShaderEffect();
}

void ColorDepthShaderEffect::buildLookUpTable() {
	ofColor c;
	for(int i = 0; i < 256; i++) {
		if((i % m_lookupStepSize) == 0) {
			c.setHsb((i + m_lookupOffset) % 255, m_saturationValue, m_brightnessValue);
		}

		m_lookup.setColor(i, 0, c);
	}
}

void ColorDepthShaderEffect::preDraw() 
{
	ShaderEffect::preDraw();

	m_lookupTexture.loadData(m_lookup);
	m_shader->setUniformTexture("color_lookup", m_lookupTexture, 5);
}

void ColorDepthShaderEffect::postDraw() 
{
	ShaderEffect::postDraw();
}

void ColorDepthShaderEffect::addUI(CanvasPtr canvas)
{
	ofxUISlider* slider = new ofxUISlider("Saturation", 0.0f, 255.0f, m_saturationValue, 100.0f, 25.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);
	
	ofxUISpacer* spacer = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer);
	m_widgets.push_back(spacer);

	ofxUISlider* slider2 = new ofxUISlider("Brightness", 0.0f, 255.0f, m_brightnessValue, 100.0f, 25.0f);
	canvas->addWidgetDown(slider2);
	m_widgets.push_back(slider2);

	ofxUISpacer* spacer2 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer2);
	m_widgets.push_back(spacer2);

	ofxUISlider* slider3 = new ofxUISlider("Step Size", 1.0f, 100.0f, m_lookupStepSize, 100.0f, 25.0f);
	canvas->addWidgetDown(slider3);
	m_widgets.push_back(slider3);

	ofxUISpacer* spacer5 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer5);
	m_widgets.push_back(spacer5);

	ofxUISlider* slider6 = new ofxUISlider("Offset", 0.0f, 255.0f, m_lookupOffset, 100.0f, 25.0f);
	canvas->addWidgetDown(slider6);
	m_widgets.push_back(slider6);

	ofxUISpacer* spacer3 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer3);
	m_widgets.push_back(spacer3);

	ofxUISlider* slider4 = new ofxUISlider("Near Clip", 0.0f, 500.0f, m_nearClip, 100.0f, 25.0f);
	canvas->addWidgetDown(slider4);
	m_widgets.push_back(slider4);

	ofxUISpacer* spacer4 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer4);
	m_widgets.push_back(spacer4);

	ofxUISlider* slider5 = new ofxUISlider("Far Clip", 501.0f, 8000.0f, m_farClip, 100.0f, 25.0f);
	canvas->addWidgetDown(slider5);
	m_widgets.push_back(slider5);

	m_event = &canvas->newGUIEvent;
	
	ofAddListener(*m_event,this,&ColorDepthShaderEffect::guiEvent);

	canvas->loadSettings("GUI/ColorDepthSettings.xml");
}

void ColorDepthShaderEffect::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 

	bool isDirty = false;

	if(name == "Saturation")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_saturationValue != slider->getScaledValue()) {
			m_saturationValue = slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_saturationValue << endl;

			isDirty = true;
		}
	}  

	if(name == "Brightness")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_brightnessValue != slider->getScaledValue()) {
			m_brightnessValue = slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_brightnessValue << endl;

			isDirty = true;
		}
	}

	if(name == "Step Size") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_lookupStepSize != slider->getScaledValue()) {
			m_lookupStepSize = (int) slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_lookupStepSize << endl;

			isDirty = true;
		}
	}

	if(name == "Offset") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_lookupOffset != slider->getScaledValue()) {
			m_lookupOffset = (int) slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_lookupOffset << endl;

			isDirty = true;
		}
	}
	
	if(name == "Near Clip") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_nearClip != slider->getScaledValue()) {
			m_nearClip = slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_nearClip << endl;

			m_parent->setKinectDepthClipping(m_nearClip, m_farClip);
		}
	}
	
	if(name == "Far Clip") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_farClip != slider->getScaledValue()) {
			m_farClip = slider->getScaledValue();
			std::cout << name << " " << kind << ": " << m_farClip << endl;

			m_parent->setKinectDepthClipping(m_nearClip, m_farClip);
		}
	}

	if(isDirty) {
		buildLookUpTable();
	}

	m_parent->getCanvas()->saveSettings("GUI/ColorDepthSettings.xml");
}