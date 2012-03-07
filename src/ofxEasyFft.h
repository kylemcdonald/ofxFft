#include "ofxFft.h"

class ofxEasyFft : ofBaseApp {
public:
	ofxEasyFft();
	void setup(int bufferSize = 512,
						 fftWindowType windowType = OF_FFT_WINDOW_HAMMING,
						 fftImplementation implementation = OF_FFT_FFTW,
						 int audioBufferSize = 512,
						 int audioSampleRate = 48000);
	void setUseNormalization(bool useNormalization);
	void update();
	vector<float>& getAudio();
	vector<float>& getBins();
	
	void audioReceived(float* input, int bufferSize, int nChannels);
	
	ofxFft* fft;
	
private:
	int audioBufferSize, audioSampleRate;
	bool useNormalization;
	ofMutex soundMutex;
	vector<float> binsFront, binsMiddle, binsBack;
	vector<float> audioFront, audioMiddle, audioBack;
	
	void normalize(vector<float>& data);
};
