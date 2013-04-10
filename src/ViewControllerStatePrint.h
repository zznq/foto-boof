#ifndef BOOF_VIEW_CONTROLLER_STATE_PRINT
#define BOOF_VIEW_CONTROLLER_STATE_PRINT

#include "ViewControllerState.h"

class ViewControllerStatePrint : public ViewControllerState
{
public:
	static ViewControllerStatePtr Instance();

	virtual void enter(ViewController* viewController);
	virtual void execute();
	virtual void exit();
};

#endif