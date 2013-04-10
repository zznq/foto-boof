#ifndef BOOF_OBSERVABLE
#define BOOF_OBSERVABLE

#include "Observer.h"

class Observable {
public:
	typedef ofPtr<Observer> ObserverPtr;
	typedef std::vector<ObserverPtr> Observers;

public:

};

#endif