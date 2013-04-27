#ifndef BOOF_TESTVIEW
#define BOOF_TESTVIEW

#include "View.h"
#include "KinectController.h"
#include "OutlineEffect.h"
#include "PointCloudEffect.h"
#include "CloudTest.h"
#include "MeshEffect.h"
#include "MeshTest.h"
#include "FatSuitEffect.h"

class TestView : public View {
public:
	TestView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true) {
		//addEffect(View::VisualEffectPtr(new OutlineEffect(width, height)));
		//addEffect(View::VisualEffectPtr(new PointCloudEffect(width, height)));
		//addEffect(View::VisualEffectPtr(new FatSuitEffect(width, height, false, false)));
		//addEffect(View::VisualEffectPtr(new CloudTest(width, height)));
		//addEffect(View::VisualEffectPtr(new MeshTest(width, height)));
	}

protected:
	virtual void doViewDraw() { }
};

#endif