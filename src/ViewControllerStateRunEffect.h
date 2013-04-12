#ifndef BOOF_VIEW_CONTROLLER_STATE_RUN_EFFECT
#define BOOF_VIEW_CONTROLLER_STATE_RUN_EFFECT

#include "ViewControllerState.h"

class ViewControllerStateRunEffect : public ViewControllerState {
public:
	static ViewControllerStatePtr Instance();

	virtual void enter(ViewController* viewController);
	virtual void execute();
	virtual void exit();
};

#endif