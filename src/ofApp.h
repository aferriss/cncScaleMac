#pragma once

#include "ofMain.h"

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
        void getLine(int i);
        void parseData(vector<ofVec3f> data);
        void getBounds(vector<ofPolyline> &pl);
    
        int w, h;
        int index;
        vector<string> paths;
        ofPolyline curLine;
        ofVec2f curCoord;
        int totalNumLines;
        vector<ofPolyline> lineList;
        ofFbo fbo;
        bool save;
};
