#include <string.h>

#include "ViewController.h"
#include "KinectController.h"
#include "ViewFactory.h"

#include "ViewControllerStateIdle.h"

int ViewController::VIEW_WIDTH = 933;
int ViewController::VIEW_HEIGHT = 700;

std::string ViewController::IMAGE_PATH = "data/images/";

ViewController::ViewController() :
	m_currentState(ViewControllerStateIdle::Instance()), m_viewGroupId(0), m_shouldStart(false),
	m_isEffectCountdownFinished(false), m_isFlashFinished(false), m_isTransitionHalfWay(false),
	m_isTransitionFinished(false), m_lastView(false), m_isPrinting(false)
{ 
	m_currentState->enter(this);

	initializeGroupId();
}

ViewController::~ViewController()
{ }

//ViewPtr CreateView(const ViewType::Enum& type, const KinectControllerPtr& kinectController, int width, int height);

void ViewController::setup(KinectControllerPtr kinectController)
{
	ViewType::Enum views[] = 
	{
		ViewType::Enum::Idle,
		ViewType::Enum::FatSuitView,
		ViewType::Enum::ColorDepthShaderView,
		ViewType::Enum::FatSuitWireView
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
	m_overlayView = OverlayViewPtr(new OverlayView(kinectController, ofGetWindowWidth(), ofGetWindowHeight(), m_views.size()));
	m_overlayView->setViewDelegate(View::ViewDelegatePtr(this));
	m_overlayView->setup();
	
	m_screen.allocate(VIEW_WIDTH, VIEW_HEIGHT, OF_IMAGE_COLOR_ALPHA);
}

void ViewController::update(float delta)
{
	m_currentState->execute();

	if(!m_isPrinting)
	{
		this->getCurrentView()->update(delta);
	}

	this->getOverlayView()->update(delta);
}

void ViewController::draw()
{
	this->getOverlayView()->draw();

	if(!m_isPrinting)
	{
		this->getCurrentView()->draw();
	}

	this->getOverlayView()->drawForeground();
}

void ViewController::updateGroupId()
{
	m_viewGroupId++;
}

void ViewController::sharePhoto()
{
	std::ostringstream oss;

	oss << ViewController::IMAGE_PATH << m_viewGroupId;

	std::string path = oss.str();

	ofDirectory dir = ofDirectory(path);
	
	if(!dir.exists()){
		dir.create(true);
	}

	dir.listDir();

	oss << "/" << (dir.size() + 1) << ".jpg";
	std::string filePath = oss.str();

	this->getOverlayView()->getScreen().saveImage(filePath);
}

void ViewController::initializeGroupId()
{
	ofDirectory dir = ofDirectory(ViewController::IMAGE_PATH);

	if(dir.exists())
	{
		dir.listDir();

		m_viewGroupId = dir.size();
	}
	else
	{
		m_viewGroupId = 0;
	}

}

void ViewController::printPhotoStrip()
{
	static ofRectangle print_size = ofRectangle(0, 0, 288, 432);
	std::ostringstream oss;

	oss << ViewController::IMAGE_PATH << m_viewGroupId;

	std::string path = oss.str();

	ofDirectory dir = ofDirectory(path);

	if(!dir.exists())
	{
		return;
	}

	dir.listDir();

	oss << "/strip.jpg";
	std::string filePath = oss.str();

	std::vector<ofFile, allocator<ofFile>> files = dir.getFiles();

	ofImage tex = ofImage();
	tex.allocate(print_size.getWidth(), print_size.getHeight(), ofImageType::OF_IMAGE_COLOR);

	ofImage background = ofImage("graphics/print_background.png");
	background.allocate(print_size.getWidth(), print_size.getHeight(), ofImageType::OF_IMAGE_COLOR);

	ofFbo fbo;
	ofCamera cam;
	fbo.allocate(print_size.getWidth(), print_size.getHeight(), GL_RGB);
	cam.begin();
	cam.setupPerspective();

		
	fbo.begin();
	ofPushMatrix();

	ofScale(1, -1, 1);
	ofTranslate(0, -print_size.getHeight());

	int x = 10;
	int y = 10;
		
	background.draw(0, 0, print_size.getWidth(), print_size.getHeight());
	for (std::vector<ofFile, allocator<ofFile>>::iterator iter = files.begin(); iter != files.end(); ++iter) {
		std::string path = (*iter).getAbsolutePath();
		ofImage image(path);

		image.draw(x, y, 175, 131);
		y += 141;
	}
	ofPopMatrix();

	tex.grabScreen(0, 0, print_size.getWidth(), print_size.getHeight());

	fbo.end();
	cam.end();

	tex.saveImage(filePath);
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

bool ViewController::isPrintingFinished() const
{
	return m_isPrintingFinished;
}

bool ViewController::isEffectCountdownFinished() const
{
	return m_isEffectCountdownFinished;
}

bool ViewController::isFlashFinished() const
{
	return m_isFlashFinished;
}

bool ViewController::isTransitionHalfWay() const
{
	return m_isTransitionHalfWay;
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
		m_isTransitionFinished = false;
		m_shouldStart = true;
		m_overlayView->starting();
		break;
	case ViewAction::STARTED:
		std::cout << "STARTING" << std::endl;
		m_overlayView->started();
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
	case ViewAction::TRANSITION_STARTED:
		std::cout << "TRANSITION_STARTED" << std::endl;
		m_isTransitionFinished = false;
		m_isTransitionHalfWay = false;
		m_overlayView->startEffectTransition();
		break;
	case ViewAction::TRANSITION_HALF_WAY:
		std::cout << "TRANSITION_HALF_WAY" << std::endl;
		m_isTransitionHalfWay = true;
		break;
	case ViewAction::TRANSITION_FINISHED:
		std::cout << "TRANSITION_FINISHED" << std::endl;
		if(m_shouldStart) {
			m_shouldStart = false;
		}
		m_isTransitionHalfWay = false;
		m_isEffectCountdownFinished = false;
		m_isFlashFinished = false;
		m_isTransitionFinished = true;
		break;
	case ViewAction::PRINTING:
		std::cout << "PRINTING" << std::endl;
		m_isPrinting = true;
		m_isPrintingFinished = false;
		m_overlayView->printing();
		break;
	case ViewAction::PRINT_FINISHED:
		std::cout << "PRINT_FINISHED" << std::endl;
		m_isPrintingFinished = true;
		m_isPrinting = false;
		m_overlayView->printed();
		break;
	default:
		std::cout << "NONE -> " << action << std::endl;
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