# ofxFft

ofxFft is a Fast-Fourier Transform addon for [openFrameworks](http://openframeworks.cc)

ofxFft wraps two libraries with the same interface: FFTW and KISS FFT. KISS FFT is included with OF, MIT licensed, and enabled by default. FFTW is GPL and must be manually enabled using the `OFX_FFT_USE_FFTW` compiler define.

## Installation & Usage

To install ofxFft, move the ofxFft folder to your `of/addons/` folder.

Search paths required when using ofxFFt:

- `of/addons/ofxFft/src`  
- `of/addons/ofxFft/libs/kiss`

Basic usage is demonstrated by example-eq and example-basic.

With OF version 0.9.0, the OF Project Generator will add the compiler search paths for the project automatically if configured to include ofxFft.

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the OpenFrameworks distribution.

Point the ProjectGenerator to `addons/ofxFft`, change the project name to the **exact** name of the example you want to generate (ie `example-basic`), and make sure to choose `ofxFft` from the addons. Hitting "Generate" will populate that example with the project files you will need to build it.

## Using FFTW

To use ofxFft with FFTW instead of KISS FFT, make sure you download the [precompiled libraries](https://github.com/downloads/kylemcdonald/ofxFft/fftw-libs.zip).

Next, you'll need to add `OFX_FFT_USE_FFTW` to your compiler flags for each project using ofxFFT:

  - Xcode:
	* Select the Project -> Build Settings -> Other C++ Flags
	* add the following:
	<pre>
	-DOFX_FFT_USE_FFTW
	</pre>

  - Linux (Makefiles & Codeblocks):
    * edit config.make in your project folder and add the following to the USER_CFLAGS line:
    <pre>
    -DOFX_FFT_USE_FFTW
    </pre>
	
  - Codeblocks (Win):
	* right-click on your project in the project tree
	* select "Build options..."
	* make sure the project name is selected in the tree (not release or debug)
	* select the "Compiler settings" tab and add the following to the "#defines" tab:
	<pre>
	OFX_FFT_USE_FFTW
	</pre>
	
  - Visual Studio:
    * right click on the project in the project tree and select Properties
    * set the Configuration to All Configurations
    * add the defines and search paths:
    * add the following to Configuration Properties->C/C++->Preprocessor->Preprocessor Definitions:
    <pre>
	OFX_FFT_USE_FFTW
	</pre>

3. If you're using FFTW on Windows, you'll also need to add:
  <pre>
  `of/addons/ofxFft/libs/fftw/include`
  </pre>
  And link with:
  <pre>
  `of/addons/ofxFft/libs/fftw/lib/win32/libfftw3f-3.dll`
  </pre>

### Using FFTW in the examples

If you're using FFTW with the example projects, in ofApp.cpp, comment the line:

	fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING);

and uncomment out the line:
	
	//fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HAMMING, OF_FFT_FFTW);

### FFTW compilation notes

FFTW was compiled with:

    ./configure --disable-shared --enable-static --enable-float --enable-threads
    make
    sudo make install

which puts the static libraries in .libs

Note that the library is compiled in 32-bit mode on both Windows and OS X.