#include "ofPolyline.h"
#include "ofxCvContourFinder.h"
#include "ofxCvBlob.h"
#include "ofxCvGrayscaleImage.h"

#include "OutlineEffect.h"

OutlineEffect::OutlineEffect(int width, int height)
: VisualEffect("outline_effect"), m_width(width), m_height(height), m_contourFinder(new ofxCvContourFinder()),
m_nearClip(500), m_farClip(4000), m_smoothingWindow(5), m_smoothingShape(0)
{

}

OutlineEffect::~OutlineEffect()
{

}

void OutlineEffect::draw()
{
	// create gray scale image from depth stream
	ofxCvGrayscaleImage image;
	image.allocate(m_width, m_height);
	image.setFromPixels(m_parent->getKinectData().m_depthStream);

	// find blobs in image
	m_contourFinder->findContours(image, 0, m_width*m_height, 100, true);

	// loop through the blobs to create outlines of each found
	std::vector<ofxCvBlob>& blobs = m_contourFinder->blobs;
	std::vector<ofPolyline> contourPoly;
	for (int i = 0; i < blobs.size(); ++i) {
		if (blobs[i].pts.size() > 5) {
			ofPolyline tempPoly;
			tempPoly.addVertices(blobs[i].pts);
			tempPoly.setClosed(true);

			// smooth the output
			ofPolyline smoothTempPoly = tempPoly.getSmoothed(m_smoothingWindow, m_smoothingShape);

			if(!smoothTempPoly.isClosed())
			{
				smoothTempPoly.close();
			}

			contourPoly.push_back(smoothTempPoly);
		}

	}

	// draw the outlines
	for (int i=0; i < contourPoly.size(); ++i) {
		contourPoly[i].draw();
	}
}

void OutlineEffect::addUI(CanvasPtr canvas)
{
	ofxUISlider* slider1 = new ofxUISlider("Near Clip", 0.0f, 500.0f, m_nearClip, 100.0f, 25.0f);
	canvas->addWidgetDown(slider1);
	m_widgets.push_back(slider1);

	ofxUISpacer* spacer1 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer1);
	m_widgets.push_back(spacer1);

	ofxUISlider* slider2 = new ofxUISlider("Far Clip", 501.0f, 8000.0f, m_farClip, 100.0f, 25.0f);
	canvas->addWidgetDown(slider2);
	m_widgets.push_back(slider2);

	ofxUISpacer* spacer2 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer2);
	m_widgets.push_back(spacer2);

	ofxUISlider* slider3 = new ofxUISlider("Smoothing Window", 0.f, 100.f, m_smoothingWindow, 100.0f, 25.0f);
	canvas->addWidgetDown(slider3);
	m_widgets.push_back(slider3);

	ofxUISpacer* spacer3 = new ofxUISpacer(100, 2);
	canvas->addWidgetDown(spacer3);
	m_widgets.push_back(spacer3);

	ofxUISlider* slider4 = new ofxUISlider("Smoothing Shape", 0.f, 1.f, m_smoothingShape, 100.0f, 25.0f);
	canvas->addWidgetDown(slider4);
	m_widgets.push_back(slider4);

	m_event = &canvas->newGUIEvent;

	ofAddListener(*m_event,this,&OutlineEffect::guiEvent);

	canvas->loadSettings("GUI/OutlineEffectSettings.xml");
}

void OutlineEffect::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 

	if(name == "Near Clip") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_nearClip != slider->getScaledValue()) {
			m_nearClip = slider->getScaledValue();

			m_parent->setKinectDepthClipping(m_nearClip, m_farClip);
		}
	}

	if(name == "Far Clip") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_farClip != slider->getScaledValue()) {
			m_farClip = slider->getScaledValue();

			m_parent->setKinectDepthClipping(m_nearClip, m_farClip);
		}
	}

	if(name == "Smoothing Window") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_smoothingWindow != slider->getScaledValue()) {
			m_smoothingWindow = slider->getScaledValue();
		}
	}

	if(name == "Smoothing Shape") {
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		if(m_smoothingShape != slider->getScaledValue()) {
			m_smoothingShape = slider->getScaledValue();
		}
	}

	m_parent->getCanvas()->saveSettings("GUI/OutlineEffectSettings.xml");
}