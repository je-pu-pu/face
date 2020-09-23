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

	float mouse_size_ratio_min_ = 0.08f;	///< �{�����[�����ŏ��ɂȂ鎞�̌��̑傫�� ( ��̑傫���ɑ΂���䗦 )
	float mouse_size_ratio_max_ = 0.275f;	///< �{�����[�����ő�ɂȂ鎞�̌��̑傫�� ( ��̑傫���ɑ΂���䗦 )

	common::chase_value< float > volume_;	///< �{�����[��
	common::chase_value< float > speed_;	///< �Đ����x

    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;
	ofSoundPlayer sound_player_;
};
