#include "ofxEasyFft.h"

ofxEasyFft::ofxEasyFft()
:useNormalization(true) {
}

void ofxEasyFft::setup(int bufferSize, fftWindowType windowType, fftImplementation implementation, int audioBufferSize, int audioSampleRate) {
	if(bufferSize < audioBufferSize) {
		ofLogFatalError() << "ofxEasyFft bufferSize (" << bufferSize << ") must be less than the audioBufferSize (" << audioBufferSize << ")";
	}
	this->audioBufferSize = audioBufferSize;
	this->audioSampleRate = audioSampleRate;
	fft = ofxFft::create(bufferSize, windowType, implementation);
	
	binsFront.resize(fft->getBinSize());
	binsMiddle.resize(fft->getBinSize());
	binsBack.resize(fft->getBinSize());
	
	audioFront.resize(bufferSize);
	audioMiddle.resize(bufferSize);
	audioBack.resize(bufferSize);
	
	ofSoundStreamSetup(0, 1, this, audioSampleRate, audioBufferSize, 1);
}

void ofxEasyFft::setUseNormalization(bool useNormalization) {
	this->useNormalization = useNormalization;
}

void ofxEasyFft::update() {
	soundMutex.lock();
	binsFront = binsMiddle;
	audioFront = audioMiddle;
	soundMutex.unlock();		
}

vector<float>& ofxEasyFft::getAudio() {
	return audioFront;
}

vector<float>& ofxEasyFft::getBins() {
	return binsFront;
}

void ofxEasyFft::audioReceived(float* input, int bufferSize, int nChannels) {
	audioBack.assign(input, input + bufferSize);
	normalize(audioBack);
	fft->setSignal(&audioBack[0]);
	float* curFft = fft->getAmplitude();
	binsBack.assign(curFft, curFft + fft->getBinSize());
	normalize(binsBack);
	soundMutex.lock();
	binsMiddle = binsBack;
	audioMiddle = audioBack;
	soundMutex.unlock();
}

void ofxEasyFft::normalize(vector<float>& data) {
	if(useNormalization) {
		float maxValue = 0;
		for(int i = 0; i < data.size(); i++) {
			if(abs(data[i]) > maxValue) {
				maxValue = abs(data[i]);
			}
		}
		for(int i = 0; i < data.size(); i++) {
			data[i] /= maxValue;
		}
	}
}
