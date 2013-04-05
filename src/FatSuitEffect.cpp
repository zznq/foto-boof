#include "NormalMapEffect.h"

#include "FatSuitEffect.h"
#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

FatSuitEffect::FatSuitEffect(int width, int height)
: ShaderEffect("fat_suit", "shaders/fatsuit.vert", "shaders/fatsuit.frag"), m_isDirty(false), m_chubFactor(1.0)
{

}

FatSuitEffect::~FatSuitEffect() 
{

}

void FatSuitEffect::preDraw() 
{
	ShaderEffect::preDraw();
	if (m_isDirty) 
	{
		m_shader->setUniform1f("chubFactor", m_chubFactor);
		m_isDirty = false;
	}
}

void FatSuitEffect::addUI(CanvasPtr canvas) 
{
	ofxUISlider* slider = new ofxUISlider("ChubFactor", 1.0f, 500.0f, m_chubFactor, 100.0f, 25.0f);
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

	if(name == "ChubFactor")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		if(m_chubFactor != slider->getScaledValue()) {
			m_chubFactor = slider->getScaledValue();

			m_isDirty = true;
		}
	}  
}