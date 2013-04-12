#ifndef BOOF_STATE
#define BOOF_STATE

#include "ViewController.h"

class ViewControllerState {
public:
	typedef ofPtr<ViewControllerState> ViewControllerStatePtr;
	typedef ofPtr<ViewController> ViewControllerPtr;
protected:
	ViewController* m_viewController;
public:
	ViewControllerState() { };
	~ViewControllerState() { };

	virtual void enter(ViewController* viewController) {
		m_viewController = viewController;
	}

	virtual void execute() {}
	virtual void exit() {}
};

#endif