#pragma once

#include "ofApp.h"
#include "ofxProcessFFT.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void keyPressed(int key);
	
	ProcessFFT fft;
};
