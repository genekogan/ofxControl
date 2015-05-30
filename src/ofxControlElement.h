#pragma once

#include "ofMain.h"
#include "ofBitmapFont.h"
#include "ofxControlConstants.h"
#include "ofxControlBase.h"
#include "ofxControlSequence.h"


class ofxControlSequence;
class ofxControlTouchOscPage;

class ofxControlElement : public ofxControlBase
{
public:
    ofxControlElement(string name);
    ofxControlElement();
    virtual ~ofxControlElement();
    
    void setParent(ofxControlElement *parent);
    bool getHasParent() {return hasParent;}
    ofxControlElement * getParent() {return parent;}

    virtual bool isMultiElement() {return false;}
    virtual bool isDiscrete() {return false;}

    virtual bool getCollapsed();
    void setMouseOver(bool mouseOver);

    string getAddress();
    bool getActive();
    void setActive(bool active);
    
    virtual void getParameters(vector<ofxControlParameterBase*> & parameters) { }
    
    virtual void lerpTo(float nextSliderValue, int lerpNumFrames) { }
    virtual void setValueFromSequence(ofxControlSequence &sequence) { }
    virtual void setSequenceFromValue(ofxControlSequence &sequence, int column) { }
    virtual void setSequenceFromExplicitValue(ofxControlSequence &sequence, int column, float value) { }
    
    virtual void getXml(ofXml &xml);
    virtual void setFromXml(ofXml &xml);
    
    virtual void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);
    virtual void updateParameterOscAddress();

    virtual string getOscAddress() {return getAddress();}
    virtual void sendOsc(ofxOscMessage &msg) { }
    virtual void receiveOsc(ofxOscMessage &msg) { }
    virtual bool valueChanged() { return false; }
    
protected:

    virtual void setupGuiPositions();
    void resetGuiPositions();
    void setupDisplayString();

    ofxControlElement *parent;
    bool hasParent;
    string display;
};
