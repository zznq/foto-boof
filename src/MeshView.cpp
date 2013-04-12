#include "MeshView.h"

#include "MeshEffect.h"
#include "PointCloudEffect.h"

MeshView::MeshView(KinectControllerPtr kinectController, int width, int height, bool pointCloud)
: View(kinectController, width, height), m_pointCloud(pointCloud)
{
	if (!m_pointCloud)
	{
		addEffect(View::VisualEffectPtr(new MeshEffect(getWidth(), getHeight())));
	}
	else
	{
		addEffect(View::VisualEffectPtr(new PointCloudEffect(getWidth(), getHeight())));
	}
}

void MeshView::doViewDraw()
{
	// do nothing here
}


void MeshView::doEffectsDraw()
{
	m_easyCam.begin();
	ofPushMatrix();
	//ofScale(1, -1, -1);
	//ofTranslate(0, 0, -100);
	ofScale(1,-1,-1);  
	ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);  

	glEnable(GL_DEPTH_TEST);

	View::doEffectsDraw();

	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();

	m_easyCam.end();
}
