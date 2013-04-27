#include "ViewControllerStateStarting.h"
#include "ViewControllerStateRunEffect.h"

ViewControllerState::ViewControllerStatePtr ViewControllerStateStarting::Instance()
{
	static ViewControllerStatePtr m_instance = ViewControllerStatePtr(new ViewControllerStateStarting());
		
	return m_instance;
}

void ViewControllerStateStarting::enter(ViewController* viewController)
{
	std::cout << "View Controller State: Starting\n";

	ViewControllerState::enter(viewController);

	m_viewController->handleViewAction(ViewAction::TRANSITION_STARTED);
	m_viewController->handleViewAction(ViewAction::STARTED);
	is_pastHalfWay = false;
}

// Once transition is finished change state to run effect
void ViewControllerStateStarting::execute()
{
	if(m_viewController->isTransitionHalfWay() && !is_pastHalfWay)
	{
		m_viewController->incrementView();

		m_viewController->getOverlayView()->setTimer(m_viewController->getCurrentView()->getViewInterval());

		is_pastHalfWay = true;
	}

	if(m_viewController->isTransitionFinished())
	{
		m_viewController->changeState(ViewControllerStateRunEffect::Instance());
	}
}
	
void ViewControllerStateStarting::exit()
{
	m_viewController->getOverlayView()->stopEffectTransition();
}