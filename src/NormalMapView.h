#ifndef BOOF_NORMALMAPVIEW
#define BOOF_NORMALMAPVIEW

#include "View.h"
#include "KinectController.h"
#include "NormalMapEffect.h"

class NormalMapView : public View {
public:
	NormalMapView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true) {
		addEffect(View::VisualEffectPtr(new NormalMapEffect(width, height)));
	}

protected:
};

#endif
