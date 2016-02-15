#include "ofApp.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 512 + 32, 512 + 64, OF_WINDOW);
	ofRunApp(new ofApp());
}
