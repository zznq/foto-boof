#include "MeshView.h"

MeshView::MeshView(KinectControllerPtr kinectController, int width, int height)
: View(kinectController, width, height) 
{
	addEffect(View::VisualEffectPtr(new MeshEffect(getWidth(), getHeight())));
}

void MeshView::doViewDraw()
{
	// do nothing here
}

void MeshView::doEffectsDraw()
{
	m_easyCam.begin();
	ofPushMatrix();
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -100);

	glEnable(GL_DEPTH_TEST);

	View::doEffectsDraw();

	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();

	m_easyCam.end();
}