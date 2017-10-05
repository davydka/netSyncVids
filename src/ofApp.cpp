#include "ofApp.h"

int cur_frame = 0;
int frame_length = 0;
int prev_frame = 0;
int received_frame = 0;

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

//--------------------------------------------------------------
void ofApp::setup() {
	HPV::InitHPVEngine();

	cout << "----" << endl;
	for (int i=0; i<arguments.size(); ++i){
		// cout << arguments.at(i) << endl;
		if(arguments.at(i) == "0") {
			master = true;
			ofSetWindowShape(0,0);
		}
		if(arguments.at(i) == "1" || arguments.at(i) == "2" || arguments.at(i) == "3" || arguments.at(i) == "4") {
			playerId = ofToInt(arguments.at(i));
		}
	}
	cout << "----" << endl;

    player.init(HPV::NewPlayer());
	ofSetVerticalSync(false);
	ofSetFrameRate(60);
    
	// Sender
	if(master) {
		player.load("top_left.hpv"); // for getting playyer length
		if(server.setup(PORT)){
			cout << "server created" << endl;
		}
		frame_length = player.getTotalNumFrames();
		HPV::DestroyHPVEngine();
	}

	// Players
	if(playerId == 1) {
		player.load("top_left.hpv");
		ofSetWindowPosition(0, 0);
	} else if(playerId == 2) {
		player.load("top_right.hpv");
		ofSetWindowPosition(ofGetWidth()+offset, 0);
	} else if(playerId == 3) {
		player.load("bottom_left.hpv");
		ofSetWindowPosition(0, ofGetHeight()+offset+23);
	} else if(playerId == 4) {
		player.load("bottom_right.hpv");
		ofSetWindowPosition(ofGetWidth()+offset, ofGetHeight()+offset+23);
	}

	if(!master) {
		player.setLoopState(OF_LOOP_NORMAL);
		player.setDoubleBuffered(true);
		
		player.play();
		player.setPaused(true);

		if(client.setup(HOST, PORT)){
			ofAddListener(client.messageReceived, this, &ofApp::onMessageComing);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	if(master) {
		if(server.isConnected()){
			server.update();
			if(server.hasClients()){
				for (auto & client : server.getClients()) {
					if(client->isCalibrated()){
						client->send("FRAME " + ofToString(cur_frame));
					}
				}
			}
		}
		cur_frame++;
		if (cur_frame >= player.getTotalNumFrames()) {
			cur_frame = 0;
		}
	}

	if(!master) {
		HPV::Update();
	}
    
	bool allReady = readyPlayer1 && readyPlayer2;
	if (allReady && master){
		cout << "all ready" << endl;
		
	}

}

void ofApp::onMessageComing(string & message){
	// cout << message <<endl;
	const string messageFrame = "FRAME ";
	if(message.find(messageFrame) == 0 && client.isCalibrated()){
		int frame = ofToInt(message.substr(messageFrame.length()));
		player.seekToFrame(frame);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	if(!master) {
		ofBackground(255);
		player.draw(0, 0, ofGetWidth(), ofGetHeight());
		// ready = true;
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	HPV::DestroyHPVEngine();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}
