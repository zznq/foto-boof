#ifndef BOOF_VIEW
#define BOOF_VIEW

#include "KinectController.h"

class View {
public:
	typedef ofPtr<KinectController> KinectControllerPtr;
private:
	int m_timeInterval; // Interval in milliseconds
protected:
	KinectControllerPtr m_kinectController;
public:
	View(KinectControllerPtr kinectController);
	~View();

	virtual void start();
	void update();
	void draw();

	int getViewInterval() {
		return m_timeInterval;
	}
};

#endif