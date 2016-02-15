#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 512 + 32, (128 + 32) * 3, OF_WINDOW);
	ofRunApp(new ofApp());
}
