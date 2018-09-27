#include "ofApp.h"
#include "Tone.h"
#include "math.h"

ofApp::ofApp()
	: volume_( 0.f, 0.f, 0.01f )
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
    
	int n = 0;

	for ( auto& i : tracker.getInstances() )
	{
		auto nose       = i.getLandmarks().getImagePoint( 33 );
		auto mouse_up   = i.getLandmarks().getImagePoint( 49 );
		auto mouse_down = i.getLandmarks().getImagePoint( 57 );

		auto mouse_size_ratio = ( mouse_down.y - mouse_up.y ) / i.getBoundingBox().getHeight();

		const auto mouse_size_ratio_min = 0.08f;
		const auto mouse_size_ratio_max = 0.3f;
		
		auto volume = ( mouse_size_ratio - mouse_size_ratio_min ) / ( mouse_size_ratio_max - mouse_size_ratio_min );
		volume = std::min( 1.f, std::max( 0.f, volume ) );

		ofDrawBitmapStringHighlight( "mouse up   : " + ofToString( mouse_up         ), 100, 100 );
		ofDrawBitmapStringHighlight( "mouse down : " + ofToString( mouse_down       ), 100, 120 );
		ofDrawBitmapStringHighlight( "mouse size : " + ofToString( mouse_size_ratio ), 100, 140 );
		ofDrawBitmapStringHighlight( "volume     : " + ofToString( volume           ), 100, 160 );

		/*
		ofDrawBitmapStringHighlight( "pose : " + ofToString( i.getPoseMatrix().getRotate().getEuler() ), 0, 500 );

		auto& pl = i.getLandmarks().getImageFeature( ofxFaceTracker2Landmarks::OUTER_MOUTH );

		for ( auto& p : pl )
		{
			ofDrawBitmapStringHighlight( ofToString( p ), 100, n * 20 );

			n++;
		}
		*/

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

			const auto face_x_ratio = ( ( ofGetWidth() - face_x ) / ofGetWidth() - 0.5f ) * tone_scaling_ + 0.5f;
			const auto tone_index = math::clamp< int >( static_cast< int >( face_x_ratio  * tones.size() ), 0, tones.size() - 1 );

			speed_.target_value() = tones[ tone_index ] / Tone::C5;

			ofDrawBitmapStringHighlight( "face x ratio : " + ofToString( face_x_ratio ) + ", " + ofToString( tone_index ), 100, 180 );
			ofDrawBitmapStringHighlight( "tone : " + ofToString( tones[ tone_index ] ) + ", " + ofToString( tone_index ), 100, 200 );
		}
		else
		{
			speed_.target_value() = speed;
		}

		volume_.target_value() = volume;
		

		volume_.chase();
		speed_.chase();

		// auto volume = 1.f;

		sound_player_.setSpeed( speed_.value() );
		sound_player_.setVolume( volume_.value() );

		/*
		for ( auto& p : i.getLandmarks().getImagePoints() )
		{
			ofDrawBitmapStringHighlight( "( " + ofToString( p.x ) + ", " + ofToString( p.y ) + " )", 100, n * 20 );

			n++;
		}
		*/
	}

	ofDrawBitmapStringHighlight( ofToString( mouseX ) + ", " + ofToString( mouseY ), 200, 200 );

    // Draw text UI
    ofDrawBitmapStringHighlight("Framerate : "+ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapStringHighlight("Tracker thread framerate : "+ofToString(tracker.getThreadFps()), 10, 40);
	ofDrawBitmapStringHighlight("music mode : " + ofToString( is_music_mode_ ), 10, 60);
	ofDrawBitmapStringHighlight("tone scaling : " + ofToString( tone_scaling_ ), 10, 80 );
    
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
	else if ( key == 'z' )
	{
		tone_scaling_ -= 0.1f;
	}
	else if ( key == 'x' )
	{
		tone_scaling_ += 0.1f;
	}
}