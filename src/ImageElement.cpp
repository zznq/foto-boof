#include "ImageElement.h"
#include "ofShader.h"

ImageElement::ImageElement(ofImage image, float transitionLapse) : Element(transitionLapse), m_image(image)
{
}

ImageElement::ImageElement(ofImage image,
						   float transitionLapse,
						   const std::string& vertexShader,
						   const std::string& fragmentShader,
						   const std::string& geometryShader) : Element(transitionLapse, vertexShader, fragmentShader, geometryShader), m_image(image)
{
}

ImageElement::~ImageElement()
{
}

void ImageElement::preDraw()
{
	Element::preDraw();

	if(getShader())
	{
		getShader()->setUniformTexture("tex", m_image, 1);
		getShader()->setUniform1f("progress_value", progress());
	}
}

void ImageElement::onDraw()
{
	Element::onDraw();

	m_image.draw(0, 0);
}