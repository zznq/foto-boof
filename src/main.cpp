#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "boof.h"

int main() {
	// create foto-boof application
	ofPtr<Boof> boofApp(new Boof(1280, 720));

	ofAppGlutWindow window;

	// setup opengl context and window
	ofSetupOpenGL(&window, 1280, 720, OF_FULLSCREEN);
	
	// run the app
	ofRunApp(boofApp);

	return 0; 
}
