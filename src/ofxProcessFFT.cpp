#include "ofxProcessFFT.h"

void ProcessFFT::setup(){
    
    scaleFactor = 10000;
    numBins = 16384;
    
    fft.setup(numBins); //default
    fft.setUseNormalization(false);
    
    graphMaxSize = 200; //approx 10sec of history at 60fps
    
    graphLow.assign(graphMaxSize, 0.0);
    graphMid.assign(graphMaxSize, 0.0);
    graphHigh.assign(graphMaxSize, 0.0);
    graphSuperLow.assign(graphMaxSize, 0.0);
    graphMaxSound.assign(graphMaxSize, 200.0);
    
    saveHistory = false;
    
    exponent = 1.0;
    
    numFFTbins = 32;
    FFTpercentage = 0.14;
    
    delta = loudestBand = noisiness = maxSound = avgMaxSoundOverTime = 0;
    
    normalize = false;
    volumeRange = 400; //only used if normalize is false

}

//---------------------------------------------
void ProcessFFT::update(){
    fft.update();
    if(saveHistory){
        if (graphHigh.size()>graphMaxSize) {
            graphHigh.erase(graphHigh.begin(), graphHigh.begin()+1);
            graphMid.erase(graphMid.begin(), graphMid.begin()+1);
            graphLow.erase(graphLow.begin(), graphLow.begin()+1);
            graphSuperLow.erase(graphSuperLow.begin(),graphSuperLow.begin()+1);
        }
    }
    
    if(graphMaxSound.size()>graphMaxSize){ //make sure this is always running!
        graphMaxSound.erase(graphMaxSound.begin(), graphMaxSound.begin()+1);
    }
    
    calculateFFT(fft.getBins(), FFTpercentage, numFFTbins);

}

//---------------------------------------------
void ProcessFFT::calculateFFT(vector<float>&buffer, float _FFTpercentage, int _numFFTbins){
    
    this->numFFTbins = _numFFTbins;
    this->FFTpercentage = _FFTpercentage;
    
    fftSpectrum.clear(); //empty it all
    
    float loudBand = 0;
    maxSound = 0;
    float freqDelta = 0;
    
    for(int i = 0; i<numFFTbins; i++){
        fftSpectrum.push_back(0); //init the vector for each pass
    }
    
    //sort through and find the loudest sound
    //use the loudest sound to normalize it to the proper range of 0-1
    //drop all those values into the fftSpectrum
    
    //average first
    for(int i=0; i<fftSpectrum.size(); i++){ //for the number of columns
        float bin_size = buffer.size()*FFTpercentage;
        
        for (int j=(bin_size*((float)i/numFFTbins)); j<bin_size*((float)1/numFFTbins)+(bin_size*((float)i/numFFTbins)) ; j++) { //for each i position, average the values in i's+offset
            fftSpectrum[i] = fftSpectrum[i] + buffer[j]*10000; //sum values in each section of buffers. Multiply by 10000 so you're not dealing with tiny numbers.
        }
        
        fftSpectrum[i] = abs((fftSpectrum[i]/(bin_size*(float)1/numFFTbins))*(1+pow(i, exponent)/numFFTbins));//Then make low frequency values weighted lower than high frequency with pow
        
        //find maximum band
        if (maxSound<fftSpectrum[i]) {
            maxSound = fftSpectrum[i];
            loudestBand = i;
        }
    }
    
    graphMaxSound.push_back(maxSound); //accumulate loudest sounds
    
    float accumMaxSounds;
    for (int i =0; i<graphMaxSound.size(); i++) {
        accumMaxSounds = accumMaxSounds+graphMaxSound[i]; //add up all loudest sounds
    }
    
    avgMaxSoundOverTime = accumMaxSounds/graphMaxSound.size(); //take average over a certain number of frames
    
    float meanSum=0;
    float mean,stdDev,stdDevAccum, variance,deviation;
    
    float spectralCentroidAccum, spectralWeightsAccum, spectralWeights;
    
    
    for(int i=0; i<fftSpectrum.size(); i++){ //for the number of columns
        
        //NORMALIZE
        if(normalize){
            fftSpectrum[i] = ofMap(fftSpectrum[i], 0, avgMaxSoundOverTime, 0, 1, true); //normalize each frame to 0-1
        }
        
        //COMPUTE NOISINESS
        //compute standard deviation - this works OK, but isn't iron clad - can detect single notes versus multiple notes
        //REMOVING FOR FULL RUN - NOT BEING USED
        /*
        meanSum = meanSum + fftSpectrum[i]; //add up all values
        
        mean=meanSum/fftSpectrum.size(); //compute the mean
        
        deviation = (fftSpectrum[i]-mean)*(fftSpectrum[i]-mean);
        
        stdDevAccum = deviation + stdDevAccum;
        
        variance = stdDevAccum/(fftSpectrum.size()-1);
        
        noisiness = stdDev = sqrt(variance);
        
        //AVERAGE PITCH/SPECTRAL CENTROID
        //compute spectral centroid/average pitch - this is not quite right yet
        spectralCentroidAccum += i*fftSpectrum[i];
        
        spectralWeightsAccum += fftSpectrum[i];
        
        spectralCentroid = spectralCentroidAccum/spectralWeightsAccum; //gives the average band that is the loudest
        */
        
        //EQ BANDS
        if (i==1 ) {
            superLowEqAvg = (fftSpectrum[0]); //just compute the lowest bass bin - not an average
        }
        
        //find bands for each 3rd of the entire thing...this is not musically accurate, just a rough estimate
        if (i>0 && i<numFFTbins*.333) {
            lowEqAvg = lowEqAvg+fftSpectrum[i];
        }
        if (i>numFFTbins*.33 && i<numFFTbins*.666) {
            midEqAvg = midEqAvg+fftSpectrum[i];
        }
        if (i>numFFTbins*.666 && i<numFFTbins) {
            highEqAvg = highEqAvg+fftSpectrum[i];
        }
    }
    
    freqDelta = freqDelta/numFFTbins;
    
    // superLowEqAvg = (float)superLowEqAvg/2; //only doing it off the lowest ones
    lowEqAvg = lowEqAvg/(numFFTbins*.333); //take a third of the entire bin collection to decide about low/mid/high
    midEqAvg = midEqAvg/(numFFTbins*.333);
    highEqAvg = highEqAvg/(numFFTbins*.333);
    
    if(saveHistory){ //only save these if drawing
        graphSuperLow.push_back(superLowEqAvg);
        graphLow.push_back(lowEqAvg);
        graphMid.push_back(midEqAvg);
        graphHigh.push_back(highEqAvg);
    }
    
}

