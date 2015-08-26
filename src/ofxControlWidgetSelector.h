#pragma once

#include "ofMain.h"
#include "ofxControlWidget.h"


class ofxControlWidgetSelector : public ofxControlWidget
{
public:
    ofxControlWidgetSelector();
    ~ofxControlWidgetSelector();
    
    void setAllowMultiple(bool multiple);
    
    void addWidget(string groupName, ofxControlWidget *widget);

private:
    bool mouseDragged(int mouseX, int mouseY);
    
    void hideAll();
    void eventSelectWidget(ofxControlMenuEventArgs &evt);

    map<ofxControlToggle*, ofxControlWidget*> widgets;
    map<string, ofxControlMenu*> menus;
    
    bool multiple;
};
