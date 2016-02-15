#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024 + 32, 128 + 32, OF_WINDOW);
	ofRunApp(new ofApp());
}
