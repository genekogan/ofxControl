#pragma once

#include "ofMain.h"
#include "ofxControl.h"


class ofApp : public ofBaseApp
{
    
public:
    
    void setup();
    void update();
    void draw();
    
    
    void buttonEvent(ofxControlButtonEventArgs & e);
    void menuSelect(ofxControlMenuEventArgs & e);
    void sliderEvent(ofxControlMultiSliderEventArgs<ofVec2f> & e);
    void colorEvent(ofxControlColorEventArgs & e);
    void panelToggleEvent(ofxControlButtonEventArgs & e);
    void multiChoiceMenuSelect(ofxControlMenuEventArgs & e);

    void textBoxEvent(ofxControlTextBoxEventArgs & e) {
        cout << "text box: " << e.textBox->getName() << " : " << e.value <<endl;
    }
    
    void keyPressed(int key) {
        if (key=='q') {
            //panel.savePreset("/Users/Gene/Desktop/testXml2.xml");
        }
        else if (key=='w') {
            //panel.loadPreset("/Users/Gene/Desktop/testXml2.xml");
        }
        
        else if (key=='1') {
            panel.removeElement("float slider");
        }
        else if (key=='2') {
            cout << "==== KEY 2 ===== " <<endl;
            panel.removeElement("widget inside panel");
            cout << "==== KEY 2 ===== " <<endl;
        }
        
        else if (key=='3') {
            panel.removeElement("multi choice menu");
        }
        else if (key=='4') {
            widget.removeElement("menu");
        }
    }
    

    ofxControlPanel panel;
    ofxControlWidget widget;
    
    bool button, toggle;
    float floatSlider;
    int intSlider;
    double doubleSlider;
    ofVec4f vec4slider;
    float rangeLow, rangeHigh;
    ofVec2f vec2RangeLow, vec2RangeHigh;
    ofPoint padValue;
    ofFloatColor color, color2;
    ofVec2f vec2slider;
    
};
