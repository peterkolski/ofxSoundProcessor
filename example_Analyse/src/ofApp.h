#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSoundAnalyser.h"


class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();

    void audioIn(  float * input, int bufferSize, int nChannels );
    
    // --- Sound Input
    ofxSoundAnalyser    analyser;


    ofxPanel        gui;
    ofxFloatSlider  magnitude;


    void drawOnset();
    void drawFFT( float magnScale );
    void drawOctave( float magnScale );
    void drawInfo();
    void drawBuffer( int scale );
    void checkOctaves();
    double harmonicsCount;
};
