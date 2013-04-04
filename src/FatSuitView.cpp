
#include "FatSuitView.h"

#include "NormalMapEffect.h"
#include "FatSuitEffect.h"
#include "MeshEffect.h"

FatSuitView::FatSuitView(KinectControllerPtr kinectController, int width, int height) 
: View(kinectController, width, height) {
	addEffect(View::VisualEffectPtr(new NormalMapEffect(width, height)));
	addEffect(View::VisualEffectPtr(new FatSuitEffect(width, height)));
	addEffect(View::VisualEffectPtr(new MeshEffect(width, height)));

	m_fbo.allocate(width, height);
}

FatSuitView::~FatSuitView()
{
	m_texture.clear();
}

void FatSuitView::draw()
{
	// draw normal map first into FBO
	m_fbo.begin();
	m_effects[0]->preDraw();
	m_effects[0]->draw();
	m_texture.loadData(getKinectData().m_depthStream);
	m_texture.draw(0, 0);
	m_effects[0]->postDraw();
	m_fbo.end();

	// now use fbo texture when drawing final
	ofPtr<ofShader> shader = dynamic_pointer_cast<ShaderEffect, VisualEffect>(m_effects[1])->getShader();
	shader->setUniformTexture("normalTexture", m_fbo.getTextureReference(), 1);
	m_effects[1]->preDraw();
	m_fbo.draw(0, 0);
	m_effects[1]->draw();
	m_effects[1]->postDraw();
}