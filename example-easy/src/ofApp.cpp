#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);	
	ofSetFrameRate(60);
	fft.setup(16384);
}

void ofApp::update() {
	fft.update();
}

void ofApp::draw() {
	ofBackground(0, 0, 0);
	
	ofPushMatrix();
	ofTranslate(16, 16);
	ofSetColor(255);
	ofDrawBitmapString("Frequency Domain", 0, 0);
	plot(fft.getBins(), 128);
	ofPopMatrix();
	
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
}

void ofApp::plot(vector<float>& buffer, float scale) {
	ofNoFill();
	int n = MIN(1024, buffer.size());
	ofDrawRectangle(0, 0, n, scale);
	ofPushMatrix();
	ofTranslate(0, scale);
	ofScale(1, -scale);
	ofBeginShape();
	for (int i = 0; i < n; i++) {
		ofVertex(i, buffer[i]);
	}
	ofEndShape();
	ofPopMatrix();
}
