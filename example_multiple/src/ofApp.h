#pragma once

#include "ofMain.h"
#include "ofxControl.h"


class ofApp : public ofBaseApp
{
    
public:
    
    void setup();
    void update();
    void draw();
    
    
    void keyPressed(int key) {

    }
    
    
    ofxControlWidget widget1;
    ofxControlWidget widget2;
    ofxControlWidget widget3;
    ofxControlWidget widget4;
    
    ofxControlWidgetSelector selector;

    
};
