//
//  sndAnalyser.cpp
//  SoundAnalyser
//
//  Created by Peter A. Kolski on 27.02.16.
//
//

/*
    - magnitudes
    - Peak frequ
 */

#include "sndAnalyser.h"

#include <numeric>
#include <cmath>
#include <iostream>

namespace snd
{
    void    Analyser::setup( float samplingRate, int fftSize,  int sampleSize, int hopSize, int nAveragesPerOctave )
    {
//        ofLogWarning() << "Analyser setup should be called before 'ofSoundStreamSetup()'";
//        ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
        
        spectralFFTsize_        = fftSize;
        spectralMagnitudes_     = vector< float >( spectralFFTsize_, 0.0 );
        spectralMagnitudesMax_  = vector< float >( spectralFFTsize_, 0.0 );
        spectralMagnitudesDB_   = vector< float >( spectralFFTsize_, 0.0 );
        
        spectralFFT_.setup( spectralFFTsize_, sampleSize, hopSize ); // args: (fftSize,  windowSize,  hopSize)
        octaveAnalyser_.setup( samplingRate, fftSize, nAveragesPerOctave ); // arg: samplingRate, nBandsInTheFFT,  nAveragesPerOctave

        octaveAverages_         = vector< float >( octaveAnalyser_.nAverages, 0.0 ); // after setup, so nAverages is set
    }
    
    //--------------------------------------------------------------
    
    void Analyser::update( vector<float> &bufferCurrent )
    {
        calculateAmplitude( bufferCurrent );
        calculateRMS( bufferCurrent );
        updateOnsetDetection( bufferCurrent );
        updateFFT_Octave( bufferCurrent );
    }
    
    //--------------------------------------------------------------
    
    void    Analyser::updateFFT_Octave( vector<float> &bufferCurrent )
    {
        // --- Process FFT & OCTAVE
        for ( const auto &singleVal : bufferCurrent )
        {
            if ( spectralFFT_.process( singleVal ) )
            {
                spectralFFT_.magsToDB();
                octaveAnalyser_.calculate( spectralFFT_.magnitudesDB ); // TODO: check if possible to use in seperate method
            }
        }
        
        // --- Write FFT to vector
        for ( int i = 0; i < spectralFFTsize_; i++ )
        {
            spectralMagnitudesMax_[ i ]     = spectralFFT_.magnitudes[ i ] > spectralMagnitudesMax_[ i ] ? spectralFFT_.magnitudes[ i ] : spectralMagnitudesMax_[ i ];
            spectralMagnitudes_[ i ]        = spectralFFT_.magnitudes[ i ];
            spectralMagnitudesDB_[ i ]      = spectralFFT_.magnitudesDB[ i ];
        }

        // --- Write OCTAVE to vector
        for ( int i = 0; i < octaveAnalyser_.nAverages; i++ )
        {
            octaveAverages_[ i ]    = octaveAnalyser_.averages[ i ];
        }

        
        // --- MinMax Centroid
        centroidCurrent_ = spectralFFT_.spectralCentroid();
        if ( centroidCurrent_ > 1.0 ) {
            minCentroid_    = centroidCurrent_ < minCentroid_ ?  centroidCurrent_ : minCentroid_;
        }
        maxCentroid_    = centroidCurrent_ > maxCentroid_ ? centroidCurrent_ : maxCentroid_;

        // --- Spectral flatness
        flatnessCurrent_ = spectralFFT_.spectralFlatness();
        if ( flatnessCurrent_ > 0.01 ) {
            minFlatness_    = flatnessCurrent_ < minFlatness_ ?  flatnessCurrent_ : minFlatness_;
        }
        maxFlatness_    = flatnessCurrent_ > maxFlatness_ ? flatnessCurrent_ : maxFlatness_;
        averageFlatness_ += flatnessCurrent_;
        
        numberFrames_++;
        
        if( getIfOnset() ) {
            onsetOctaveAverages_.push_back( getOctaveAverages() );
            for( int i = 20; i < octaveAverages_.size(); i++ ) {
                float value = octaveAverages_.at(i);
                if( value > 2.0 ) {
                    //ofLog() << i << " - " << averages.at(i);
                    harmonicsCount += 1.0;
                    //std::cout << "harmonicness: " << (harmonicsCount / numberFrames_ ) << std::endl;
                }
            }
        }
    }
    
