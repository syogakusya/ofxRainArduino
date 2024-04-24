#include "ofApp.h"

const int num = 40;
const int rainNum = 4;

void Raindrop::setup(float x, float y) {
	isActive = false;
	position.set(x, y);
}

void Raindrop::update(float rainRadius, float rainMaxSpeed, float rainMinSpeed) {
	if (isActive) {
		//speed = ofRandom(rainMinSpeed, rainMaxSpeed);
		radius = rainRadius;
		position.y = position.y + speed;
		if (position.y > ofGetWindowHeight()) {
			isActive = false;
			position.y = 0;
			position.x = ofRandom(ofGetWindowWidth());
		}
	}
	
	//position.y += speed; // 雨滴を下に移動
	//if (position.y > ofGetWindowHeight()) { // 画面下端に到達したら
	//	position.y = 0; // 画面上端に戻す
	//	position.x = ofRandom(ofGetWindowWidth()); // x座標をランダムに再設定
	//}
}

void Raindrop::draw(float rainRadius) {
	if (isActive) {
		ofDrawCircle(position.x, position.y, rainRadius);
		ofPushStyle();
		ofDrawCircle(position.x, position.y - rainRadius*1.1, rainRadius*0.9);
		ofDrawCircle(position.x, position.y - rainRadius*1.2, rainRadius*0.8);
		ofPopStyle();
	}
}

void Raindrop::reset() {
	position.x = ofRandom(ofGetWindowWidth());
	position.y = 0;
	isActive = false;
}


void Obstacle::setup(float x, float y, float r, float num) {
	position.set(x, y);
	number = num;
	radius = r;
}

void Obstacle::draw() {
	ofDrawCircle(position.x, position.y, radius);
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundAuto(true);
	ofSetVerticalSync(false);
	ofSetFrameRate(120);

	sendDetectSerial.listDevices();
	for (auto& a : sendDetectSerial.getDeviceList()) {
		cout << a.getDeviceID()<<" : "<<a.getDeviceName() << endl;
	}

	sendDetectSerial.setup("COM4", 9600);
	recieveHeartBeatSerial.setup("COM6", 9600);

	obstacles.clear();
	raindrops.clear();

	ofBackground(0);

	for (int i = 0; i < num; i++) {
		Raindrop raindrop;
		//raindrop.setup(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
		raindrop.setup(ofRandom(ofGetWindowWidth()), 0);
		raindrop.speed = ofRandom(RainMinSpeed, RainMaxSpeed);
		raindrop.radius = RainRadius;
		raindrops.push_back(raindrop);
	}

	Obstacle obstacle;
	for (int i = 0; i < 5; i++) {
		obstacle.setup((ofGetWindowWidth() / 5) * (i+1) - 100, 750, 30 , (i+1));
		obstacles.push_back(obstacle);
	}

	
	ofDisableArbTex();
	fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight());
	ofEnableArbTex();

	bloom.setup(fbo.getWidth(), fbo.getHeight(), fbo);
	setup_GUI();
}

void ofApp :: setup_GUI() {
	gui.setup();
	doDrawGUI = false;
	Group_DrawContents.setup("DrawContents");
	Group_DrawContents.add(RainMaxSpeed.setup("RainMaxSpeed", 5, 1, 30));
	Group_DrawContents.add(RainMinSpeed.setup("RainMinSpeed", 3, 1, 30));
	Group_DrawContents.add(RainRadius.setup("RainRadius", 10, 1, 30));
	gui.add(&Group_DrawContents);

	Group_Bloom.setup("Bloom");
	Group_Bloom.add(scale.setup("Scale", 2.3f, 0.1f, 16.0f));
	Group_Bloom.add(brightness.setup("Brightness", 5.0f, 0.0f, 30.0f));
	Group_Bloom.add(thresh.setup("Threshold", 0.0f, 0.0f, 1.0f));
	Group_Bloom.add(b_EnableBloom.setup("b_EnableBloom", true));
	gui.add(&Group_Bloom);

	gui.minimizeAll();
}

//--------------------------------------------------------------
void ofApp::update(){

	if (recieveHeartBeatSerial.isInitialized()) {
		if (recieveHeartBeatSerial.available()) {
			char h = recieveHeartBeatSerial.readByte();
			if (h == 'h') {
				cout << "Heart Beat beat it !!" << endl;
				for (int i = 0; i < rainNum; i++) {
					bool procceing = true;
					int j = 0;
					while (procceing && j < raindrops.size() - 1) {
						if (!raindrops[j].isActive) {
							raindrops[j].isActive = true;
							procceing = false;
							break;
						}
						else {
							j++;
						}
					}
				}
			}
		}
	}


	for (auto& raindrop : raindrops) {
		raindrop.update(RainRadius, RainMaxSpeed, RainMinSpeed);
	}

	for (auto& obstacle : obstacles) {
		for (auto& raindrop : raindrops) {
			float distance = obstacle.position.distanceSquared(raindrop.position);
			if (distance < (obstacle.radius * obstacle.radius)) {
				cout << "Raindrop hit at : " << obstacle.number << endl;
				//ここに衝突した時のプログラムを入れる
				switch (obstacle.number) {
				case 1:
					sendDetectSerial.writeByte('1');
					break;
				case 2:
					sendDetectSerial.writeByte('2');
					break;
				case 3:
					sendDetectSerial.writeByte('3');
					break;
				case 4:
					sendDetectSerial.writeByte('4');
					break;
				case 5:
					sendDetectSerial.writeByte('5');
					break;
				default:
					cout << "EXCEPTION!" << endl;
				}
				raindrop.reset();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	fbo.begin();
	ofClear(0);
	ofSetColor(24, 235, 249);
	for (auto& raindrop : raindrops)
		raindrop.draw(RainRadius);
	/*ofSetColor(255, 0, 0);
	for (auto& obstacle : obstacles) {
		obstacle.draw();
	}*/
	fbo.end();
	if (b_EnableBloom) {
		bloom.setBrightness(brightness);
		bloom.setScale(scale);
		bloom.setThreshold(thresh);
		bloom.process();
	}

	if (b_EnableBloom) bloom.getResultFbo().draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	else    fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	if(doDrawGUI)
		gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') {
		sendDetectSerial.writeByte('1');
	}
	if (key == '2') {
		sendDetectSerial.writeByte('2');
	}
	if (key == '3') {
		sendDetectSerial.writeByte('3');
	}
	if (key == '4') {
		sendDetectSerial.writeByte('4');
	}
	if (key == '5') {
		sendDetectSerial.writeByte('5');
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 'r') {

		for (int i = 0; i < rainNum; i++) {
			bool procceing = true;
			int j = 0;
			while (procceing && j < raindrops.size() - 1) {
				if (!raindrops[j].isActive) {
					raindrops[j].isActive = true;
					procceing = false;
					break;
				}
				else {
					j++;
				}
			}
		}
	}
	if (key == 'g') {
		doDrawGUI = !doDrawGUI;
	}
	if (key == 's') {
		for (auto& raindrop : raindrops) {
			raindrop.speed = ofRandom(RainMinSpeed, RainMaxSpeed);
		}
	}
}

void ofApp::mousePressed(int x, int y, int button) {

	//if (button == OF_MOUSE_BUTTON_LEFT) {
	//	Obstacle obstacle;
	//	obstacle.setup(x, y, 30); // マウスクリックした位置に障害物を生成
	//	obstacles.push_back(obstacle);
	//}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
