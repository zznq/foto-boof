#include "ColorDepthShaderEffect.h"

ColorDepthShaderEffect::ColorDepthShaderEffect(const std::string effectName,
								const int width,
								const int height,
								const std::string& vertexShader,
								const std::string& fragmentShader,
								const std::string& geometryShader)
											   : ShaderEffect(effectName, vertexShader, fragmentShader, geometryShader)
{
	m_lookup = std::vector<ofPixels>(256);

	m_saturationValue = 255;
	m_brightnessValue = 255;

	buildLookUpTable();

	m_lookupTexture.allocate(256, 1, GL_RGB);
}

ColorDepthShaderEffect::~ColorDepthShaderEffect()
{
	ShaderEffect::~ShaderEffect();
}

void ColorDepthShaderEffect::buildLookUpTable() {
	for(int i = 0; i < 256; i++) {
		ofColor c;
		ofPixels p;
		c.setHsb(i, m_saturationValue, m_brightnessValue);
		p.setColor(i, 0, c);
			
		m_lookup.push_back(p);
	}
}

void ColorDepthShaderEffect::preDraw() 
{
	ShaderEffect::preDraw();

	m_lookupTexture.loadData(m_lookup[0]);
	m_shader->setUniformTexture("color_lookup", m_lookupTexture, 2);

	m_lookupTexture.draw(0, 0);
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

	m_event = &canvas->newGUIEvent;
	
	ofAddListener(*m_event,this,&ColorDepthShaderEffect::guiEvent);
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

	if(isDirty) {
		buildLookUpTable();
	}
}