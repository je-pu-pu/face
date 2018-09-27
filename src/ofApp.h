// ofxFaceTrack2 example: SIMPLE EXAMPLE
//
// This example shows minimal usage of the ofxFaceTracker2 addon,
// doing simple landmark detection and pose estimation
//

#pragma once

#include "ofMain.h"
#include "ofxFaceTracker2.h"

#include "chase_value.h"

class ofApp : public ofBaseApp{
public:
	ofApp();

    void setup();
    void update();
    void draw();

	void keyPressed( int );
    
	bool is_music_mode_ = false;
	float tone_scaling_ = 2.5f;

	common::chase_value< float > volume_;
	common::chase_value< float > speed_;

    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;
	ofSoundPlayer sound_player_;
};
