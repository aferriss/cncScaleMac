#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    w = 800;
    h = 800;
    ofSetWindowShape(w, h);
    ofSetVerticalSync(false);
    
    fbo.allocate(5000, 5000, GL_RGB, 4);
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
//    totalNumLines = 100000;
//    for(int i = 0; i<totalNumLines; i++){
        ofPolyline l;
        lineList.push_back(l);
//    }
    
    index = 0;
    
    curCoord = ofVec2f(0,0);
    getLine(index);
    
    ofSetBackgroundAuto(false);
    ofClear(255);
    
    save = false;
    
    doTsne();
    updatePoints();
}
//--------------------------------------------------------------
void ofApp::doTsne(){
    runManually = false;
    
    int N = lineList.size();
    int D = 100;
    int numClasses = 10;
    
    vector<vector<float> > classCenters;
    classCenters.resize(numClasses);
    
    for(int i = 0; i<numClasses; i++){
        vector<float> classCenter;
        classCenter.resize(D);
        for(int j = 0; j<D; j++){
            classCenter[j] = ofRandom(1);
        }
        classCenters[i] = classCenter;
    }
    
    linePoints.clear();
    int maxNumPts = 1;
    for(int i = 0; i<lineList.size(); i++){
        maxNumPts = max(maxNumPts, (int)lineList[i].size());
    }
    
    vector<ofPolyline> resampledLines;
    for(int i = 0; i<lineList.size(); i++){
        ofPolyline l;
        l = lineList[i].getResampledByCount(maxNumPts);
        cout<<ofToString((int)l.size())<<endl;
        
        resampledLines.push_back(l);
    }
    N = resampledLines.size();
    
    for(int i = 0; i<N; i++){
        resampledLines[i].resize(maxNumPts-1);
        int class_ = ofRandom(numClasses);
        vector<float> point;
//        point.resize(lineList[i].size()*2);

        for(int j=0; j<resampledLines[i].size(); j++){
            point.push_back( resampledLines[i][j].x);
            point.push_back( resampledLines[i][j].y);
        }
        
        LinePoint lp;
        lp.class_ = class_;
        lp.boundingBox = resampledLines[i].getBoundingBox();
        lp.line = resampledLines[i];
        lp.point = point;
        
        linePoints.push_back(lp);
    }
    
    vector<vector<float> > data;
    for (int i = 0; i < N; i++) {
        data.push_back(linePoints[i].point);
    }
    
    int dims = 2;
    float perplexity = 40;
    float theta = 0.2;
    bool normalize = true;
    
    tsnePoints = tsne.run(data, dims, perplexity, theta, normalize, runManually);
    
    if (!runManually) {
        for (int i=0; i<linePoints.size(); i++) {
            linePoints[i].tsnePoint = ofPoint(tsnePoints[i][0], tsnePoints[i][1]);
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::getLine(int ind){
    ofVec3f point;
    int count = 0;
    vector<ofVec3f> pointData;
    ofPolyline pl;
    ofBuffer buffer = ofBufferFromFile("allI5.txt");
//    ofBuffer buffer = ofBufferFromFile(paths[ind]);
    string text = buffer.getText();
    vector<string> lines = ofSplitString(text, "\n");
//    if(count < totalNumLines){
        for(int i = 0; i<lines.size(); i++){
            vector<string> line = ofSplitString(lines[i], " ");
            if(line[0] != "" && line[0] != "end"){
                point.x = ofToFloat(line[0]);
                point.y = ofToFloat(line[1]);
                point.z = ofToInt(line[2]);
                
    //            pointData.push_back(point);
                lineList[count].lineTo(point.x, point.y);
                
                
            } else if(line[0] == "end"){
                count++;
                ofPolyline l;
                lineList.push_back(l);
            }
        }
        
//    }

    
    getBounds(lineList);
}
//--------------------------------------------------------------
void ofApp::updatePoints(){
    for(int i = 0; i<linePoints.size(); i++){
        float x = fbo.getWidth() * linePoints[i].tsnePoint.x;
        float y = fbo.getHeight() * linePoints[i].tsnePoint.y;
        
        for(int j = 0; j<linePoints[i].line.size(); j++){
            linePoints[i].line[j].x += x;
            linePoints[i].line[j].y += y;
        }
    }
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
            
            for(int j = 0; j<pl[i].size(); j++){
                pl[i][j].x = ofMap(pl[i][j].x, minX, maxX, 0, 16);
                pl[i][j].y = ofMap(pl[i][j].y, minX, maxX, 0, 8);
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
//            lineList[index].draw();
    linePoints[index].line.draw();
    //    ofPopMatrix();
    
    
//        for (int i=0; i<linePoints.size(); i++) {
//            float x = fbo.getWidth() * linePoints[i].tsnePoint.x;
//            float y = fbo.getHeight() * linePoints[i].tsnePoint.y;
//            
//            ofPushMatrix();
//            ofTranslate(x, y);
//            linePoints[i].line.draw();
//            ofPopMatrix();
//            
//        }
    
        ofSetColor(255);
    fbo.end();
    
    fbo.draw(0,0, fbo.getWidth(), fbo.getHeight());
    

    
    index = (index+1)%linePoints.size();
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
