#ifndef BOOF_FATSUITWIREVIEW
#define BOOF_FATSUITWIREVIEW

#include "View.h"
#include "KinectController.h"
#include "OutlineEffect.h"
#include "PointCloudEffect.h"
#include "CloudTest.h"
#include "MeshEffect.h"
#include "MeshTest.h"
#include "FatSuitEffect.h"

class FatSuitWireView : public View {
public:
	FatSuitWireView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true) {
		addEffect(View::VisualEffectPtr(new FatSuitEffect(width, height, true, true)));
	}

protected:
	virtual void doViewDraw() { }
};

#endif