#include "View.h"
#include "KinectController.h"

View::View(KinectControllerPtr kinectController, int width, int height) 
: m_kinectController(kinectController),  m_width(width), m_height(height), m_timeInterval(5000) 
{
	m_texture.allocate(m_kinectController->getDataWidth(), m_kinectController->getDataHeight(), GL_RGB);
}

View::~View() {
	m_texture.clear();
}

void View::update() {

}

void View::draw() {
	VisualEffects::iterator iter;
	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->preDraw();
	}

	// apply each effect to the kinect data
	// NOTE: this will modify the data in place, not copy
	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->draw();
	}

	m_texture.loadData(getKinectData().m_videoStream);
	m_texture.draw(0, 0, m_width, m_height);

	for (iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		(*iter)->postDraw();
	}
}

void View::addEffect(const VisualEffectPtr& effect) 
{
	effect->setParent(this);
	m_effects.push_back(effect);
}

void View::clearEffects() {
	m_effects.clear();
}

void View::removeEffect(const std::string& effectName)
{
	for (VisualEffects::iterator iter = m_effects.begin(); iter != m_effects.end(); ++iter) {
		if ((*iter)->getName() == effectName) {
			m_effects.erase(iter);
			break;
		}
	}
}

int View::getViewInterval() {
	return m_timeInterval;
}

KinectData View::getKinectData() const {
	return m_kinectController->getKinectData();
}