#include "ofMain.h"

#include "boof.h"

int main() {
	ofPtr<Boof> boofApp(new Boof());

	// setup opengl context and window
	ofSetupOpenGL(boofApp->getWindow(), 1024, 768, OF_WINDOW);

	// run the app
	ofRunApp(boofApp);

	return 0; 
}
