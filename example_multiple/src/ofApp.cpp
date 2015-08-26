#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // a widget is a collection of gui elements
    widget1.setName("a widget 1");
    widget1.addButton("button", new bool(false));
    widget1.addSlider("slider", new float(0.5), 0.0f, 1.0f);
    widget1.addColor("color", new ofFloatColor(0.5, 0.2, 0.9));

    widget2.setName("a widget 2");
    widget2.addSlider("slider", new float(0.5), 0.0f, 1.0f);
    widget2.addMultiSlider("vec2 slider", new ofVec2f(2.5, 6.2), ofVec2f(0, 0), ofVec2f(10, 10));
    widget2.addToggle("my tog", new bool(false));

    widget3.setName("a widget 3");
    widget3.addSlider("my other slider", new int(5), 0, 10);
    widget3.addMultiSlider("vec4 slider", new ofVec4f(8.2, 5.3, 2.5, 6.2), ofVec4f(0,0,0, 0), ofVec4f(10, 10, 10, 10));
    widget3.addToggle("my new tog", new bool(false));

    widget4.setName("a widget 4");
    widget4.addToggle("my one toggle", new bool(true));
    widget4.addMultiSlider("vec3", new ofVec3f(5.3, 2.5, 6.2), ofVec3f(0, 0, 0), ofVec3f(10, 10, 10));
    widget4.addMultiSlider("vec2 slider", new ofVec2f(2.5, 6.2), ofVec2f(0, 0), ofVec2f(10, 10));

    selector.setName("Selector");
    selector.setPosition(20, 20);
    selector.addWidget("group 1", &widget1);
    selector.addWidget("group 2", &widget2);
    selector.addWidget("group 2", &widget3);
    selector.addWidget("group 1", &widget4);
    
    selector.setAllowMultiple(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){

}
