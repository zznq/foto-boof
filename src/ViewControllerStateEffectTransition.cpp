#include "ViewControllerStateEffectTransition.h"
#include "ViewControllerStateRunEffect.h"

ViewControllerState::ViewControllerStatePtr ViewControllerStateEffectTransition::Instance()
{
	static ViewControllerStatePtr m_instance = ViewControllerStatePtr(new ViewControllerStateEffectTransition());
	
	return m_instance;
}

void ViewControllerStateEffectTransition::enter(ViewController* viewController)
{
	std::cout << "View Controller State: Effect Transition\n";

	ViewControllerState::enter(viewController);

	// Load Effect data into overlay, start transitioning into next effect
	// Load Effect data into image, save to web server
	// Iterate to next effect
	m_viewController->handleViewAction(ViewAction::TRANSITION_STARTED);
	m_viewController->sharePhoto();

	is_pastHalfWay = false;
}

void ViewControllerStateEffectTransition::execute()
{
	if(m_viewController->isTransitionHalfWay() && !is_pastHalfWay)
	{
		is_pastHalfWay = true;
		m_viewController->incrementView();

		if(!m_viewController->isLastEffect())
		{
			m_viewController->getOverlayView()->setTimer(m_viewController->getCurrentView()->getViewInterval());
		}
		else
		{
			m_viewController->handleViewAction(ViewAction::PRINTING);
		}
	}

	// Once transition is finished
	if(m_viewController->isTransitionFinished())
	{
		// If we are done print out images
		if(m_viewController->isLastEffect())
		{
			m_viewController->changeState(ViewControllerStatePrint::Instance());
		}
		else
		{
			// Or Run next effect
			m_viewController->changeState(ViewControllerStateRunEffect::Instance());
		}
	}
}

void ViewControllerStateEffectTransition::exit()
{
	m_viewController->getOverlayView()->stopEffectTransition();
}