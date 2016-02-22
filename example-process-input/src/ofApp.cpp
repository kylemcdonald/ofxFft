#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);	
	ofSetFrameRate(60);
	fft.setup();
    
}

void ofApp::update() {
	fft.update();
}

void ofApp::draw() {
    fft.drawBars();
    fft.drawDebug();
    
    ofNoFill();
    ofDrawRectangle(824, 0, 200, 200);
    ofDrawRectangle(824, 200, 200, 200);
    ofDrawRectangle(824, 400, 200, 200);
    ofDrawRectangle(824, 600, 200, 200);
    
    fft.drawHistoryGraph(ofPoint(824,0), LOW);
    fft.drawHistoryGraph(ofPoint(824,200),MID );
    fft.drawHistoryGraph(ofPoint(824,400),HIGH );
    fft.drawHistoryGraph(ofPoint(824,600),MAXSOUND );
    ofDrawBitmapString("LOW",850,20);
    ofDrawBitmapString("HIGH",850,420);
    ofDrawBitmapString("MID",850,220);
    ofDrawBitmapString("MAX VOLUME",850,620);
    
    ofSetColor(0);
    ofDrawBitmapString("Press 'r' or 'q' to toggle normalization of values", 20,320);
    
}

void ofApp::keyPressed(int key){
    
    //toggle between normalized and not to get a sense of the effects
    //You will need to set a volume range if you're not normalizing everything - but this will depend on your sound source and input type to determine the maximum volume range of your codez
    
    if(key=='q'){
        fft.setVolumeRange(100);
        fft.setNormalize(false);
    }
    if(key=='r'){
        fft.setNormalize(true);
    }
}
