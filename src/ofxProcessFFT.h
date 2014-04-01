#pragma once
#include "ofMain.h"
#include "ofxEasyFft.h"

//This does a simple breakout of Kyle McDonald's EasyFFT class from his ofxFFT implementation

enum fftRangeType {SUPERLOW, LOW, MID, HIGH, MAXSOUND};

class ProcessFFT {
public:

        
    ofxEasyFft fft;
    
    void setup(); //whether you want your sounds in a 0-1 range or a 0-volumeRange - setting volume range doesn't matter if you're normalizing
    void update();
    //For Debugging audio trends
    void drawHistoryGraph(ofPoint pt, fftRangeType drawType);
    
    //To feed to other components
    float getLoudBand();
    float getSuperLowVal();
    float getLowVal();
    float getMidVal();
    float getHighVal();
    float getNoisiness(); //not currently implemented
    bool  getNormalized();
    float getSpectralCentroid(); //this is not currently implemented
    float getDelta();
    float getUnScaledLoudestValue();
    float getSmoothedUnScaledLoudestValue();
    
    float getIntensityAtFrequency(float _freq);
    vector<float> getSpectrum();
    
    
    int getNumFFTbins();
    float getFFTpercentage();
    float getExponent();
    
    void setNumFFTBins(int _numFFTBins);
    void setFFTpercentage(float _FFTpercentage);
    void setExponent(float _Exponent);
    void setHistorySize(int _framesOfHistory);
    void setVolumeRange(int _volumeRange);
    void setNormalize(bool _normalize);
    
    void drawBars();
    void drawDebug();
    

    
private:
    
    bool    normalize; //decide if you want the values between 0 and 1 or between 0 - 1000
    int     volumeRange; //use if you're not normalizing so you can give things a proper range for visualization
    
    int     scaleFactor; //this is arbitrary - it raises the FFT numbers so they aren't 0.0000054
    int     numBins;
    float   noisiness;
    float   spectralCentroid;
    
    float   delta;

    void    calculateFFT(vector<float>& buffer, float FFTpercentage, int numFFTbins);
    
    bool    saveHistory;
    int     graphMaxSize; //number of frames that the values average over if you want a longer tail
    
    int     numFFTbins; //how many Columns are we analyzing - 16-32, etc
    float   FFTpercentage; //how much of the FFT are we parsing out into bins? usually around 20% of the whole 0-5khz
    float   exponent; //this is a factor for making the high end be valued slightly more than the low end since the bass tends to be higher energy than the high end in FFT - in a range from 1.0-1.6
    
    int     loudestBand; //for each frame - this is the loudest frequency band at that time
    
    float   maxSound; //this is the loudest sound for each frame - unnormalized
    
    float   avgMaxSoundOverTime; //average max sound in relation to the graph max sound
    
    vector <float> fftSpectrum; //this holds the amplitudes of multiple columns of the FFT
    vector <float> graphLow;
    vector <float> graphMid;
    vector <float> graphHigh;
    vector <float> graphSuperLow;
    vector <float> graphMaxSound;
    
    float superLowEqAvg, lowEqAvg, midEqAvg, highEqAvg;
    
    void drawAvgGraph(ofPoint pt, vector<float> values, ofColor _color);
    void drawAvgGraphUnScaled(ofPoint pt, vector<float> values, ofColor _color);
    
};