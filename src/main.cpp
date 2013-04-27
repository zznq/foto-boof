#include "ofMain.h"

#include "boof.h"

int main() {
	// create foto-boof application
	ofPtr<Boof> boofApp(new Boof(1280, 720));

	// run the app
	ofRunApp(boofApp);

	return 0; 
}
