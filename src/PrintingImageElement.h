#ifndef BOOF_PRINTING_IMAGE_ELEMENT
#define BOOF_PRINTING_IMAGE_ELEMENT

#include "ImageElement.h"

class PrintingImageElement : public ImageElement
{
public:
	PrintingImageElement() : ImageElement(ofImage("graphics/printing_foreground.png"), 500)
	{
		m_image.allocate(1280, 720, ofImageType::OF_IMAGE_COLOR_ALPHA);
	}

	PrintingImageElement(const std::string& vertexShader, 
							   const std::string& fragmentShader, 
							   const std::string& geometryShader) :
		ImageElement(ofImage("graphics/printing_foreground.png"), 500, vertexShader, fragmentShader, geometryShader)
	{
		m_image.allocate(1280, 720, ofImageType::OF_IMAGE_COLOR_ALPHA);
	}
};

#endif