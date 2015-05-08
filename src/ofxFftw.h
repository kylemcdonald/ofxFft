#pragma once

#ifdef OFX_FFT_USE_FFTW

#include "ofxFft.h"
#include "fftw3.h"

class ofxFftw : public ofxFft{
public:
	ofxFftw();
	void setup(int signalSize, fftWindowType windowType);
	~ofxFftw();
protected:
	void executeFft();
	void executeIfft();
private:
	float *fftIn, *fftOut, *ifftIn, *ifftOut;
	fftwf_plan fftPlan, ifftPlan;
};

#endif
