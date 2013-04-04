#ifndef BOOF_FATSUITVIEW
#define BOOF_FATSUITVIEW

#include "ofFbo.h"

#include "View.h"
#include "KinectController.h"
#include "NormalMapEffect.h"
#include "FatSuitEffect.h"

class FatSuitView : public View {
public:
	FatSuitView(KinectControllerPtr kinectController, int width, int height);
	~FatSuitView();
	virtual void draw();

protected:
	ofFbo m_fbo;
};

#endif
