#pragma once

#include "ofMain.h"
#include "ofxFft.h"

class ofxEasyFft : public ofBaseSoundInput{
public:
	ofxEasyFft();
    ~ofxEasyFft();
	void setup(int bufferSize = 512,
						 fftWindowType windowType = OF_FFT_WINDOW_HAMMING,
						 fftImplementation implementation = OF_FFT_BASIC,
						 int audioBufferSize = 256,
						 int audioSampleRate = 44100);
	void setUseNormalization(bool useNormalization);
	void update();
	vector<float>& getAudio();
	vector<float>& getBins();
	
	void audioReceived(float* input, int bufferSize, int nChannels);
	
	ofxFft* fft;
	ofSoundStream stream;
private:
	bool useNormalization;
	ofMutex soundMutex;
	vector<float> audioFront, audioMiddle, audioBack, audioRaw;
	vector<float> bins;
	
	void normalize(vector<float>& data);

};
