#ifndef BOOF_VIEW_CONTROLLER_STATES_IDLE
#define BOOF_VIEW_CONTROLLER_STATES_IDLE

#include "ViewControllerState.h"

class ViewControllerStateIdle : public ViewControllerState {
public:
	static ViewControllerStatePtr Instance();

	virtual void enter(ViewController* viewController);
	virtual void execute();
	virtual void exit();
};

#endif