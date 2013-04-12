#ifndef BOOF_VIEW_DELEGATE
#define BOOF_VIEW_DELEGATE

enum ViewAction;

// View Lifecylce abstract class
class ViewDelegate {
public:
	virtual void handleViewAction(ViewAction action) {};
};

#endif