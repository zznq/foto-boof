#include "Element.h"

#include "ofShader.h"
#include "ofGraphics.h"

Element::Element(float transitionLapse) : m_transitionLapse(transitionLapse), m_lapse(0)
{

}

Element::Element(float transitionLapse,
				 const std::string& vertexShader, 
				 const std::string& fragmentShader, 
				 const std::string& geometryShader) : m_transitionLapse(transitionLapse), m_lapse(0), m_shader(new ofShader())
{
	// load our shader files
	// shaders are located in data/shaders
	m_shader->load(vertexShader, fragmentShader, geometryShader);
}

Element::~Element()
{

}

void Element::begin()
{
	m_lapse = 0;
	m_isTransitioning = true;
	m_isEnding = false;
}

void Element::end()
{
	m_lapse = 0;
	m_isTransitioning = true;
	m_isEnding = true;
}

bool Element::isFinished() const
{
	return !m_isTransitioning && m_isEnding;
}

float Element::progress() const
{
	float start = 0.0f;
	float stop = 1.f;

	if(!m_isEnding) {
		start = 1.f;
		stop = 0.0f;
	}
	
	return ofLerp(start, stop, ofClamp(m_lapse / m_transitionLapse, 0, 1));
}


void Element::update(float delta)
{
	if(m_isTransitioning)
	{
		m_lapse += delta;

		if(m_lapse > m_transitionLapse)
		{
			//m_isTransitioning = false;
		}
	}
}

void Element::draw(int x, int y)
{
	ofPushMatrix();

	ofTranslate(x, y);

	if(m_isTransitioning)
	{
		preDraw();

		onDraw();

		postDraw();
	}
	else
	{
		onDraw();
	}

	ofPopMatrix();
}

void Element::onDraw()
{
}

void Element::preDraw() 
{
	if(m_shader)
	{
		m_shader->begin();
	}
}

void Element::postDraw() 
{
	if(m_shader)
	{
		m_shader->end();
	}
}

ofPtr<ofShader>& Element::getShader()
{
	return m_shader;
}