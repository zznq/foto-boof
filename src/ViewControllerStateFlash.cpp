#include "ViewControllerStateFlash.h"
#include "ViewControllerStateEffectTransition.h"

ViewControllerState::ViewControllerStatePtr ViewControllerStateFlash::Instance()
{
	static ViewControllerStatePtr m_instance = ViewControllerStatePtr(new ViewControllerStateFlash());
		
	return m_instance;
}

void ViewControllerStateFlash::enter(ViewController* viewController)
{
	std::cout << "View Controller State: Flash\n";

	ViewControllerState::enter(viewController);
	
	m_viewController->getOverlayView()->startFlash();
}

void ViewControllerStateFlash::execute()
{
	if(m_viewController->isFlashFinished())
	{
		m_viewController->changeState(ViewControllerStateEffectTransition::Instance());
	}
}
	
void ViewControllerStateFlash::exit()
{
	m_viewController->getOverlayView()->stopFlash();
}