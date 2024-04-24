#pragma once

#include "ofMain.h"
#include "ofxBloom.h"
#include "ofxGui.h"

// 雨滴を表すクラス
class Raindrop {
public:
	ofVec2f position;
	bool isActive;
	float speed;
	float radius;
	void setup(float x, float y);
	void update(float rainRadius, float rainMaxSpeed, float rainMinSpeed);
	void draw(float rainRadius);
	void reset();
};

// 障害物を表すクラス
class Obstacle {
public:
	ofVec2f position;
	float radius;
	int number;
	void setup(float x, float y, float r, float num);
	void draw();
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofSerial sendDetectSerial;
		ofSerial recieveHeartBeatSerial;
		vector<Raindrop> raindrops;
		vector<Obstacle> obstacles;

		float realTime;
		float timeLastTryConnect;
		std::vector<ofSerialDeviceInfo> deviceList;

		ofxPanel gui;
		ofxGuiGroup Group_DrawContents;
		ofxFloatSlider RainRadius;
		ofxFloatSlider RainMaxSpeed;
		ofxFloatSlider RainMinSpeed;
		ofxGuiGroup Group_Bloom;
		ofxFloatSlider scale;
		ofxFloatSlider thresh;
		ofxFloatSlider brightness;
		ofxToggle b_EnableBloom;

		ofxBloom bloom;
		ofFbo fbo;

		void setup_GUI();

		bool doDrawGUI;
};
