#pragma once

#include "ofMain.h"

// forward declarations
class ofAppBaseWindow;
class KinectController;
class PixelEffect;

class Boof : public ofBaseApp {
public:
	// convenience typedefs
	typedef ofPtr<ofAppBaseWindow> WindowPtr;
	typedef ofPtr<KinectController> KinectControllerPtr;
	typedef ofPtr<PixelEffect> PixelEffectPtr;
public:
	Boof();
	~Boof();

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	WindowPtr getWindow() const;

private:
	WindowPtr m_window;
	KinectControllerPtr m_kinectController;
	PixelEffectPtr m_bwEffect;
	bool m_bwEffectEnabled;
};
