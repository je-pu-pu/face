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

	float mouse_size_ratio_min_ = 0.08f;	///< ボリュームが最小になる時の口の大きさ ( 顔の大きさに対する比率 )
	float mouse_size_ratio_max_ = 0.275f;	///< ボリュームが最大になる時の口の大きさ ( 顔の大きさに対する比率 )

	common::chase_value< float > volume_;	///< ボリューム
	common::chase_value< float > speed_;	///< 再生速度

    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;
	ofSoundPlayer sound_player_;
};
