#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    
    ofxHPVPlayer player;

	ofxOscSender sender;
	ofxOscReceiver receiver;

	vector<string> arguments;
};
