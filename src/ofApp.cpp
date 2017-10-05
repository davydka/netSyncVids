#include "ofApp.h"

int SIDE_WIDTH;
int SIDE_HEIGHT;

int cur_frame = 0;
int received_frame = 0;
// int prev_counter;

static int64_t prev_frame = 0;
// static int64_t cur_frame = 0;
static uint8_t offset = 1;

bool master = false;
bool ready = true;
int playerId = 0;
bool readyPlayer1 = false;
bool readyPlayer2 = false;
bool readyPlayer3 = false;
bool readyPlayer4 = false;

#define HOST "localhost"
#define PORT 12345
#define playerPort1 12346
#define playerPort2 12347

//--------------------------------------------------------------
void ofApp::setup() {
	HPV::InitHPVEngine();

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
	masterSender.setup(HOST, PORT);
	masterReceiver.setup(PORT);

	if(master) {
		player1.setup(HOST, playerPort1);
		player2.setup(HOST, playerPort1);
	}

    player.init(HPV::NewPlayer());
	if(playerId == 1) {
		player.load("top_left.hpv");
		receiver1.setup(PORT);
	} else if(playerId == 2) {
		player.load("top_right.hpv");
		receiver2.setup(PORT);
	} else if(playerId == 3) {
		player.load("bottom_left.hpv");
	} else if(playerId == 4) {
		player.load("bottom_right.hpv");
	}
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
	ofxOscMessage m;
	m.setAddress("/player");
	m.addIntArg(playerId);
	m.addBoolArg(ready);
	// m.addIntArg(cur_frame);
	masterSender.sendMessage(m, false);
	
	while(masterReceiver.hasWaitingMessages()){
		ofxOscMessage m;
		masterReceiver.getNextMessage(m);
		if(m.getAddress() == "/player"){
			int thisPlayerId = m.getArgAsInt(0);
			int thisPlayerReady = m.getArgAsBool(1);
			// received_frame = m.getArgAsInt(2);
			
			if(thisPlayerId == 1 && thisPlayerReady)
				readyPlayer1 = true;
			if(thisPlayerId == 2 && thisPlayerReady)
				readyPlayer2 = true;
		}
	}

	if(playerId == 1) {
		while(receiver1.hasWaitingMessages()){
			cout << "player 1" << endl;
			ofxOscMessage m;
			receiver1.getNextMessage(m);

			if(m.getAddress() == "/frame"){
				cout << "received 1" << endl;
				received_frame = m.getArgAsInt(0);

				cur_frame = received_frame;
				player.seekToFrame(cur_frame);
				prev_frame = cur_frame;
				if(master)
					cur_frame++;
			}
		}
	}

	if(playerId == 2) {
		while(receiver2.hasWaitingMessages()){
			cout << "player 2" << endl;
			ofxOscMessage m;
			receiver2.getNextMessage(m);

			if(m.getAddress() == "/frame"){
				cout << "received 2" << endl;
				received_frame = m.getArgAsInt(0);

				cur_frame = received_frame;
				player.seekToFrame(cur_frame);
				prev_frame = cur_frame;
				if(master)
					cur_frame++;
			}
		}
	}
    
	bool allReady = readyPlayer1 && readyPlayer2;
	if (allReady && master){
		cout << "all ready" << endl;
		cout << received_frame << endl;
		
		ofxOscMessage m;
		m.setAddress("/frame");
		m.addIntArg(cur_frame);
		player1.sendMessage(m, false);
		player2.sendMessage(m, false);
	}

	HPV::Update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(255);
	player.draw(0, 0, ofGetWidth(), ofGetHeight());
	// ready = true;
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
