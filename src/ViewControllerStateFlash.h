#ifndef BOOF_VIEW_CONTROLLER_STATES_FLASH
#define BOOF_VIEW_CONTROLLER_STATES_FLASH

#include "ViewControllerState.h"

class ViewControllerStateFlash : public ViewControllerState {
public:
	static ViewControllerStatePtr Instance();

	virtual void enter(ViewController* viewController);
	virtual void execute();
	virtual void exit();
};

#endif