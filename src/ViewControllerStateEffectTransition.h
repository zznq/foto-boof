#ifndef BOOF_VIEW_CONTROLLER_STATE_EFFECT_TRANSITION
#define BOOF_VIEW_CONTROLLER_STATE_EFFECT_TRANSITION

#include "ViewControllerState.h"
#include "ViewControllerStatePrint.h"

class ViewControllerStateRunEffect;

class ViewControllerStateEffectTransition : public ViewControllerState {
private:
	bool is_pastHalfWay;
public:
	static ViewControllerStatePtr Instance();

	virtual void enter(ViewController* viewController);
	virtual void execute();
	virtual void exit();
};


#endif