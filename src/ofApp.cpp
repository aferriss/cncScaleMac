#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    w = 800;
    h = 800;
    ofSetWindowShape(w, h);
    
    fbo.allocate(3312, 6768, GL_RGB, 4);
    fbo.begin();
        ofClear(255);
    fbo.end();

//    string path = "textFiles";
//    ofDirectory dir(path);
//    dir.listDir();
//    
//    for(int i = 0; i<dir.size(); i++){
//        paths.push_back(dir.getPath(i));
//    }
    totalNumLines = 100000;
    for(int i = 0; i<totalNumLines; i++){
        ofPolyline l;
        lineList.push_back(l);
    }
    
    index = 0;
    
    curCoord = ofVec2f(0,0);
    getLine(index);
    
    ofSetBackgroundAuto(false);
    ofClear(255);
    
    save = false;
}
//--------------------------------------------------------------
void ofApp::getLine(int ind){
    ofVec3f point;
    int count = 0;
    vector<ofVec3f> pointData;
    ofPolyline pl;
    ofBuffer buffer = ofBufferFromFile("allOFix.txt");
//    ofBuffer buffer = ofBufferFromFile(paths[ind]);
    string text = buffer.getText();
    vector<string> lines = ofSplitString(text, "\n");
    if(count < totalNumLines){
        for(int i = 0; i<lines.size(); i++){
            vector<string> line = ofSplitString(lines[i], " ");
            if(line[0] != "" && line[0] != "end"){
                point.x = ofToFloat(line[0]);
                point.y = ofToFloat(line[1]);
                point.z = ofToInt(line[2]);
                
    //            pointData.push_back(point);
                
                lineList[count].curveTo(point.x, point.y);
            } else if(line[0] == "end"){
                count++;
            }
        }
        
    }

    
    getBounds(lineList);
    
//    float minX = pointData[0].x;
//    float minY = pointData[0].y;
//    float maxX = pointData[0].x;
//    float maxY = pointData[0].y;
//    
//    for(int i = 0; i<pointData.size(); i++){
//        minX = min(minX, pointData[i].x);
//        minY = min(minY, pointData[i].y);
//        maxX = max(maxX, pointData[i].x);
//        maxY = max(maxY, pointData[i].y);
//    }
//    
//    for(int i = 0; i<pointData.size(); i++){
//        int x = ofMap(pointData[i].x, minX, maxX, 0, 72);
//        int y = ofMap(pointData[i].y, minY, maxY, 0, 18);
//        pl.curveTo(x, y);
//    }
    
//    return pl;
}
//--------------------------------------------------------------
void ofApp::getBounds(vector<ofPolyline> &pl){
    int between = 15 /4;
    int xWidth = (50 + between)/4;
    int yHeight = 36/4;
    int xOff = 0;
    int yOff = 0;
    
    for(int i = 0; i<pl.size(); i++){
        if(pl[i].size()>0){
            float minX = pl[i][0].x;
            float minY = pl[i][0].y;
            float maxX = pl[i][0].x;
            float maxY = pl[i][0].y;
            
            for(int j = 0; j<pl[i].size(); j++){
                minX = min(minX, pl[i][j].x);
                minY = min(minY, pl[i][j].y);
                maxX = max(maxX, pl[i][j].x);
                maxY = max(maxY, pl[i][j].y);
            }
            
//            int xOff = ((i) % w)*xWidth;
            
            for(int j = 0; j<pl[i].size(); j++){
                pl[i][j].x = ofMap(pl[i][j].x, minX, maxX, 0+xOff, 50/4+xOff);
                pl[i][j].y = ofMap(pl[i][j].y, minY, maxY, 0+yOff, 36/4+yOff);
            }
            
            xOff += xWidth;
            
            if(xOff >= fbo.getWidth()-(xWidth+1)){
                xOff = 0;
                yOff += yHeight;
            }
        }
    }
}
//--------------------------------------------------------------
void ofApp::update(){
    if(save){
        ofPixels savePix;
        fbo.readToPixels(savePix);
        ofSaveImage(savePix, "images/"+ofGetTimestampString()+".jpg");
        save = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
        ofSetColor(0);
    //    ofPushMatrix();
    //    ofTranslate(w/2 -  lineList[index].getBoundingBox().width/2, h/2 -  lineList[index].getBoundingBox().height/2);
    //        curLine.draw();
            lineList[index].draw();
    //    ofPopMatrix();
        
    
        ofSetColor(255);
    fbo.end();
    
    fbo.draw(0,0);
    
    index = (index+1)%totalNumLines;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        index++;
    }
    
    if(key == 's'){
        save = true;
    }
    
    if(key == 'c'){
        ofSetWindowTitle(ofToString(index));
    }
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
void ofApp::mousePressed(int x, int y, int button){

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
