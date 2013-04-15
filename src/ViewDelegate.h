#ifndef BOOF_VIEW_DELEGATE
#define BOOF_VIEW_DELEGATE

struct ViewAction {
	enum Enum {
		NONE,
		STARTING,
		EFFECT_COUNTDOWN_FINISHED,
		FLASH_FINISHED,
		TRANSITION_FINISHED
	};
};

// View Lifecylce abstract class
class ViewDelegate {
public:
	virtual void handleViewAction(const ViewAction::Enum& action) {};
};

#endif