#pragma once

#include "ofMain.h"
#include "ofxFft.h"

enum {SINE, MIC, NOISE};

class ofApp : public ofBaseApp {
public:
	void setup();
	void plot(vector<float>& buffer, float scale, float offset);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void draw();
	void keyPressed(int key);

	int plotHeight, bufferSize;

	ofxFft* fft;
	
	int spectrogramOffset;
	ofImage spectrogram;
	int mode;
	
	int appWidth, appHeight;
	
	ofMutex soundMutex;
	vector<float> drawBins, middleBins, audioBins;
	vector<float> drawBuffer, middleBuffer, audioBuffer;
};
