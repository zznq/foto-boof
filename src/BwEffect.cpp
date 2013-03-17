#include "ofColor.h"

#include "BwEffect.h"

//I am not sure how to use the CaptureStream_RGB enum without this include but I know this will result in this file
// being included way too much.
#include "KinectController.h"

BwEffect::BwEffect()
: PixelEffect("bw_effect", CaptureStream_RGB)
{

}

void BwEffect::apply(ofPixels& pixels) {
	float grayScale = 0.f;
	for (int i=0; i < pixels.getWidth(); ++i) {
		for (int j=0; j < pixels.getHeight(); ++j) {
			ofColor color = pixels.getColor(i, j);
			grayScale = float(color.r * 0.2989 + color.g * 0.5870 + color.b * 0.11);
			pixels.setColor(i, j, grayScale);
		}
	}
}