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
	const float static DEFAULT_X_OFFSET;
	const float static DEFAULT_Y_OFFSET;
	const float static DEFAULT_Z_OFFSET;

	ofVec3f m_offset;

	float m_nearClipping;
	float m_farClipping;

	ofCamera m_cam;
protected:
	bool m_useDepth;
	bool m_countDownRunning;

	int m_width;
	int m_height;
	int m_timeInterval; // Interval in milliseconds
	
	float m_runningTime;

	VisualEffects m_effects;
	ofTexture m_texture;
	ViewDelegatePtr m_delegate;

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

	void startCountDown();
	void stopCountDown();

	void addEffect(const VisualEffectPtr& effect);
	void removeEffect(const std::string& effectName);
	void clearEffects();

	int getWidth() const;
	int getHeight() const;

	ofVec3f getOffset() const;
	void setOffset(const ofVec3f& offset);
	void setOffset(const float x, const float y, const float z);

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