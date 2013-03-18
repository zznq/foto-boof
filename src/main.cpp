#include "ofMain.h"

#include "boof.h"

int main() {
	// create foto-boof applicatoin
	ofPtr<Boof> boofApp(new Boof());

	// run the app
	ofRunApp(boofApp);

	return 0; 
}
