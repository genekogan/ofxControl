#pragma once

#include "ofMain.h"
#include "ofxControlWidget.h"
#include "ofxControlOscManager.h"


class ofxControlOscManagerPanel : public ofxControlElement
{
public:
    ofxControlOscManagerPanel(string name, ofxControlWidget *panel, ofxControlOscManager *osc);
    ~ofxControlOscManagerPanel();
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    void getXml(ofXml &xml) { }
    void setFromXml(ofXml &xml) { }
    
    void setupGuiPositions();
    
protected:
    
    struct ElementOsc
    {
        ofxControlElement *element;
        ofxControlOscManager *osc;
        
        ofxControlTextBox *tAddress;
        ofxControlToggle *tSend;
        string address;
        bool sending;
        
        ElementOsc(ofxControlElement *element, ofxControlOscManager *osc);
        void eventSetAddress(ofxControlTextBoxEventArgs &evt);
        void eventSetSending(ofxControlButtonEventArgs &evt);
        void updateAddress();
    };
    
    struct ElementOscGroup
    {
        ofxControlElement *element;
        ofxControlOscManager *osc;
        
        vector<ElementOsc*> elements;
        ElementOscGroup(ofxControlElement *element, ofxControlOscManager *osc);
    };
    
    void setupofxControlOscManagerPanel();
    void addElement(ofxControlElement* & newElement);
    void removeElement(ofxControlElement* & element);
    void eventMakeTouchOscLayout(ofxControlButtonEventArgs &evt);

    map<ofxControlElement*,ElementOscGroup*> groups;
    ofxControlWidget * panel;
    ofxControlOscManager * osc;
    int ofxControlManagerWidth;
    ofxControlButton *tTouchOsc;
};


