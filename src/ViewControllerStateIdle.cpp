#include "ViewControllerStateIdle.h"
#include "ViewControllerStateStarting.h"

ViewControllerState::ViewControllerStatePtr ViewControllerStateIdle::Instance()
{
	static ViewControllerStatePtr m_instance = ViewControllerStatePtr(new ViewControllerStateIdle());
		
	return m_instance;
}

void ViewControllerStateIdle::enter(ViewController* viewController)
{
	std::cout << "View Controller State: Idle\n";

	ViewControllerState::enter(viewController);
}

void ViewControllerStateIdle::execute()
{
	if(m_viewController->shouldStart())
	{
		m_viewController->changeState(ViewControllerStateStarting::Instance());
	}
}

void ViewControllerStateIdle::exit()
{
	m_viewController->updateGroupId();
}