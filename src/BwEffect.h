#ifndef BW_EFFECT_H
#define BW_EFFECT_H

#include "VisualEffect.h"

// converts to black & white
class BwEffect: public VisualEffect {
public:
	BwEffect();
	~BwEffect();

	virtual void draw();

	virtual void addUI(CanvasPtr canvas);

	virtual void guiEvent(ofxUIEventArgs &e);
};

#endif