#ifndef BOOF_TESTVIEW
#define BOOF_TESTVIEW

#include "View.h"
#include "KinectController.h"
#include "OutlineEffect.h"
#include "PointCloudEffect.h"
#include "Test.h"

class TestView : public View {
public:
	TestView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true) {
		//addEffect(View::VisualEffectPtr(new OutlineEffect(width, height)));
		//addEffect(View::VisualEffectPtr(new PointCloudEffect(width, height)));
		addEffect(View::VisualEffectPtr(new TestEffect(width, height)));

	}

protected:
	virtual void doViewDraw() { }
};

#endif