#include "VisualEffect.h"

VisualEffect::VisualEffect(const std::string& name) 
: m_name(name), m_parent(0) 
{
	m_widgets = Widgets();
}

VisualEffect::~VisualEffect() 
{

}

void VisualEffect::addUI(CanvasPtr canvas) {
}

void VisualEffect::removeUI(CanvasPtr canvas) {
}

const std::string& VisualEffect::getName() 
{ 
	return m_name; 
}

void VisualEffect::setParent(View* parent) 
{ 
	m_parent = parent;

	addUI(m_parent->getCanvas());

	m_parent->getCanvas()->autoSizeToFitWidgets();
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