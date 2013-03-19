#ifndef BW_EFFECT_H
#define BW_EFFECT_H

#include "VisualEffect.h"

// converts to black & white
class BwEffect: public VisualEffect {
public:
	BwEffect();

	virtual void draw();

	virtual void guiEvent(ofxUIEventArgs &e);
};

#endif