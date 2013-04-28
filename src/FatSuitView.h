#ifndef BOOF_FATSUITVIEW
#define BOOF_FATSUITVIEW

#include "View.h"
#include "KinectController.h"
#include "OutlineEffect.h"
#include "PointCloudEffect.h"
#include "CloudTest.h"
#include "MeshEffect.h"
#include "MeshTest.h"
#include "FatSuitEffect.h"

class FatSuitView : public View {
public:
	FatSuitView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true) {
		addEffect(View::VisualEffectPtr(new FatSuitEffect(width, height, false, false)));
	}

	void setup()
	{
		m_canvas->setVisible(false);
	}
protected:
	virtual void doViewDraw() { }
};

#endif