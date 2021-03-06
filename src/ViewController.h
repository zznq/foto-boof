#ifndef BOOF_VIEW_CONTROLLER
#define BOOF_VIEW_CONTROLLER

#include "boof.h"

//#include "ViewControllerState.h"
class ViewControllerState;

#include "View.h"
#include "OverlayView.h"
#include "ViewDelegate.h"

#include "ofCamera.h"

class ViewController: public ViewDelegate {
public:
	typedef ofPtr<View> ViewPtr;
	typedef ofPtr<OverlayView> OverlayViewPtr;
	typedef std::vector<ViewPtr> Views;
	typedef Views::iterator ViewsIterator;
	typedef ofPtr<ViewControllerState> ViewControllerStatePtr;
public:
	ViewController();
	~ViewController();

	void setup(KinectControllerPtr kinectController);
	void update(float delta);
	void draw();

	ViewPtr getCurrentView();
	OverlayViewPtr getOverlayView();

	void updateGroupId();
	void sharePhoto();
	void printPhotoStrip();

	bool shouldStart() const;
	bool isPrintingFinished() const;
	bool isEffectCountdownFinished() const;
	bool isFlashFinished() const;
	bool isTransitionHalfWay() const;
	bool isTransitionFinished() const;
	bool isLastEffect() const;

	void changeState(const ViewControllerStatePtr state);
	
	// ViewDelegate Overrides
	virtual void handleViewAction(const ViewAction::Enum& action);

	void incrementView();
private:
	bool m_shouldStart;
	bool m_isPrintingFinished;
	bool m_isEffectCountdownFinished;
	bool m_isFlashFinished;
	bool m_isTransitionHalfWay;
	bool m_isTransitionFinished;
	bool m_lastView;
	bool m_isPrinting;

	static int VIEW_WIDTH;
	static int VIEW_HEIGHT;
	static std::string IMAGE_PATH;

	int m_viewGroupId;

	ViewControllerStatePtr m_currentState;

	ViewPtr m_currentView;
	OverlayViewPtr m_overlayView;

	Views m_views;
	ViewsIterator m_viewsIterator;

	ofImage m_screen;

	void addView(const ViewPtr& view);

	void initializeGroupId();
};

#endif