#ifndef BW_EFFECT_H
#define BW_EFFECT_H

#include "PixelEffect.h"

// converts to black & white
class BwEffect: public PixelEffect {
public:
	BwEffect();

	virtual void apply(ofPixels& pixels);
};

#endif