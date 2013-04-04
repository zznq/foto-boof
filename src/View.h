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

protected:
	int m_width;
	int m_height;
	int m_timeInterval; // Interval in milliseconds
	VisualEffects m_effects;
	ofTexture m_texture;
	ViewDelegatePtr m_delegate;

	float m_runningTime;

	bool m_useDepth;

	CanvasPtr m_canvas;
	KinectControllerPtr m_kinectController;

protected:
	// can be overridden if the view needs to do
	// something different in its draw routine
	virtual void doViewDraw();

	// can be overridden if the view needs to do
	// something different when the effects preDraw is called
	virtual void doEffectsPreDraw();

	// can be overridden if the view needs to do
	// something different when the effects draw is called
	virtual void doEffectsDraw();

	// can be overridden if the view needs to do
	// something different when the effects postDraw is called
	virtual void doEffectsPostDraw();

public:
	View(KinectControllerPtr kinectController, int width, int height, bool useDepth=false);

	virtual ~View();

	virtual void setup();
	virtual void update(float delta);
	virtual void draw();
	virtual void close();

	void addEffect(const VisualEffectPtr& effect);
	void removeEffect(const std::string& effectName);
	void clearEffects();
	int getWidth() const;
	int getHeight() const;
	int getViewInterval();
	
	const KinectControllerPtr& getKinectController() const;
	KinectData getKinectData() const;
	CanvasPtr getCanvas() const;
	void setKinectDepthClipping(float nearClip, float farClip) const;

	void setViewDelegate(ViewDelegatePtr delegate);
	
	void viewStart();
	void viewComplete();
	void viewCountdownStarted();
};

#endif