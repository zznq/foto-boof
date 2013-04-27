#include "ViewControllerStatePrint.h"
#include "ViewControllerStateIdle.h"

ViewController::ViewControllerStatePtr ViewControllerStatePrint::Instance()
{
	static ViewControllerStatePtr m_instance = ViewControllerStatePtr(new ViewControllerStatePrint());
		
	return m_instance;
}

void ViewControllerStatePrint::enter(ViewController* viewController)
{
	std::cout << "View Controller State: Printing\n";

	ViewControllerState::enter(viewController);

	m_viewController->printPhotoStrip();
}

void ViewControllerStatePrint::execute()
{
	// Go Back to IDLE state
	if(m_viewController->isPrintingFinished())
	{
		m_viewController->changeState(ViewControllerStateIdle::Instance());
	}
}

void ViewControllerStatePrint::exit()
{
	//m_viewController->incrementView();
}