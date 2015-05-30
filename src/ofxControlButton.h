#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlSequence.h"
#include "ofxControlTouchOsc.h"


class ofxControlButtonBase;

struct ofxControlButtonEventArgs
{
    ofxControlButtonBase *button;
    bool value;
    
    ofxControlButtonEventArgs(ofxControlButtonBase *button, bool value)
    {
        this->button = button;
        this->value = value;
    }
};


class ofxControlButtonBase : public ofxControlElement
{
public:
    
    template <typename L, typename M>
    ofxControlButtonBase(ofxControlParameter<bool> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlButtonBase(string name, bool *value, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlButtonBase(string name, L *listener, M method);
    
    ofxControlButtonBase(ofxControlParameter<bool> *parameter);
    ofxControlButtonBase(string name, bool *value);
    ofxControlButtonBase(string name);
    
    virtual ~ofxControlButtonBase();
        
    void setLeftJustified(bool leftJustified) {this->leftJustified = leftJustified;}
    
    bool getValue();
    void setValue(bool value, bool sendChangeNotification=false);

    void lerpTo(float nextValue, int numFrames);
    
    virtual void update();
    virtual void draw();
    
    ofEvent<ofxControlButtonEventArgs> buttonEvent;
    
protected:
    
    bool isDiscrete() {return true;}

    void initializeButton();
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    void setupGuiPositions();
    void updateParameterOscAddress();

    void setValueFromSequence(ofxControlSequence &sequence);
    void setSequenceFromValue(ofxControlSequence &sequence, int column);
    void setSequenceFromExplicitValue(ofxControlSequence &sequence, int column, float value);

    virtual void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);
    string getOscAddress();
    void sendOsc(ofxOscMessage &msg);
    void receiveOsc(ofxOscMessage &msg);
    bool valueChanged();

    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);

    ofxControlParameter<bool> *parameter;
    float stringWidth, stringHeight;
    bool leftJustified;
    
    float lerpNextValue;;
    int lerpFrame, lerpNumFrames;
    bool changed;
};


template <typename L, typename M>
ofxControlButtonBase::ofxControlButtonBase(ofxControlParameter<bool> *parameter, L *listener, M method) : ofxControlElement(name)
{
    this->parameter = parameter;
    initializeButton();
    ofAddListener(buttonEvent, listener, method);
}

template <typename L, typename M>
ofxControlButtonBase::ofxControlButtonBase(string name, bool *value, L *listener, M method) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<bool>(name, value);
    initializeButton();
    ofAddListener(buttonEvent, listener, method);
}

template <typename L, typename M>
ofxControlButtonBase::ofxControlButtonBase(string name, L *listener, M method) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<bool>(name, new bool());
    initializeButton();
    ofAddListener(buttonEvent, listener, method);
}



class ofxControlButton : public ofxControlButtonBase
{
public:
    ofxControlButton(ofxControlParameter<bool> *parameter) : ofxControlButtonBase(parameter) { }
    ofxControlButton(string name, bool *value) : ofxControlButtonBase(name, value) { }
    ofxControlButton(string name) : ofxControlButtonBase(name) { }

    template <typename L, typename M>
    ofxControlButton(ofxControlParameter<bool> *parameter, L *listener, M method) : ofxControlButtonBase(parameter, listener, method) { }
    
    template <typename L, typename M>
    ofxControlButton(string name, bool *value, L *listener, M method) : ofxControlButtonBase(name, value, listener, method) { }
    
    template <typename L, typename M>
    ofxControlButton(string name, L *listener, M method) : ofxControlButtonBase(name, listener, method) { }
    
    void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);
    
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
};



class ofxControlToggle : public ofxControlButtonBase
{
public:
    ofxControlToggle(ofxControlParameter<bool> *parameter) : ofxControlButtonBase(parameter) { }
    ofxControlToggle(string name, bool *value) : ofxControlButtonBase(name, value) { }
    ofxControlToggle(string name) : ofxControlButtonBase(name) { }

    template <typename L, typename M>
    ofxControlToggle(ofxControlParameter<bool> *parameter, L *listener, M method) : ofxControlButtonBase(parameter, listener, method) { }
    
    template <typename L, typename M>
    ofxControlToggle(string name, bool *value, L *listener, M method) : ofxControlButtonBase(name, value, listener, method) { }
    
    template <typename L, typename M>
    ofxControlToggle(string name, L *listener, M method) : ofxControlButtonBase(name, listener, method) { }
    
    void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);
    
    bool mousePressed(int mouseX, int mouseY);
    bool keyPressed(int key);
};
