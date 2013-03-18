#ifndef BOOF_VIEW
#define BOOF_VIEW

#include "ofTexture.h"

#include "VisualEffect.h"
#include "KinectData.h"

class KinectController;

class View {
public:
	typedef ofPtr<KinectController> KinectControllerPtr;
	typedef ofPtr<VisualEffect> VisualEffectPtr;
	typedef std::vector<VisualEffectPtr> VisualEffects;
private:
	int m_width;
	int m_height;
	int m_timeInterval; // Interval in milliseconds
	VisualEffects m_effects;
	ofTexture m_texture;
protected:
	KinectControllerPtr m_kinectController;
public:
	View(KinectControllerPtr kinectController, int width, int height);
	virtual ~View();

	virtual void update();
	virtual void draw();
	void addEffect(const VisualEffectPtr& effect);
	void removeEffect(const std::string& effectName);
	void clearEffects();
	int getViewInterval();
	KinectData getKinectData() const;
};

#endif