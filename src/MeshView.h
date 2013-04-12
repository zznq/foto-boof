#ifndef BOOF_MESHVIEW
#define BOOF_MESHVIEW

#include "ofEasyCam.h"

#include "View.h"
#include "KinectController.h"

class MeshView : public View {
public:
	MeshView(KinectControllerPtr kinectController, int width, int height, bool pointCloud=false);
protected:
	virtual void doViewDraw();
	virtual void doEffectsDraw();

private:
	ofEasyCam m_easyCam;
	bool m_pointCloud;
};

#endif