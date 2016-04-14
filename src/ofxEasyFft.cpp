#include "ofxEasyFft.h"

ofxEasyFft::ofxEasyFft()
:useNormalization(true) {
}
ofxEasyFft::~ofxEasyFft(){
    stream.close();
}

void ofxEasyFft::setup(int bufferSize, fftWindowType windowType, fftImplementation implementation, int audioBufferSize, int audioSampleRate) {
	if(bufferSize < audioBufferSize) {
		ofLogWarning("ofxEasyFft") << "bufferSize (" << bufferSize << ") less than audioBufferSize (" << audioBufferSize << "), using " << audioBufferSize;
		bufferSize = audioBufferSize;
	}
	fft = ofxFft::create(bufferSize, windowType, implementation);
	
	bins.resize(fft->getBinSize());
	
	audioFront.resize(bufferSize);
	audioMiddle.resize(bufferSize);
	audioBack.resize(bufferSize);
	audioRaw.resize(bufferSize);
	
    stream.getDeviceList();
    stream.setup(0, 1, audioSampleRate, audioBufferSize, 2);
    stream.setInput(this);
}

void ofxEasyFft::setUseNormalization(bool useNormalization) {
	this->useNormalization = useNormalization;
}

void ofxEasyFft::update() {
	soundMutex.lock();
	audioFront = audioMiddle;
	soundMutex.unlock();
	
	fft->setSignal(&audioFront[0]);
	float* curFft = fft->getAmplitude();
	copy(curFft, curFft + fft->getBinSize(), bins.begin());
	normalize(bins);
}

vector<float>& ofxEasyFft::getAudio() {
	return audioFront;
}

vector<float>& ofxEasyFft::getBins() {
	return bins;
}

void ofxEasyFft::audioReceived(float* input, int bufferSize, int nChannels) {
	if(audioRaw.size() > bufferSize) {
		copy(audioRaw.begin() + bufferSize, audioRaw.end(), audioRaw.begin()); // shift old
	}
	copy(input, input + bufferSize, audioRaw.end() - bufferSize); // push new
	copy(audioRaw.begin(), audioRaw.end(), audioBack.begin());
	normalize(audioBack);

	soundMutex.lock();
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
