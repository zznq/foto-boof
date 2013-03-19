#include "VisualEffect.h"

VisualEffect::VisualEffect(const std::string& name) 
: m_name(name), m_parent(0) 
{
	m_widgets = Widgets();
}

VisualEffect::~VisualEffect() 
{

}

const std::string& VisualEffect::getName() 
{ 
	return m_name; 
}

void VisualEffect::setParent(View* parent) 
{ 
	m_parent = parent; 
}


void VisualEffect::addUI(CanvasPtr canvas)
{
	Widgets::iterator iter;
	for (iter = m_widgets.begin(); iter != m_widgets.end(); ++iter) {
		canvas->addWidgetDown(*iter);
	}

	//ofAddListener(canvas->newGUIEvent,this,&VisualEffect::guiEvent);
}

void VisualEffect::removeUI(CanvasPtr canvas)
{
	Widgets::iterator iter;
	for (iter = m_widgets.begin(); iter != m_widgets.end(); ++iter) {
		canvas->removeWidget(*iter);
	}
}

void VisualEffect::preDraw() 
{

}

void VisualEffect::draw() 
{

}

void VisualEffect::postDraw()
{

}

void VisualEffect::guiEvent(ofxUIEventArgs &e)
{

}