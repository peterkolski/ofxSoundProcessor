//
//  sndAnalyser.hpp
//  SoundAnalyser
//
//  Created by Peter A. Kolski on 27.02.16.
//
//

#ifndef sndAnalyser_hpp
#define sndAnalyser_hpp

//#include "ofxiOS.h"
#include "ofxMaxim.h"
#include <vector>
#include <queue>

using namespace std;

namespace snd
{

    class Analyser
    {
    public:
        void    setup( float samplingRate, int fftSize,  int sampleSize, int hopSize, int nAveragesPerOctave );
        void    update( vector< float > &bufferCurrent );
        void    resetAllMinMax();
        void    resetMaxRMS()               { maxRMS_ = 0.0; }
        void    resetMaxAmplitude()         { maxAmplitude_ = 0.0; }
        void    resetMaxFFTMagnitudesMax()  { spectralMagnitudesMax_ = vector< float >( spectralFFTsize_, 0.0 ); }
        void    resetFFTMinMax();
        
        void    setOnsetTimeSteps( int steps )          { onsetTimeSteps_ = steps; }
        void    setOnsetThreshold( float threshold )    { onsetDiffThreshold_ = threshold; }
        
        float   getRMS()            { return rmsCurrent_; }
        float   getAmplitude()      { return amplitudeCurrent_; }
        float   getMaxRMS()         { return maxRMS_; }
        float   getMaxAmplitude()   { return maxAmplitude_; }
        bool    getIfOnset()        { return onsetIsNow_; }
        
        float   getFFTspectralCentroid()    { return centroidCurrent_; }
        float   getFFTspectralCentroidMin() { return minCentroid_; }
        float   getFFTspectralCentroidMax() { return maxCentroid_; }
        float   getFFTspectralFlatness()    { return flatnessCurrent_; }
        float   getFFTspectralFlatnessMin() { return minFlatness_; }
        float   getFFTspectralFlatnessMax() { return maxFlatness_; }
        float   getFFTspectralFlatnessAvg();
        float   getFFTspectralRmsAverage();
        double     getOctaveHarmonicsCount( float threshold );

        vector< float > &getFFTSpectralMagnitudes()            { return spectralMagnitudes_; }
        vector< float > &getFFTSpectralMagnitudesMaximums()    { return spectralMagnitudesMax_; }
        vector< float > &getFFTSpectralMagnitudesDB()          { return spectralMagnitudesDB_; }
        vector< float > &getOctaveAverages()                   { return octaveAverages_; }
        
    private:
        void    updateOnsetDetection( vector< float > &bufferCurrent );
        void    updateFFT_Octave( vector< float > &bufferCurrent );
        float   calculateRMS( vector< float > &bufferCurrent );
        float   calculateAmplitude( vector< float > &bufferCurrent );
        
        // --- changable values
        float   rmsCurrent_         = 0.0;
        float   amplitudeCurrent_   = 0.0;
        float   flatnessCurrent_    = 0.0;
        float   centroidCurrent_    = 0.0;
        float   maxRMS_             = 0.0;
        float   maxAmplitude_       = 0.0;
        float   maxCentroid_        = 0.0;
        float   maxFlatness_        = 0.0;
        float   minCentroid_        = 1000000000;
        float   minFlatness_        = 1000000000;
        double  averageFlatness_    = 0.0;
        double  averageRms_         = 0.0;
        
        // --- parameters
        int     onsetTimeSteps_     = 2;
        float   onsetDiffThreshold_ = 0.01;
        bool    onsetIsNow_         = false;
        int     spectralFFTsize_    = 512;
        long    numberFrames_       = 0;
        double  harmonicsCount      = 0;
        
        vector< float >             onsetMeasureValue_;
        vector< float >             onsetValueDifference_;
        vector< float >             spectralMagnitudes_;
        vector< float >             spectralMagnitudesMax_;
        vector< float >             spectralMagnitudesDB_;
        vector< float >             octaveAverages_;
        vector< vector< float > >   onsetOctaveAverages_;
        
        ofxMaxiFFT                  spectralFFT_;
        ofxMaxiFFTOctaveAnalyzer    octaveAnalyser_;

    };
}


#endif /* sndAnalyser_hpp */