    //--------------------------------------------------------------
    
    float Analyser::getFFTspectralFlatnessAvg()
    {
        if ( numberFrames_ == 0 ) {
            return 0.0;
        }
        else
        {
            return averageFlatness_ / (double)numberFrames_;
        }
    }
    
    //--------------------------------------------------------------
    
    float Analyser::getFFTspectralRmsAverage()
    {
        if ( numberFrames_ == 0 ) {
            return 0.0;
        }
        else
        {
            return averageRms_ / (double)numberFrames_;
        }
    }
    
    //--------------------------------------------------------------
    
    double Analyser::getOctaveHarmonicsCount( float threshold )
    {
        if( numberFrames_ == 0 ) {
            return 0.0;
        }
        
        return harmonicsCount / (double)(numberFrames_);
    }

    //--------------------------------------------------------------
    
    void    Analyser::resetAllMinMax()
    {
        resetMaxRMS();
        resetMaxAmplitude();
        resetMaxFFTMagnitudesMax();
        resetFFTMinMax();
        
        // TODO: exclude in independent function
        numberFrames_ = 0;
        onsetOctaveAverages_.clear();
        harmonicsCount = 0;
        averageRms_ = 0;
        averageFlatness_ = 0;
    }

    //--------------------------------------------------------------
    
    float   Analyser::calculateRMS( vector< float > &bufferCurrent )
    {
//   auto sqrSum = std::accumulate( begin( bufferCurrent), end( bufferCurrent ),
//                                  0.0, [](float val, float val2 ){ return val*val2; }  );

        rmsCurrent_ = 0.0;
        for ( auto &val : bufferCurrent )
        {
            rmsCurrent_ += val*val;
        }
        rmsCurrent_ /= (float)bufferCurrent.size();
        rmsCurrent_ = std::sqrt( rmsCurrent_ );
        
        averageRms_ += rmsCurrent_;

        // --- setting MinMax
        maxRMS_ = rmsCurrent_ > maxRMS_ ? rmsCurrent_ : maxRMS_;
        
        return rmsCurrent_;
    }
    
    //--------------------------------------------------------------
    
    float Analyser::calculateAmplitude( vector<float> &bufferCurrent )
    {
        amplitudeCurrent_ = 0.0;
        for ( auto &val : bufferCurrent )
        {
            amplitudeCurrent_ = std::abs( val ) > amplitudeCurrent_ ? std::abs( val ) : amplitudeCurrent_;
        }
        // --- setting MinMax
        maxAmplitude_ = amplitudeCurrent_ > maxAmplitude_ ? amplitudeCurrent_ : maxAmplitude_;
        return amplitudeCurrent_;
    }
    
    //--------------------------------------------------------------
    
    void Analyser::updateOnsetDetection( vector<float> &bufferCurrent )
    {
        float currentValue = calculateRMS( bufferCurrent ); // Alternatively amplitude
        
        // clear first val (should be the same)
        if ( onsetMeasureValue_.size() > onsetTimeSteps_ )
            onsetMeasureValue_.erase( onsetMeasureValue_.begin() );
        if ( onsetValueDifference_.size() > onsetTimeSteps_ )
            onsetValueDifference_.erase( onsetValueDifference_.begin() );


        
        if ( onsetMeasureValue_.size() && onsetValueDifference_.size() ) {
            if (currentValue - onsetMeasureValue_.back() > 0.0 )
                onsetValueDifference_.push_back( currentValue - onsetMeasureValue_.back() );
            else
                onsetValueDifference_.push_back( 0.0 );
            
            onsetMeasureValue_.push_back( currentValue );
//            std::cout << std::accumulate( onsetDiff_.begin() , onsetDiff_.end(), 0.0 ) << "\n";
            onsetIsNow_ = ( onsetDiffThreshold_ < std::accumulate( onsetValueDifference_.begin() , onsetValueDifference_.end(), 0.0 ) );
        }
        else
        {
            onsetMeasureValue_.push_back( currentValue );
            onsetValueDifference_.push_back( 0.0 );
        }
    }
    
    void Analyser::resetFFTMinMax()
    {
        flatnessCurrent_    = 0.0;
        minFlatness_        = 10000000.0;
        maxFlatness_        = 0.0;
        
        centroidCurrent_    = 0.0;
        minCentroid_        = 10000000.0;
        maxCentroid_        = 0.0;
        
    }
}