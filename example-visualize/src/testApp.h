#pragma once

#include "ofMain.h"
#include "ofxFft.h"

#define MIC 0
#define NOISE 1
#define SINE 2

class testApp : public ofBaseApp {
public:
	void setup();
	void plot(vector<float>& buffer, float scale, float offset);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void draw();
	void keyPressed(int key);

	int plotHeight, bufferSize;

	ofxFft* fft;

	float appWidth;
	float appHeight;

	ofImage spectrogram;
	int spectrogramOffset;

	int mode;
	
	ofMutex soundMutex;
	vector<float> drawBuffer, middleBuffer, audioBuffer;
	vector<float> drawBins, middleBins, audioBins;
};
