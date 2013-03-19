#ifndef BOOF_BWVIEW
#define BOOF_BWVIEW

#include "View.h"
#include "KinectController.h"
#include "BwEffect.h"

class BwView : public View {
public:
	BwView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height) {
		addEffect(View::VisualEffectPtr(new BwEffect()));
	}
};

#endif