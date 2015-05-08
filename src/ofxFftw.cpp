#include "ofxFftw.h"

#ifdef OFX_FFT_USE_FFTW

ofxFftw::ofxFftw() :
	fftPlan(NULL) {
}

void ofxFftw::setup(int signalSize, fftWindowType windowType) {
	ofxFft::setup(signalSize, windowType);

	// more info on setting up a forward r2r fft here:
	// http://www.fftw.org/fftw3_doc/Real_002dto_002dReal-Transforms.html
	fftIn = (float*) fftwf_malloc(sizeof(float) * signalSize);
	fftOut = (float*) fftwf_malloc(sizeof(float) * signalSize);
	fftPlan = fftwf_plan_r2r_1d(signalSize, fftIn, fftOut, FFTW_R2HC,
	                            FFTW_DESTROY_INPUT | FFTW_MEASURE);

	// the difference between setting up an r2r ifft and fft
	// is using the flag/kind FFTW_HC2R instead of FFTW_R2HC:
	// http://www.fftw.org/fftw3_doc/Real_002dto_002dReal-Transform-Kinds.html
	ifftIn = (float*) fftwf_malloc(sizeof(float) * signalSize);
	ifftOut = (float*) fftwf_malloc(sizeof(float) * signalSize);
	ifftPlan = fftwf_plan_r2r_1d(signalSize, ifftIn, ifftOut, FFTW_HC2R,
	                             FFTW_DESTROY_INPUT | FFTW_MEASURE);
}

void ofxFftw::executeFft() {
	memcpy(fftIn, signal, sizeof(float) * signalSize);
	runWindow(fftIn);
	fftwf_execute(fftPlan);
	// explanation of halfcomplex format:
	// http://www.fftw.org/fftw3_doc/The-Halfcomplex_002dformat-DFT.html
	copyReal(fftOut);
	imag[0] = 0;
	for (int i = 1; i < binSize; i++)
		imag[i] = fftOut[signalSize - i];
	cartesianUpdated = true;
}

void ofxFftw::executeIfft() {
	memcpy(ifftIn, real, sizeof(float) * binSize);
	for (int i = 1; i < binSize; i++)
		ifftIn[signalSize - i] = imag[i];
	fftwf_execute(ifftPlan);
	runInverseWindow(ifftOut);
	copySignal(ifftOut);
}

ofxFftw::~ofxFftw() {
	if (fftPlan != NULL) {
		fftwf_destroy_plan(fftPlan);
		fftwf_free(fftIn);
		fftwf_free(fftOut);

		fftwf_destroy_plan(ifftPlan);
		fftwf_free(ifftIn);
		fftwf_free(ifftOut);
		fftwf_cleanup();
	}
}

#endif
