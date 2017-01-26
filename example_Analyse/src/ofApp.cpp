#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup(){
    auto sampleRate = 44100;
    auto bufferSize = 512;
    
    analyser.setup( sampleRate,
                    bufferSize * 2,
                    bufferSize,
                    bufferSize / 2,
                    100 ); // call before ofSoundStreamSetup()
    analyser.setOnsetTimeSteps( 5 );
    analyser.setOnsetThreshold(0.015);

    std::cout << "Octave Analyser - Nr of averages: " << analyser.getOctaveAverages().size() << "\n";

    ofxMaxiSettings::setup( sampleRate, 1, bufferSize );
    ofSoundStreamSetup( 1, 1, this, sampleRate, bufferSize, 1 );

    gui.setup( "SoundAnalyser" );
    gui.add( magnitude.setup( "Strength", 5, 0, 10 ) );
}

//--------------------------------------------------------------
void ofApp::update(){
//    analyser.update( recorder.getBufferCurrent() );
}

void ofApp::checkOctaves() {
    vector< float > averages = analyser.getOctaveAverages();
    //ofLog() << "averages count: " << averages.size();
    for( int i = 20; i < averages.size(); i++ ) {
        float value = averages.at(i);
        if( value > 2.0 ) {
            //ofLog() << i << " - " << averages.at(i);
            harmonicsCount += 1.0;
//            ofLog() << "harmonicness: " << (harmonicsCount / ofGetFrameNum() );
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient( ofColor::gray, ofColor::black );

    drawOnset();
    drawBuffer( magnitude );
    drawFFT( magnitude );
    drawOctave( magnitude);

    drawInfo();

    gui.draw();
}

//--------------------------------------------------------------

void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
    analyser.grabAudioBuffer( input );
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

void ofApp::drawInfo()
{
    ofDrawBitmapStringHighlight( "Time: " + ofToString( ofGetElapsedTimeMillis() / 1000 ) , 100, 150 );
    ofDrawBitmapStringHighlight( "RMS: " + ofToString(          analyser.getRMS() ), 100, 200 );
    ofDrawBitmapStringHighlight( "MaxRMS: " + ofToString(       analyser.getMaxRMS() ), 100, 220 );
    ofDrawBitmapStringHighlight( "AMP: " + ofToString(          analyser.getAmplitude() ), 100, 240 );
    ofDrawBitmapStringHighlight( "MaxAMP: " + ofToString(       analyser.getMaxAmplitude() ), 100, 260 );
    ofDrawBitmapStringHighlight( "Onset: " + ofToString(        analyser.getIfOnset() ), 100, 280 );
    ofDrawBitmapStringHighlight( "FFT Centroid: " + ofToString( analyser.getFFTspectralCentroid() ), 100, 300 );
    ofDrawBitmapStringHighlight( "FFT Centroid Min: " + ofToString( analyser.getFFTspectralCentroidMin() ), 100, 320 );
    ofDrawBitmapStringHighlight( "FFT Centroid Max: " + ofToString( analyser.getFFTspectralCentroidMax() ), 100, 340 );
    ofDrawBitmapStringHighlight( "FFT Flatness: " + ofToString( analyser.getFFTspectralFlatness() ), 100, 360 );
    ofDrawBitmapStringHighlight( "FFT Flatness Min: " + ofToString( analyser.getFFTspectralFlatnessMin() ), 100, 380 );
    ofDrawBitmapStringHighlight( "FFT Flatness Max: " + ofToString( analyser.getFFTspectralFlatnessMax() ), 100, 400 );
}

void ofApp::drawOctave( float magnScale )
{
    ofSetColor( ofColor::blue );
    int xStep = ofGetWidth() / analyser.getOctaveAverages().size();
    int yLine = ofGetHeight() * 3 / 4;
    float height = 0.0;
    int i = 0;
    for( auto &_octaveAverage : analyser.getOctaveAverages() )
    {
        height = _octaveAverage * 20 * 3 / 2;
        ofDrawRectangle( xStep * i++, yLine, xStep,  -( height * magnScale ) );
    }
}

void ofApp::drawFFT( float magnScale )
{
    int i  = 0;
    int xStep = ofGetWidth() / analyser.getFFTSpectralMagnitudes().size();
    int yLine = ofGetHeight() / 3;

    ofSetColor(0);
    ofSetLineWidth(2);
    ofDrawLine( 0, yLine, ofGetWidth(), yLine );

    ofSetColor( ofColor::red, 255 );
    for( auto &magnMax : analyser.getFFTSpectralMagnitudesMaximums() ) {
        ofDrawRectangle( xStep * i++, yLine, xStep,  -( magnMax * magnScale ) );
    }

    ofSetColor( ofColor::lightGreen, 255 );
    i = 0;
    for( auto &magn : analyser.getFFTSpectralMagnitudes() ) {
        ofDrawRectangle( xStep * i++, yLine, xStep,  -( magn * magnScale ) );
    }
}

void ofApp::drawOnset()
{
    if ( analyser.getIfOnset() )
    {
        ofSetColor( ofColor::wheat, 255 );
        ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() / 10 );

        checkOctaves();
    }
}

void ofApp::drawBuffer( int scale )
{
    ofPushStyle();
    float y1 = ofGetHeight() * 0.5;
    ofSetColor(0);
    ofSetLineWidth(2);
    ofDrawLine(0, y1, ofGetWidth(), y1);

    // --- Drawing Buffer TimeLine
    ofSetColor( ofColor::lightBlue );
    ofSetLineWidth(1);
    for( int i=0; i < analyser.getAudioBuffer().size(); i++){
        float p = i / (float)( analyser.getAudioBuffer().size() - 1 );
        float x = p * ofGetWidth();
        float y2 = y1 + analyser.getAudioBuffer()[i] * scale * 100;

        ofDrawLine(x, y1, x, y2);
    }
    ofPopStyle();
}
//--------------------------------------------------------------

