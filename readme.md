# ofxFft is an addon for [openFrameworks](http://openframeworks.cc)

ofxFft wraps two libraries with the same interface: FFTW and KISS FFT. KISS FFT is included with OF, MIT licensed, and enabled by default. FFTW is GPL, and can be enabled using the `OFX_FFT_USE_FFTW` compiler define.

To use ofxFft with FFTW make sure you download the [precompiled libraries](https://github.com/downloads/kylemcdonald/ofxFft/fftw-libs.zip).

Basic usage is demonstrated by example-eq and example-basic.

To install ofxFft, move the ofxFft folder to your `of/addons/` folder.

With each project, you need to:

1. Add to your compiler search path:
  - `of/addons/ofxFft/src`  
  - `of/addons/ofxFft/libs/kiss`

2. If you're using FFTW, you'll need to add `OFX_FFT_USE_FFTW` to your compiler flags:

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


3. If you're using FFTW on Windows, you'll also need to add:
  - `of/addons/ofxFft/libs/fftw/include`

  And link with:
  - `of/addons/ofxFft/libs/fftw/lib/win32/libfftw3f-3.dll`


FFTW was compiled with:

    ./configure --disable-shared --enable-static --enable-float --enable-threads
    make
    sudo make install

which puts the static libraries in .libs
