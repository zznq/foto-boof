#ifndef BOOF_VIEW
#define BOOF_VIEW

#include "ofxUI.h"

#include "ofTexture.h"

#include "VisualEffect.h"
#include "KinectData.h"
#include "ViewDelegate.h"


class VisualEffect;
class KinectController;
typedef ofPtr<KinectController> KinectControllerPtr;

class View {
public:
	typedef ofPtr<VisualEffect> VisualEffectPtr;
	typedef std::vector<VisualEffectPtr> VisualEffects;
	typedef ofPtr<ViewDelegate> ViewDelegatePtr;
	typedef ofPtr<ofxUICanvas> CanvasPtr;
private:
	int m_width;
	int m_height;
	int m_timeInterval; // Interval in milliseconds
	VisualEffects m_effects;
	ofTexture m_texture;
	ViewDelegatePtr m_delegate;

	float m_runningTime;

	bool m_useDepth;

protected:
	CanvasPtr m_canvas;
	KinectControllerPtr m_kinectController;
public:
	View(KinectControllerPtr kinectController, int width, int height);
	View(KinectControllerPtr kinectController, int width, int height, bool useDepth);

	virtual ~View();

	virtual void setup();
	virtual void update(float delta);
	virtual void draw();
	virtual void close();

	void addEffect(const VisualEffectPtr& effect);
	void removeEffect(const std::string& effectName);
	void clearEffects();
	int getViewInterval();
	
	ofPixels getKinectStream() const;
	CanvasPtr getCanvas() const;
	void setKinectDepthClipping(float nearClip, float farClip) const;

	void setViewDelegate(ViewDelegatePtr delegate);
	
	void viewStart();
	void viewComplete();
	void viewCountdownStarted();
};

#endif