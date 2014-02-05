#pragma once

#include "ofMain.h"
#include "ofxProcessFFT.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
    void keyPressed(int key);
	
	ProcessFFT fft;
};