//---------------------------------------------
void ProcessFFT::drawHistoryGraph(ofPoint pt, fftRangeType drawType){
    
    saveHistory=true; //only do this if drawing
    
    switch (drawType) {
        case SUPERLOW:
            drawAvgGraph(pt, graphSuperLow, ofColor(0, 100, 255,200));
            break;
        case LOW:
            drawAvgGraph(pt, graphLow, ofColor(0, 100, 255,200));
            break;
        case MID:
            drawAvgGraph(pt, graphMid, ofColor(0, 255, 100,200));
            break;
        case HIGH:
            drawAvgGraph(pt, graphHigh,ofColor(255, 0, 100,200));
            break;
        case MAXSOUND:
            drawAvgGraphUnScaled(pt, graphMaxSound,ofColor(255, 100, 255,200));
            break;
        default:
            drawAvgGraphUnScaled(pt, graphMaxSound,ofColor(255, 100, 255,200));
            break;
    }
    

    
}

//---------------------------------------------
void ProcessFFT::drawAvgGraph(ofPoint pt, vector<float> values, ofColor _color){

    if (normalize) {
        ofEnableAlphaBlending();
        ofPushMatrix();
        ofFill();
        ofSetColor(_color);
        ofTranslate(pt.x, pt.y);
        ofBeginShape();
    
        float avgVal;
        for (int i = 0; i < (int)ofMap(values.size(), 0 , values.size(), 0,200); i++){ //scale it to be 200px wide
            if( i == 0 ) ofVertex(i, 200);
            
            ofVertex(i,ofMap(values[(int)ofMap(i, 0 , 200, 0,values.size())], 0, 1, 200, 0,true));
            
            avgVal = avgVal+values[(int)ofMap(i, 0 , 200, 0,values.size())];
            if( i == 200 -1 ) ofVertex(i, 200);
        }
    
        avgVal = avgVal/values.size();
        
        ofEndShape(false);
        ofSetColor(255);
        ofDrawLine(0,ofMap(avgVal, 0, 1, 200, 0,true) , 200, ofMap(avgVal, 0, 1, 200, 0,true));
        ofPopMatrix();
        ofDisableAlphaBlending();
    }else{
        //not normalized
        ofEnableAlphaBlending();
        ofPushMatrix();
        ofFill();
        ofSetColor(_color);
        ofTranslate(pt.x, pt.y);
        ofBeginShape();
        
        float avgVal;
        for (int i = 0; i < (int)ofMap(values.size(), 0 , values.size(), 0,200); i++){ //scale it to be 200px wide
            if( i == 0 ) ofVertex(i, 200);
            
            ofVertex(i,ofMap(values[(int)ofMap(i, 0 , 200, 0,values.size())], 0, volumeRange, 200, 0,true));
            
            avgVal = avgVal+values[(int)ofMap(i, 0 , 200, 0,values.size())];
            if( i == 200 -1 ) ofVertex(i, 200);
        }
        
        avgVal = avgVal/values.size();
        
        ofEndShape(false);
        ofSetColor(255);
        ofDrawLine(0,ofMap(avgVal, 0, volumeRange, 200, 0,true) , 200, ofMap(avgVal, 0, volumeRange, 200, 0,true));
        ofPopMatrix();
        ofDisableAlphaBlending();
    }
    
}
//---------------------------------------------
void ProcessFFT::drawAvgGraphUnScaled(ofPoint pt, vector<float> values, ofColor _color){
        ofEnableAlphaBlending();
        ofPushMatrix();
        ofFill();
        ofSetColor(_color);
        ofTranslate(pt.x, pt.y);
        ofBeginShape(); //then do the average again
    
    float prevAvgMaximum = 0;
        float avgMaximum =0;
    
        for (int i = 0; i < (int)ofMap(values.size(), 0 , values.size(), 0,200); i++){
            if( i == 0 ) ofVertex(i, 200);
            
            ofVertex(i,ofMap(values[(int)ofMap(i, 0 , 200, 0,values.size())], 0, volumeRange, 200, 0,true));
            
            avgMaximum = avgMaximum+values[(int)ofMap(i, 0 , 200, 0,values.size())];
            
            if (i<((int)ofMap(i, 0 , 200, 0,values.size()))/2) {
                prevAvgMaximum = prevAvgMaximum + values[(int)ofMap(i, 0 , 200, 0,values.size())]; //take half of the bin and get the average and compare that to the whole thing
            }
            
            if( i == 200 -1 ) ofVertex(i, 200);
        }
        
        ofEndShape(false);
    
    avgMaximum = avgMaximum/values.size();
    prevAvgMaximum = prevAvgMaximum/(values.size()/2);
    
   // cout<< "Delta: " << avgMaximum - prevAvgMaximum <<endl;
    
    //make trigger a percentage of the current max volume
    
    
    
    ofSetColor(255);
    ofDrawLine(0,ofMap(avgMaximum, 0, volumeRange, 200, 0,true) , 200, ofMap(avgMaximum, 0,volumeRange, 200, 0,true));

    
        ofPopMatrix();
        ofDisableAlphaBlending();
}

