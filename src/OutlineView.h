#ifndef BOOF_OUTLINE_VIEW_H
#define BOOF_OUTLINE_VIEW_H

#include "View.h"
#include "KinectController.h"
#include "OutlineEffect.h"

class OutlineView : public View {
public:
	OutlineView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height) {
		addEffect(View::VisualEffectPtr(new OutlineEffect(width, height)));
	}

protected:
	void doViewDraw() { }
};

#endif