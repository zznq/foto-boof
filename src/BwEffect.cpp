#include "ofMain.h"
#include "ofColor.h"

#include "BwEffect.h"
#include "View.h"
#include "KinectData.h"


BwEffect::BwEffect()
: VisualEffect("bw_effect")
{ }

BwEffect::~BwEffect()
{
	if(m_event)
	{
		ofRemoveListener(*m_event, this,&BwEffect::guiEvent);
	}
}

void BwEffect::draw()
{
	if (m_parent)
	{
		float grayScale = 0.f;
		ofPixels& pixels = m_parent->getKinectData().m_videoStream;
		for (int i=0; i < pixels.getWidth(); ++i)
		{
			for (int j=0; j < pixels.getHeight(); ++j)
			{
				ofColor color = pixels.getColor(i, j);
				grayScale = float(color.r * 0.2989 + color.g * 0.5870 + color.b * 0.11);
				pixels.setColor(i, j, grayScale);
			}
		}
	}
}

void BwEffect::addUI(CanvasPtr canvas)
{
	ofxUISlider* slider = new ofxUISlider("BGR", 0.0f, 255.0f, 50.0f, 50.0f, 50.0f);
	canvas->addWidgetDown(slider);
	m_widgets.push_back(slider);

	m_event = &canvas->newGUIEvent;
	
	ofAddListener(*m_event,this,&BwEffect::guiEvent);
}

void BwEffect::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 

	if(name == "BGR")
	{
		std::cout << name << " " << kind << endl;	
	}  
}