#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);	
	fft.setup(1024);
}

void testApp::update() {
	fft.update();
}

void testApp::draw() {
	ofBackground(0, 0, 0);
	
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(16, 16);
	
	ofDrawBitmapString("Frequency Domain", 0, 0);
	plot(fft.getBins(), 128);
	ofPopMatrix();
	string msg = ofToString((int) ofGetFrameRate()) + " fps";
	ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
}

void testApp::plot(vector<float>& buffer, float scale) {
	ofNoFill();
	int n = buffer.size();
	ofRect(0, 0, n, scale);
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