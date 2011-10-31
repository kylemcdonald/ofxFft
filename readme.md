Basic usage is demonstrated by ofxFftEq and ofxFftTest.

To install ofxFft, move the ofxFft folder to your of/addons/ folder.

With each project, you need to:

1 Add to your compiler search path:
  of/addons/ofxFftw/src/
  of/addons/ofxFftw/libs/kiss

2 If you're using FFTW on Windows, you'll also need to add:
  of/libs/fftw/include/
  And link with:
  of/addons/lib/fftw/lib/win32/libfftw3f-3.dll

  If you're not using FFTW on Windows, you'll need to remove the line:

  #define USE_FFTW

  From ofxFft.h

FFTW was compiled with:

./configure --disable-shared --enable-static --enable-float --enable-threads
make
sudo make install

which puts the static libraries in .libs