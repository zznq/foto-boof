#ifndef BOOF_BWVIEW
#define BOOF_BWVIEW

#include "View.h"
#include "KinectController.h"
#include "BwEffect.h"

class BwView : public View {
public:
	BwView(KinectControllerPtr kinectController) : View(kinectController) {
	}

	void start() {
		m_kinectController->clearEffects();
		m_kinectController->addEffect(KinectController::PixelEffectPtr(new BwEffect()));
	}
};

#endif