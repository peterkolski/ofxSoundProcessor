//
//  sndRecorder.cpp
//  SoundAnalyser
//
//  Created by Peter A. Kolski on 25.02.16.
//
//

#include "sndRecorder.h"

namespace snd {
    void Recorder::init( int bufferSize, int samplerate  )
    {
        bufferSizeCurrent_   = bufferSize;
        sampleRate_          = samplerate;
        bufferCurrent_.assign( bufferSizeCurrent_, 0.0 );
    }
    
    void Recorder::resetBuffer()
    {
        bufferAllSample_.clear();
        resetBufferCurrent();
    }
    
    void Recorder::grabAudioBuffer( float * input )
    {
        if ( doGrab_ )
        {
            for (int i = 0; i < bufferCurrent_.size(); i++)
            {
                bufferCurrent_[ i ] = input[ i ];
            }
        }
        if ( doRecord_ )
        {
            for (int i = 0; i < bufferCurrent_.size(); i++)
            {
                bufferAllSample_.push_back( input[ i ] );
            }
        }
    }
    
    void Recorder::outputAudioBuffer( float *output )
    {
        //grabAudioBuffer( output );
        
        if ( doPlay_ )
        {
            int startBufferID = bufferCurrent_.size() * playNrBuffer_;
            
            if ( startBufferID < bufferAllSample_.size() ) // reaching the end?
            {
                for ( int i = 0; i < bufferCurrent_.size(); i++ ) {
                    bufferCurrent_[ i ] = bufferAllSample_[ startBufferID + i ];
                    output[ i ]         = bufferCurrent_[ i ];
                }
                playNrBuffer_++;
            }
            else
            {
                // --- LOOPING or not?
                if ( doLoop_ )
                {
                    restartPlaying();
                    resetBufferCurrent();
                }
                else
                {
                    stopPlaying();
                }
            }
        }
        if ( !doPlay_ && !doRecord_ )
        {
            // --- TODO: Does this block the MP3 output?
            resetBufferCurrent();
            for ( int i = 0; i < bufferCurrent_.size(); i++ ) {
                output[ i ] = 0.0;
            }
        }
    }
    
    void Recorder::startPlaying()
    {
        doPlay_ = true;
        stopRecording();
        stopGrabbingSound();
    }
    
    void Recorder::startRecording()
    {
        doRecord_   = true;
        startGrabbingSound();
        stopPlaying();
    }
}

