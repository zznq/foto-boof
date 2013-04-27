#ifndef BOOF_START_SIDEBAR_IMAGE_ELEMENT
#define BOOF_START_SIDEBAR_IMAGE_ELEMENT

#include "ImageElement.h"

class StartSideBarImageElement : public ImageElement
{
public:
	StartSideBarImageElement() : ImageElement(ofImage("graphics/start_foreground.png"), 500)
	{
		m_image.allocate(337, 720, ofImageType::OF_IMAGE_COLOR_ALPHA);
	}

	StartSideBarImageElement(const std::string& vertexShader, 
							   const std::string& fragmentShader, 
							   const std::string& geometryShader) :
		ImageElement(ofImage("graphics/start_foreground.png"), 500, vertexShader, fragmentShader, geometryShader)
	{
		m_image.allocate(337, 720, ofImageType::OF_IMAGE_COLOR_ALPHA);
	}
};

#endif