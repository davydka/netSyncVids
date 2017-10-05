#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"
#include "ofxNetworkSync.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(int key);

	void onMessageComing(string & message);
    
    ofxHPVPlayer player;

	ofxNetworkSyncServer server;
	ofxNetworkSyncClient client;

	vector<string> arguments;
};
