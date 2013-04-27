#ifndef BOOF_IMAGE_ELEMENT
#define BOOF_IMAGE_ELEMENT

#include "Element.h"
#include "ofImage.h"

class ImageElement : public Element
{
protected:
	ofImage m_image;
public:
	ImageElement(ofImage image, float transitionLapse);
	ImageElement(ofImage image,
				 float transitionLapse, 
				 const std::string& vertexShader, 
				 const std::string& fragmentShader, 
				 const std::string& geometryShader);
	~ImageElement();

	virtual void preDraw();
	virtual void onDraw();
};

#endif