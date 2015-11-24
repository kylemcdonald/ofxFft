#include "ofMain.h"
#include "testApp.h"

int main() {
	ofSetupOpenGL(1024 + 32, 128 + 32, OF_WINDOW);
	ofRunApp(new testApp());
}