//---------------------------------------------
void ProcessFFT::drawBars(){
    if(normalize){
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(2);
    for(int i=0; i<fftSpectrum.size(); i++){ //for the number of columns
        if (i==loudestBand) {
            ofSetColor(255,0,0);
        }
        else{
            ofSetColor(100,100,200);
        }
        ofNoFill();
        ofDrawRectangle(ofGetWidth()*((float)i/numFFTbins), ofGetHeight()-20, ofGetWidth()/numFFTbins, -ofMap(fftSpectrum[i], 0, 1, 0, ofGetHeight() -50));
    }
    ofPopStyle();
    }else{
        //not normalized
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetLineWidth(2);
        for(int i=0; i<fftSpectrum.size(); i++){ //for the number of columns
            if (i==loudestBand) {
                ofSetColor(255,0,0);
            }
            else{
                ofSetColor(100,100,200);
            }
            ofNoFill();
            ofDrawRectangle(ofGetWidth()*((float)i/numFFTbins), ofGetHeight()-20, ofGetWidth()/numFFTbins, -ofMap(fftSpectrum[i], 0, volumeRange, 0, ofGetHeight() -50));
        }
        ofPopStyle();
    }
}

//---------------------------------------------
void ProcessFFT::drawDebug(){
    ofPushMatrix();
    ofDrawBitmapStringHighlight("Loudest Band: " + ofToString(loudestBand), 250,20);
    ofDrawBitmapStringHighlight("Curr. Max Sound Val: "+ ofToString(maxSound), 250,40);
    ofDrawBitmapStringHighlight("Super Low Avg: " + ofToString(superLowEqAvg), 250,60);
    ofDrawBitmapStringHighlight("Low Avg: " + ofToString(lowEqAvg), 250,80);
    ofDrawBitmapStringHighlight("Mid Avg: " + ofToString(midEqAvg), 250,100);
    ofDrawBitmapStringHighlight("High Avg: " + ofToString(highEqAvg), 250,120);
    ofDrawBitmapStringHighlight("Noisiness: " + ofToString(noisiness), 250,140);
    ofDrawBitmapStringHighlight("SpectralCentroid: " + ofToString(spectralCentroid), 250,160);
    ofDrawBitmapStringHighlight("Avg Max Sound: " + ofToString(avgMaxSoundOverTime), 250,180);
    ofDrawBitmapStringHighlight("Delta: " + ofToString(getDelta()), 250,200);
    ofDrawBitmapStringHighlight("Delta Shift Detected: " + ofToString(abs(getDelta())>(avgMaxSoundOverTime*.20)), 250,220);
    
    
    ofDrawBitmapStringHighlight("Freq Range up to: " +ofToString(ofMap(FFTpercentage, 0, 0.23, 0, 5000)) + "hz", 450,60);
    float freqPerBin = ofMap(FFTpercentage, 0, 0.23, 0, 5000)/numFFTbins;
    ofDrawBitmapStringHighlight("Freq range per bin: " +ofToString(freqPerBin) + "hz", 450,80);
    ofDrawBitmapStringHighlight("Approx Number of octaves from C0: " +ofToString(ofMap(ofMap(FFTpercentage, 0, 0.23, 0, 5000),0,5000,0,8)), 450,100); //wrong wrong wrong - octave frequency doubles as it goes up - octave is from n to 2n hz, so do more math for this
    ofDrawBitmapStringHighlight("Approx Freq of Loudest Band: " +ofToString(freqPerBin*loudestBand)+"hz", 450,120);
    ofPopMatrix();
}


