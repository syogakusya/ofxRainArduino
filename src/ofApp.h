#pragma once

#include "ofMain.h"

// 雨滴を表すクラス
class Raindrop {
public:
	ofVec2f position;
	bool isActive;
	float speed;
	void setup(float x, float y);
	void update();
	void draw();
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
};
