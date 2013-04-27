#ifndef BOOF_BACKGROUND_IMAGE_ELEMENT
#define BOOF_BACKGROUND_IMAGE_ELEMENT

#include "ImageElement.h"

class BackgroundImageElement : public ImageElement
{
public:
	BackgroundImageElement() : ImageElement(ofImage("graphics/background.png"), 0)
	{
		m_image.allocate(1280, 720, ofImageType::OF_IMAGE_COLOR);
	}
};

#endif