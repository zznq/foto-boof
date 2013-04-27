#ifndef BOOF_PRIMARY_SIDEBAR_IMAGE_ELEMENT
#define BOOF_PRIMARY_SIDEBAR_IMAGE_ELEMENT

#include "ImageElement.h"

class PrimarySideBarImageElement : public ImageElement
{
public:
	PrimarySideBarImageElement() : ImageElement(ofImage("graphics/primary_sidebar.png"), 500)
	{
		m_image.allocate(337, 720, ofImageType::OF_IMAGE_COLOR_ALPHA);
	}

	PrimarySideBarImageElement(const std::string& vertexShader, 
							   const std::string& fragmentShader, 
							   const std::string& geometryShader) :
		ImageElement(ofImage("graphics/primary_sidebar.png"), 500, vertexShader, fragmentShader, geometryShader)
	{
		m_image.allocate(337, 720, ofImageType::OF_IMAGE_COLOR_ALPHA);
	}
};

#endif