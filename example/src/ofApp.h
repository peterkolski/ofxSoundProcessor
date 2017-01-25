#pragma once

#include "ofxMaxim.h"
#include "ofxGui.h"
#include "sndRecorder.hpp"
#include "sndAnalyser.hpp"


class ofApp : public ofxiOSApp {
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    void audioIn(  float * input, int bufferSize, int nChannels );
    void audioOut( float * output, int bufferSize, int nChannels );
    
    void checkOctaves();
    double harmonicsCount;
    
    // --- Sound Input
    maxiSample      maxiPlayer;
    bool            playMaxi = false;
    bool            useMic = false;
//    ofSoundPlayer   soundFile;
    snd::Recorder   recorder;
    snd::Analyser   analyser;
    int             sampleRate;
    
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
};
