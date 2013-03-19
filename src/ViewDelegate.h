#ifndef BOOF_VIEW_DELEGATE
#define BOOF_VIEW_DELEGATE

// View Lifecylce abstract class
class ViewDelegate {
public:
	virtual void viewStart() {};
	virtual void viewComplete() {};
	virtual void viewCountdownStarted() {};
};

#endif