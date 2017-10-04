#include "ofApp.h"

int SIDE_WIDTH;
int SIDE_HEIGHT;

int cur_counter;
int prev_counter;

static int64_t prev_frame = 0;
static int64_t cur_frame = 0;
static uint8_t offset = 1;

bool master = false;
int playerId = 0;
#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------------
void ofApp::setup()
{
    HPV::InitHPVEngine();
    
	// cout << arguments[0] << endl;
	/*
	if(arguments[0] == 1){
		cout << "player 1" << endl;
	}
	*/
	cout << "----" << endl;
	for (int i=0; i<arguments.size(); ++i){
		cout << arguments.at(i) << endl;
		if(arguments.at(i) == "1") {
			master = true;
		}
		if(arguments.at(i) == "1" || arguments.at(i) == "2" || arguments.at(i) == "3" || arguments.at(i) == "4") {

			playerId = ofToInt(arguments.at(i));
		}

	}
	cout << "----" << endl;
	receiver.setup(PORT);
	sender.setup(HOST, PORT);
	cur_counter = 0;
	prev_counter = 0;

    player.init(HPV::NewPlayer());
	if(playerId == 1)
		player.load("top_left.hpv");
	else if(playerId == 2)
		player.load("top_right.hpv");
	else if(playerId == 3)
		player.load("bottom_left.hpv");
	else if(playerId == 4)
		player.load("bottom_right.hpv");
    player.setLoopState(OF_LOOP_NORMAL);
    player.setDoubleBuffered(true);
	
    player.play();
    player.setPaused(true);
    
    ofSetVerticalSync(false);
    ofSetFrameRate(15);
    
	ofSetWindowPosition(0, 0);
    
	if(playerId == 1)
		ofSetWindowPosition(0, 0);
	else if(playerId == 2)
		ofSetWindowPosition(ofGetWidth()+offset, 0);
	else if(playerId == 3)
		ofSetWindowPosition(0, ofGetHeight()+offset);
	else if(playerId == 4)
		ofSetWindowPosition(ofGetWidth()+offset, ofGetHeight()+offset);
}

//--------------------------------------------------------------
void ofApp::update() {
	
	// cur_counter++;
	if (cur_counter >= player.getTotalNumFrames()){
		cur_counter = 0;
	}
	ofxOscMessage m;
	m.setAddress("/frame");
	m.addIntArg(cur_counter);
	sender.sendMessage(m, false);
    
	if (cur_frame != prev_frame){
		player.seekToFrame(cur_counter);
		prev_frame = cur_frame;
	}
    
	cur_counter++;
	cur_frame++;

	if (cur_frame >= player.getTotalNumFrames()){
		cur_frame = 0;
	}

	HPV::Update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(255);
	player.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::exit()
{
    HPV::DestroyHPVEngine();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}
