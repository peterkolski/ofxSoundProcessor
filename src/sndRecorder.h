//
//  sndRecorder.hpp
//  SoundAnalyser
//
//  Created by Peter A. Kolski on 25.02.16.
//
//

#ifndef sndRecorder_hpp
#define sndRecorder_hpp

#include <iostream>
#include <vector>

using namespace std;

namespace snd {
    
    class Recorder
    {
        void                resetBufferCurrent()    { bufferCurrent_.assign( bufferSizeCurrent_, 0.0 ); }
        
        bool                doGrab_             = false;
        bool                doRecord_           = false;
        bool                doPlay_             = false;
        bool                doLoop_             = false;
        int                 bufferSizeCurrent_  = 512;
        int                 sampleRate_         = 44100;
        int                 playNrBuffer_       = 0;
        vector< float >     bufferCurrent_;
        vector< float >     bufferAllSample_;
        
    public:
        void                init( int bufferSize, int sampleRate );
        void                grabAudioBuffer(   float * input );
        void                outputAudioBuffer( float * output );
        
        void                startPlaying();
        void                startGrabbingSound()    { doGrab_    = true; }
        void                startRecording();
        
        void                stopPlaying()           { doPlay_    = false; }
        void                stopGrabbingSound()     { doGrab_    = false; doRecord_ = false; }
        void                stopRecording()         { doRecord_  = false; }
        
        bool                isPlaying()             { return doPlay_; }
        bool                isRecording()           { return doRecord_; }
        bool                isGrabbing()            { return doGrab_; }
        
        void                loopPlayback()          { doLoop_    = true; }
        void                noLoopPlayback()        { doLoop_    = false; }
        void                restartPlaying()        { playNrBuffer_ = 0; }
        void                resetBuffer();
        
        int                 getSampleRate()         { return sampleRate_; }
        int                 getBufferSizeCurrent()  { return bufferSizeCurrent_; }
        vector< float >    &getBufferCurrent()      { return bufferCurrent_; }
        vector< float >    &getBufferRecorded()     { return bufferAllSample_; }

        // --- Max buffer. Prob not needed
        //        void                setSampleBufferSize( int size )     { sampleBufferSize = size;}
        //        int                 getSampleBufferSize()   { return sampleBufferSize; }
    };
    
}

#endif /* sndRecorder_hpp */
