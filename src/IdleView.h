#ifndef BOOF_IDLEVIEW
#define BOOF_IDLEVIEW

#include "View.h"
#include "ViewController.h"
#include "KinectController.h"

// Use this class to display the color stream without timer
class IdleView : public View {
public:
	IdleView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, false)
	{
		m_canvas->setColorBack(ofColor(0.0f, 0.0F, 0.0F, 0.0f));
	}

	void update(float delta) {
		View::update(delta);

		//TODO: Add some sort of gesture recognition or button input
		startActionFired();
	}

	void startActionFired()
	{
		if(this->m_delegate)
		{
			this->m_delegate->handleViewAction(STARTING);
		}
	}
};

#endif