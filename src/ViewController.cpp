#include <string.h>

#include "ViewController.h"
#include "KinectController.h"
#include "ViewFactory.h"

#include "ViewControllerStateIdle.h"

int ViewController::VIEW_WIDTH = 640; //933;
int ViewController::VIEW_HEIGHT = 480; //700;

ViewController::ViewController() :
	m_currentState(ViewControllerStateIdle::Instance()), m_viewGroupId(0), m_shouldStart(false),
	m_isEffectCountdownFinished(false), m_isFlashFinished(false), m_isTransitionFinished(false),
	m_lastView(false)
{ 
	m_currentState->enter(this);
}

ViewController::~ViewController()
{ }

//ViewPtr CreateView(const ViewType::Enum& type, const KinectControllerPtr& kinectController, int width, int height);

void ViewController::setup(KinectControllerPtr kinectController)
{
	ViewType::Enum views[] = 
	{ 
		ViewType::Enum::Idle,
		ViewType::Enum::BwShaderView,
		ViewType::Enum::ColorDepthShaderView,
		ViewType::Enum::FatSuitView 
	};

	// add all view types
	for (int i=0; i < 4; ++i) 
	{
		ViewPtr view = CreateView(views[i], kinectController, VIEW_WIDTH, VIEW_HEIGHT);
		//ViewPtr view = CreateView(ViewType::MeshView, kinectController, kinectController->getDataWidth(), kinectController->getDataHeight());
		
		view->setViewDelegate(View::ViewDelegatePtr(this));
		
		addView(view);
	}

	m_viewsIterator = m_views.begin();
	getCurrentView()->setup();

	// This needs to be the entire width of the screen
	m_overlayView = OverlayViewPtr(new OverlayView(kinectController, ofGetWindowWidth(), ofGetWindowHeight()));
	m_overlayView->setViewDelegate(View::ViewDelegatePtr(this));
	
	m_screen.allocate(VIEW_WIDTH, VIEW_HEIGHT, OF_IMAGE_COLOR_ALPHA);
}

void ViewController::update(float delta)
{
	m_currentState->execute();

	this->getCurrentView()->update(delta);
	this->getOverlayView()->update(delta);
}

void ViewController::draw()
{
	this->getCurrentView()->draw();
	this->getOverlayView()->draw();
}

void ViewController::updateGroupId()
{
	m_viewGroupId++;
}

void ViewController::sharePhoto()
{
	std::ostringstream oss;

	oss << "data/images/" << m_viewGroupId;

	m_screen.grabScreen(0, 0, m_screen.getWidth(), m_screen.getHeight());
	
	std::string path = oss.str();

	ofDirectory dir = ofDirectory(path);
	
	if(!dir.exists()){
		dir.create(true);
	}

	dir.listDir();

	oss << "/" << (dir.size() + 1) << ".jpg";
	std::string filePath = oss.str();

	m_screen.saveImage(filePath);

	//TODO: 
	//	Get screen data from overlay
	//	Save photo to disk
	//	Send photo to web server
}

void ViewController::printPhotoStrip()
{
	//TODO:
	//	Composite a photostrip from the 4 photos taken
	//	Print Composite
	//	Send COmposite to web server
}

void ViewController::changeState(const ViewControllerStatePtr state)
{
	m_currentState->exit();

	m_currentState = state;

	m_currentState->enter(this);
}

bool ViewController::shouldStart() const
{
	return m_shouldStart;
}

bool ViewController::isEffectCountdownFinished() const
{
	return m_isEffectCountdownFinished;
}

bool ViewController::isFlashFinished() const
{
	return m_isFlashFinished;
}

bool ViewController::isTransitionFinished() const
{
	return m_isTransitionFinished;
}

bool ViewController::isLastEffect() const
{
	return m_lastView;
}

void ViewController::handleViewAction(const ViewAction::Enum& action)
{
	std::cout << "ViewAction Triggered: ";

	switch(action) {
	case ViewAction::STARTING:
		std::cout << "STARTING" << std::endl;
		m_shouldStart = true;
		break;
	case ViewAction::EFFECT_COUNTDOWN_FINISHED:
		std::cout << "EFFECT_COUNTDOWN_FINISHED" << std::endl;
		m_isTransitionFinished = false;
		m_isEffectCountdownFinished = true;
		break;
	case ViewAction::FLASH_FINISHED:
		std::cout << "FLASH_FINISHED" << std::endl;
		m_isFlashFinished = true;
		break;
	case ViewAction::TRANSITION_FINISHED:
		std::cout << "TRANSITION_FINISHED" << std::endl;
		if(m_shouldStart) {
			m_shouldStart = false;
		}
		m_isEffectCountdownFinished = false;
		m_isFlashFinished = false;
		m_isTransitionFinished = true;
		break;
	}
}

void ViewController::addView(const ViewPtr& view)
{
	m_views.push_back(view);
}

ViewController::ViewPtr ViewController::getCurrentView()
{
	return (* m_viewsIterator);
}

ViewController::OverlayViewPtr ViewController::getOverlayView()
{
	return m_overlayView;
}

void ViewController::incrementView()
{
	this->getCurrentView()->close();
	
	m_viewsIterator++;

	if(m_viewsIterator == m_views.end()) {
		m_viewsIterator = m_views.begin();
		m_lastView = true;
	} else {
		m_lastView = false;
	}

	this->getCurrentView()->setup();
}