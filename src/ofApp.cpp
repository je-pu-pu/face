#include "ofApp.h"
#include "Tone.h"
#include "math.h"

ofApp::ofApp()
	: volume_( 0.f, 0.f, 0.025f )
	, speed_( 1.f, 1.f, 0.1f )
{

}

//--------------------------------------------------------------
void ofApp::setup(){
    // All examples share data files from example-data, so setting data path to this folder
    // This is only relevant for the example apps
    ofSetDataPathRoot( "./data/model/" );
    
    // Setup grabber
    grabber.setup(1280,720);
    
    // Setup tracker
    tracker.setup();

	ofSetDataPathRoot( "./data/" );
	sound_player_.load( "a.wav" );
	sound_player_.setLoop( true );
	sound_player_.setVolume( 0.f );
	sound_player_.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
    // Update tracker when there are new frames
    if(grabber.isFrameNew()){
        tracker.update(grabber);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw camera image
    grabber.draw(0, 0);
    
    // Draw tracker landmarks
    tracker.drawDebug();
    
    // Draw estimated 3d pose
    // tracker.drawDebugPose();
    
	for ( auto& i : tracker.getInstances() )
	{
		auto nose       = i.getLandmarks().getImagePoint( 33 );
		auto mouse_up   = i.getLandmarks().getImagePoint( 49 );
		auto mouse_down = i.getLandmarks().getImagePoint( 57 );

		auto mouse_size_ratio = ( mouse_down.y - mouse_up.y ) / i.getBoundingBox().getHeight();

		auto volume = ( mouse_size_ratio - mouse_size_ratio_min_ ) / ( mouse_size_ratio_max_ - mouse_size_ratio_min_ );
		volume = std::min( 1.f, std::max( 0.f, volume ) );

		ofDrawBitmapStringHighlight( "mouse up   : " + ofToString( mouse_up         ), 10, 200 );
		ofDrawBitmapStringHighlight( "mouse down : " + ofToString( mouse_down       ), 10, 220 );
		ofDrawBitmapStringHighlight( "mouse size : " + ofToString( mouse_size_ratio ), 10, 240 );
		ofDrawBitmapStringHighlight( "volume     : " + ofToString( volume           ), 10, 260 );

		const auto face_x = nose.x; // ( ( i.getBoundingBox().getLeft() + i.getBoundingBox().getRight() ) / 2.f );
		const auto speed = ( ofGetWidth() - face_x ) / ( ofGetWidth() / 2.f );

		if ( is_music_mode_ )
		{
			/*
			constexpr std::array< float, 15 > tones = {
				Tone::C4, Tone::D4, Tone::E4, Tone::F4, Tone::G4, Tone::A4, Tone::B4,
				Tone::C5, Tone::D5, Tone::E5, Tone::F5, Tone::G5, Tone::A5, Tone::B5,
				Tone::C6
			};
			*/
			
			constexpr std::array< float, 11 > tones = {
				Tone::C4, Tone::D4, Tone::E4, Tone::G4, Tone::A4,
				Tone::C5, Tone::D5, Tone::E5, Tone::G5, Tone::A5,
				Tone::C6
			};

			/*
			constexpr std::array< float, 9 > tones = {
				Tone::C4, Tone::D4, Tone::E4, Tone::G4,
				Tone::C5, Tone::D5, Tone::E5, Tone::G5,
				Tone::C6
			};
			*/

			const auto face_x_ratio = ( ( ofGetWidth() - face_x ) / ofGetWidth() - 0.5f ) * tone_scaling_ + 0.5f;
			const auto tone_index = math::clamp< int >( static_cast< int >( face_x_ratio  * tones.size() ), 0, tones.size() - 1 );

			speed_.target_value() = tones[ tone_index ] / Tone::C5;

			ofDrawBitmapStringHighlight( "face x ratio : " + ofToString( face_x_ratio ) + ", " + ofToString( tone_index ), 10, 280 );
			ofDrawBitmapStringHighlight( "tone : " + ofToString( tones[ tone_index ] ) + ", " + ofToString( tone_index ), 10, 300 );
		}
		else
		{
			speed_.target_value() = speed;
		}

		volume_.target_value() = volume;

		break;
	}

	if ( volume_.value() < volume_.target_value() )
	{
		// volume_.fit_to_target();
		volume_.chase();
		volume_.chase();
	}
	else
	{
		volume_.chase();
	}

	speed_.chase();

	sound_player_.setSpeed( speed_.value() );
	sound_player_.setVolume( volume_.value() );

    // Draw text UI
    ofDrawBitmapStringHighlight( "FPS : " + ofToString( ofGetFrameRate() ), 10, 20 );
    ofDrawBitmapStringHighlight( "Tracker thread FPS : "+ofToString( tracker.getThreadFps() ), 10, 40 );
	ofDrawBitmapStringHighlight( "music mode : " + ofToString( is_music_mode_ ), 10, 60 );
	ofDrawBitmapStringHighlight( "tone scaling : " + ofToString( tone_scaling_ ), 10, 80 );
	ofDrawBitmapStringHighlight( "mouse size min : " + ofToString( mouse_size_ratio_min_ ), 10, 100 );
	ofDrawBitmapStringHighlight( "mouse size max : " + ofToString( mouse_size_ratio_max_ ), 10, 120 );
	// ofDrawBitmapStringHighlight( "mouse size     : " + ofToString( mouse_size_ratio_ ), 10, 140 );
    
#ifndef __OPTIMIZE__
    ofSetColor(ofColor::red);
    ofDrawBitmapString("Warning! Run this app in release mode to get proper performance!",10,60);
    ofSetColor(ofColor::white);
#endif
}

void ofApp::keyPressed( int key )
{
	if ( key == 'm' )
	{
		is_music_mode_ = ! is_music_mode_;
	}
	else if ( key == 'q' )
	{
		tone_scaling_ -= 0.1f;
	}
	else if ( key == 'w' )
	{
		tone_scaling_ += 0.1f;
	}
	else if ( key == 'a' )
	{
		mouse_size_ratio_min_ -= 0.001f;
	}
	else if ( key == 's' )
	{
		mouse_size_ratio_min_ += 0.001f;
	}
	else if ( key == 'z' )
	{
		mouse_size_ratio_max_ -= 0.001f;
	}
	else if ( key == 'x' )
	{
		mouse_size_ratio_max_ += 0.001f;
	}
}