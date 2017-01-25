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
    void audioOut( float * output, int bufferSize, int nChannels );
    
    void checkOctaves();
    double harmonicsCount;
    
    // --- Sound Input
    ofxSoundRecorder    recorder;
    ofxSoundAnalyser    analyser;
    maxiSample          maxiPlayer;
    bool                playMaxi = false;
    bool                useMic = true;
    int                 sampleRate;
    
    void    stopRec()   { recorder.stopRecording(); }
    void    stopGrb()   { recorder.stopGrabbingSound(); }
    void    stopPlay()  { recorder.stopPlaying(); }
    void    startRec()  { recorder.startRecording(); }
    void    startGrb()  { recorder.startGrabbingSound(); }
    void    startPlay() { recorder.startPlaying(); }
    void    resetBuff() { recorder.resetBuffer(); }
    void    restartPlay() { recorder.restartPlaying(); }
    void    resetMinMax() { analyser.resetAllMinMax(); }
    void    loopPlay()    { recorder.loopPlayback(); }
    void    noLoopPlay()  { recorder.noLoopPlayback(); }
    
    // --- Sound Playback
    ofxPanel        gui;
    ofxButton       startPlaying, startGrabbingSound, startRecording;
    ofxButton       stopPlaying, stopGrabbingSound, stopRecording;
    ofxButton       restartPlaying, resetBuffer, loopPlayback, noLoopPlayback;
    ofxButton       resetAllMinMax;
    ofxFloatSlider  magnitude;


    void drawOnset();
    void drawFFT( float magnScale );
    void drawOctave( float magnScale );
    void drawInfo();
};
