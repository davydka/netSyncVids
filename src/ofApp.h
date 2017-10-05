#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"
#include "ofxOsc.h"
// #include "ofxMidi.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    
    ofxHPVPlayer player;

	ofxOscSender masterSender;
	ofxOscSender player1;
	ofxOscSender player2;
	ofxOscReceiver masterReceiver;
	ofxOscReceiver receiver1;
	ofxOscReceiver receiver2;

	vector<string> arguments;
};