//GETTERS

float ProcessFFT::getIntensityAtFrequency(float _freq){
    //Todo: figure out this calculation from the raw bins
    //8193 bins
    
   // fft.getBins()[fft.getBins().size()];
    
    int whichBin;
    whichBin = ofMap(_freq, 0, 22100, 0, fft.getBins().size()); //An approximation...
    
    float normalizedFreq;
    normalizedFreq = ofMap(fft.getBins()[whichBin]*scaleFactor, 0, avgMaxSoundOverTime, 0, 1,true); //the scalefactor is just a scaling factor to have easier numbers to look at
    
    return normalizedFreq;
    
}

float ProcessFFT::getDelta(){
    float prevAvgMaximum = 0;
    float avgMaximum =0;
    
    for (int i = 0; i < graphMaxSound.size(); i++){
              
        avgMaximum = avgMaximum+graphMaxSound[i];
        
        if (i<graphMaxSound.size()/2) {
            prevAvgMaximum = prevAvgMaximum + graphMaxSound[i]; //take half of the bin and get the average and compare that to the whole thing
        }
        
    }
    
    avgMaximum = avgMaximum/graphMaxSound.size();
    prevAvgMaximum = prevAvgMaximum/(graphMaxSound.size()/2);
    
    delta = avgMaximum - prevAvgMaximum;
    return delta; //if the delta is greater than a percentage of the maximum volume, then trigger an event (delta scales to maximum volume)
}


float ProcessFFT::getUnScaledLoudestValue(){
    //This returns the unnormalized value of the current loudest sound - useful for detecting whether the volume input is low or really high
    return maxSound;
}

float ProcessFFT::getSmoothedUnScaledLoudestValue(){
    return avgMaxSoundOverTime;
}

vector<float> ProcessFFT::getSpectrum(){
    return fftSpectrum;
}

float ProcessFFT::getNoisiness(){
    return noisiness;
}

bool ProcessFFT::getNormalized(){
    return normalize;
}

float ProcessFFT::getLoudBand(){
    return loudestBand; //Todo: this needs to be an average
}

float ProcessFFT::getSuperLowVal(){
    return superLowEqAvg; //this is NOT smoothed, but outputs
}

float ProcessFFT::getLowVal(){
    return lowEqAvg;
}

float ProcessFFT::getMidVal(){
    return midEqAvg;
}

float ProcessFFT::getHighVal(){
    return highEqAvg;
}

float ProcessFFT::getFFTpercentage(){
    return FFTpercentage;
}

float ProcessFFT::getExponent(){
    return FFTpercentage;
}

int ProcessFFT::getNumFFTbins(){
    return numFFTbins;
}

//SETTERS
void ProcessFFT::setFFTpercentage(float _FFTpercentage){
    this->FFTpercentage=_FFTpercentage;
}

void ProcessFFT::setExponent(float _exponent){
    this->exponent=_exponent;
}

void ProcessFFT::setNumFFTBins(int _numFFTBins){
    this->numFFTbins = _numFFTBins;
}

void ProcessFFT::setHistorySize(int _framesOfHistory){
    this->graphMaxSize = _framesOfHistory;
}

void ProcessFFT::setNormalize(bool _normalize){
    this->normalize = _normalize;
}

void ProcessFFT::setVolumeRange(int _volumeRange){
    this->volumeRange = _volumeRange;
}
