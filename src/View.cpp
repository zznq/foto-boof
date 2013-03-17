#include "View.h"

View::View(KinectControllerPtr kinectController) : m_kinectController(kinectController),  m_timeInterval(5000) {
}

View::~View() {
}

void View::start() {

}

void View::update() {
	m_kinectController->update();
}

void View::draw() {
	m_kinectController->draw();
}