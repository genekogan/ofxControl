#include "ofxControlOscManager.h"


ofxControlOscManager::ofxControlOscManager()
{
    panel.setName("OSC");
    panel.addToggle("send", &sending, this, &ofxControlOscManager::eventToggleSending);
    panel.addTextBox("hostOut", &hostOut, this, &ofxControlOscManager::eventEditHostOut);
    panel.addTextBox("portOut", &portOutS, this, &ofxControlOscManager::eventEditPortOut);
    panel.addToggle("receive", &receiving, this, &ofxControlOscManager::eventToggleReceiving);
    panel.addTextBox("portIn", &portInS, this, &ofxControlOscManager::eventEditPortIn);
    
    portIn = 9000;
    portOut = 9001;
    portInS = "9000";
    portOutS = "9001";
    hostOut = "localhost";
}

ofxControlOscManager::~ofxControlOscManager()
{

}

void ofxControlOscManager::eventToggleSending(ofxControlButtonEventArgs &evt)
{
    if (sending) {
        setupSender(hostOut, portOut);
    }
}

void ofxControlOscManager::eventToggleReceiving(ofxControlButtonEventArgs &evt)
{
    if (receiving) {
        setupReceiver(portIn);
    }
}

void ofxControlOscManager::eventEditHostOut(ofxControlTextBoxEventArgs &evt)
{
    sending = false;
}

void ofxControlOscManager::eventEditPortOut(ofxControlTextBoxEventArgs &evt)
{
    sending = false;
    portOut = ofToInt(portOutS);
}

void ofxControlOscManager::eventEditPortIn(ofxControlTextBoxEventArgs &evt)
{
    receiving = false;
    portIn = ofToInt(portInS);
}

bool ofxControlOscManager::setupSender(string host, int portOut)
{
    try
    {
        sender.setup(host, portOut);
        this->host = host;
        this->portOut = portOut;
        sending = true;
        ofLog(OF_LOG_NOTICE, "Connect OSC Sender "+host+", port "+ofToString(portOut));
    }
    catch(runtime_error &e)
    {
        sending = false;
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return sending;
}

bool ofxControlOscManager::setupReceiver(int portIn)
{
    try
    {
        receiver.setup(portIn);
        this->portIn = portIn;
        receiving = true;
        ofLog(OF_LOG_NOTICE, "Connect OSC Receiver, port "+ofToString(portIn));
    }
    catch(runtime_error &e)
    {
        receiving = false;
        ofLog(OF_LOG_ERROR, ofToString(e.what()));
    }
    return receiving;
}

void ofxControlOscManager::addWidget(ofxControlWidget & widget)
{
    widget.createOscManager(this);
    for (auto element : widget.getElements()) {
        addElementToofxControlOscManager(element);
    }
}

void ofxControlOscManager::addElementToofxControlOscManager(ofxControlElement * element)
{
    if (element->isMultiElement())
    {
        for (auto e : ((ofxControlMultiElement *) element)->getElements()) {
            addElementToofxControlOscManager(e);
        }
    }
    else
    {
        if (rParameters.count(element->getOscAddress()) == 0) {
            rParameters[element->getOscAddress()] = element;
        }
        else {
            ofLog(OF_LOG_WARNING, "Parameter at "+element->getOscAddress()+" already registered with receiver");
        }

    }
}

void ofxControlOscManager::addElementToSender(ofxControlElement *element)
{
    vector<ofxControlElement*>::iterator it = sParameters.begin();
    for (; it != sParameters.end(); ++it)
    {
        if (*it == element)
        {
            ofLog(OF_LOG_WARNING, "Parameter at "+element->getOscAddress()+" already registered with sender");
            return;
        }
    }
    sParameters.push_back(element);
}

void ofxControlOscManager::removeElementFromSender(ofxControlElement *element)
{
    vector<ofxControlElement*>::iterator it = sParameters.begin();
    while (it != sParameters.end())
    {
        if (*it == element) {
            sParameters.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ofxControlOscManager::update()
{
    if (receiving) {
        receiveOscMessages();
    }
    if (sending) {
        sendOscMessages();
    }
}

void ofxControlOscManager::receiveOscMessages()
{
    while(receiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        receiver.getNextMessage(&msg);
        string address = msg.getAddress();
        
        if (rParameters.count(address) > 0) {
            rParameters[address]->receiveOsc(msg);
        }
    }
}

void ofxControlOscManager::sendOscMessages()
{
    for (auto p : sParameters)
    {
        if (p->valueChanged())
        {
            ofxOscMessage msg;
            msg.setAddress(p->getOscAddress());
            p->sendOsc(msg);
            sender.sendMessage(msg);
        }
    }
}
