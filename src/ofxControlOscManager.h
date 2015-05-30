#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxControlWidget.h"


class ofxControlOscManager
{
public:
    ofxControlOscManager();
    ~ofxControlOscManager();
    
    ofxControlWidget & getControl() {return panel;}
    
    bool setupSender(string host, int portOut);
    bool setupReceiver(int portIn);
    
    void addWidget(ofxControlWidget & widget);
    void addElementToofxControlOscManager(ofxControlElement * element);
    void addElementToSender(ofxControlElement *element);
    void removeElementFromSender(ofxControlElement *element);
    
    void update();
    
    
    void blah() {
        
        ofxOscMessage msg;
        msg.setAddress("/live/state");
        sender.sendMessage(msg);
    }
    
private:

    void eventToggleSending(ofxControlButtonEventArgs &evt);
    void eventToggleReceiving(ofxControlButtonEventArgs &evt);
    void eventEditHostOut(ofxControlTextBoxEventArgs &evt);
    void eventEditPortOut(ofxControlTextBoxEventArgs &evt);
    void eventEditPortIn(ofxControlTextBoxEventArgs &evt);
    
    void receiveOscMessages();
    void sendOscMessages();
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    string host;
    int portIn, portOut;
    bool sending, receiving;
    
    vector<ofxControlElement*> sParameters;
    map<string,ofxControlElement*> rParameters;
    
    ofxControlWidget panel;
    string hostOut;
    string portInS, portOutS;
};

