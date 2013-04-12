#ifndef BOOF_VIEW_CONTROLLER_STATE_STARTING
#define BOOF_VIEW_CONTROLLER_STATE_STARTING

#include "ViewControllerState.h"

class ViewControllerStateStarting : public ViewControllerState {
public:
	static ViewControllerStatePtr Instance();

	virtual void enter(ViewController* viewController);
	// Once transition is finished change state to run effect
	virtual void execute();
	virtual void exit();
};

#endif