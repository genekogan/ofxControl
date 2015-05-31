#pragma once

#include "ofMain.h"
#include "ofxControl.h"





class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    
    void menuSelect(ofxControlMenuEventArgs & e);
    void multiChoiceMenuSelect(ofxControlMenuEventArgs & e);

    ofxControlPanel panel;
    
    bool filled;
    float radius;
    float lineWidth;
    int resolution;
    ofVec2f position;
    ofFloatColor myColor;
    bool b1, b2, b3;
    
    string greeting;
    string city;
    
};
