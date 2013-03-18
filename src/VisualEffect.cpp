#include "VisualEffect.h"
#include "View.h"

VisualEffect::VisualEffect(const std::string& name) 
: m_name(name), m_parent(0) 
{

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

void VisualEffect::preDraw() 
{

}

void VisualEffect::draw() 
{

}

void VisualEffect::postDraw()
{

}