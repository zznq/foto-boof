#include "ViewControllerStateRunEffect.h"
#include "ViewControllerStateFlash.h"

ViewControllerState::ViewControllerStatePtr ViewControllerStateRunEffect::Instance()
{
	static ViewControllerStatePtr m_instance = ViewControllerStatePtr(new ViewControllerStateRunEffect());
		
	return m_instance;
}

void ViewControllerStateRunEffect::enter(ViewController* viewController)
{
	std::cout << "View Controller State: Run Effect\n";
	
	ViewControllerState::enter(viewController);
	
	// Start playtime countdown on current effect
	m_viewController->getCurrentView()->startCountDown();
	m_viewController->getOverlayView()->startTimer();
}

void ViewControllerStateRunEffect::execute()
{
	// When playtime countdown is finished start taking picture
	if(m_viewController->isEffectCountdownFinished())
	{
		m_viewController->changeState(ViewControllerStateFlash::Instance());
	}
}

void ViewControllerStateRunEffect::exit()
{
	m_viewController->getCurrentView()->stopCountDown();
	m_viewController->getOverlayView()->stopTimer();
}