#pragma once

#include "ofMain.h"
#include "ofxControl.h"


class ofApp : public ofBaseApp
{
public:
    ~ofApp();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key) {
       // widget.blah();
    }
    
    void toggleNotifier(ofxControlButtonEventArgs & s);
    void buttonNotifier(ofxControlButtonEventArgs & s);
    void sliderNotifier(ofxControlSliderEventArgs<double> & s);
    
    void vec3SliderNotifier(ofxControlMultiSliderEventArgs<ofVec3f> & s);
    
    void menuNotifier(ofxControlButtonEventArgs & s);
    void menuNotifierMultiChoice(ofxControlButtonEventArgs & s);
    
    
    ofxControlButton *button;
    
    ofxControlToggle *toggle;
    ofxControlToggle *toggleNotify;
    
    
    
    ofxControl2dPad *pad;
    ofPoint mypt;
    
    
    ofxControlWidget widget;
    
    
    ofxControlSlider<float> *floatSlider;
    ofxControlSlider<int> *intSlider;
    ofxControlSlider<double> *doubleSlider;
    
    ofxControlRangeSlider<float> *rangeFloatSlider;
    ofxControlRangeSlider<int> *rangeIntSlider;
    
    
    ofxControlMultiSlider<ofPoint> *vec3Slider;

    /*
    Gui2dPad *pad;
    
    GuiColor *colorPicker;
    
    GuiMenu *menu, *menuMultiChoice;
*/
    ofPoint vec3val;
    
    
    bool myToggle;
    float floatValue;
    int intValue;
    double doubleValue;
    float rangeLow, rangeHigh;
    ofPoint padValue, padValue2;
    ofFloatColor bgColor;
};
