# ofxFft is an addon for [openFrameworks](http://openframeworks.cc/)

ofxFft wraps two libraries with the same interface: FFTW and KISS FFT. KISS FFT is MIT licensed, and enabled by default. FFTW is GPL, and can be enabled using the `#define USE_FFTW` flag.

To use ofxFft with FFTW make sure you download the [precompiled libraries](https://github.com/downloads/kylemcdonald/ofxFft/fftw-libs.zip).

Basic usage is demonstrated by example-eq and example-basic.

To install ofxFft, move the ofxFft folder to your `of/addons/` folder.

Note: With ofx version 0.9.0, the project generator should add the compiler search paths for the project automatically if configured to include ofxFft.

With each project, you need to:

1. Add to your compiler search path:
  - `of/addons/ofxFft/src`  
  - `of/addons/ofxFft/libs/kiss`

2. If you're using FFTW on Windows, you'll also need to add:
  - `of/addons/ofxFft/libs/fftw/include`

  And link with:
  - `of/addons/ofxFft/libs/fftw/lib/win32/libfftw3f-3.dll`


If you're not using FFTW, you'll need to:

1. From ofxFft.h, remove the line:

	#define USE_FFTW

2. In the example projects ofApp.cpp, uncomment the line:

	//fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);

	and comment out the line:
	
	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);



FFTW was compiled with:

./configure --disable-shared --enable-static --enable-float --enable-threads
make
sudo make install

which puts the static libraries in .libs

Note that the library is compiled in 32-bit mode on both Windows and OS X.