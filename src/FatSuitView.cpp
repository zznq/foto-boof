
#include "FatSuitView.h"

#include "NormalMapEffect.h"
#include "FatSuitEffect.h"
#include "MeshEffect.h"

FatSuitView::FatSuitView(KinectControllerPtr kinectController, int width, int height) 
: View(kinectController, width, height) {
	addEffect(View::VisualEffectPtr(new NormalMapEffect(width, height)));
	addEffect(View::VisualEffectPtr(new FatSuitEffect(width, height)));

	m_fbo.allocate(width, height);
}

FatSuitView::~FatSuitView()
{

}

void FatSuitView::draw()
{
	// draw normal map first into FBO
	m_fbo.begin();
	m_effects[0]->preDraw();
	m_texture.loadData(getKinectData().m_depthStream);
	m_effects[0]->draw();
	m_texture.draw(0, 0);
	m_effects[0]->postDraw();
	m_fbo.end();

	ofPtr<ofShader> shader = dynamic_pointer_cast<ShaderEffect, VisualEffect>(m_effects[1])->getShader();

	m_texture.loadData(getKinectData().m_videoStream);

	m_easyCam.begin();
	ofPushMatrix();
	//ofScale(1, -1, -1);
	ofTranslate(0, 0, -100);
	ofScale(1,-1,1);  
	//ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);  

	glEnable(GL_DEPTH_TEST);

	// now use fbo texture when drawing final
	m_effects[1]->preDraw();
	shader->setUniformTexture("texture", m_texture, 0);
	shader->setUniformTexture("normal_tex", m_fbo.getTextureReference(), 1);

	m_effects[1]->draw();
	//m_fbo.draw(0, 0);
	//m_texture.draw(0, 0);
	m_effects[1]->postDraw();

	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();

	m_easyCam.end();
}