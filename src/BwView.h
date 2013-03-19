#ifndef BOOF_BWVIEW
#define BOOF_BWVIEW

#include "View.h"
#include "KinectController.h"
#include "BwEffect.h"
#include "BwShaderEffect.h"

class BwView : public View {
public:
	BwView(KinectControllerPtr kinectController, int width, int height, CanvasPtr canvas) : View(kinectController, width, height, canvas) {
		addEffect(View::VisualEffectPtr(new BwEffect()));
	}
};

#endif