#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlMultiElement.h"


struct ofxControl2dPadPoint
{
    ofxControl2dPadPoint(ofxControlParameter<ofPoint> *parameter);
    ~ofxControl2dPadPoint();
    
    void setValue(ofPoint padValue);
    void lerpTo(ofPoint nextValue, int numFrames);
    void update();
    void increment(float x, float y);
    string getValueString();
    
    ofxControlParameter<ofPoint> *parameter;
    ofPoint previous;
    ofPoint padValue;

    ofPoint lerpPrevValue, lerpNextValue;;
    int lerpFrame, lerpNumFrames;
};


struct ofxControl2dPadEventArgs
{
    ofxControl2dPadPoint *point;
    ofPoint padValue;
    
    ofxControl2dPadEventArgs(ofxControl2dPadPoint *point, ofPoint padValue)
    {
        this->point = point;
        this->padValue = padValue;
    }
};


class ofxControl2dPad : public ofxControlMultiElement
{
public:
    
    ofxControl2dPad(ofxControlParameter<ofPoint> *parameter);
    ofxControl2dPad(string name, ofPoint *value, ofPoint min, ofPoint max);
    ofxControl2dPad(string name, ofPoint min, ofPoint max);
    
    template<typename L, typename M>
    ofxControl2dPad(ofxControlParameter<ofPoint> *parameter, L *listener, M method);
    
    template<typename L, typename M>
    ofxControl2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method);
    
    template<typename L, typename M>
    ofxControl2dPad(string name, ofPoint min, ofPoint max, L *listener, M method);
    
    ~ofxControl2dPad();
    
    
    void setParent(ofxControlElement *parent);
    
    void setMin(ofPoint min);
    void setMax(ofPoint max);
    ofPoint getMin() {return min;}
    ofPoint getMax() {return max;}
    
    ofxControl2dPadPoint * addPoint(ofxControlParameter<ofPoint> *parameter);
    ofxControl2dPadPoint * addPoint(ofPoint *value);
    ofxControl2dPadPoint * addPoint();
    void removePoint(int idx);
    void clearPoints();

    bool getDrawConnectedPoints() {return connectPoints;}
    void setDrawConnectedPoints(bool connectPoints) {this->connectPoints = connectPoints;}
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    ofPoint getValue(int idx) {return points[idx]->padValue;}
    void setValue(int idx, ofPoint padValue);
    
    void lerpTo(int idx, ofPoint nextValue, int numFrames);

    ofPoint getParameterValue(int idx) {return points[idx]->parameter->get();}
    int getNumberOfPoints() {return points.size();}
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    
    void update();
    void draw();
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    ofEvent<ofxControl2dPadEventArgs> padEvent;
    
protected:
    
    void setupPad(ofPoint min, ofPoint max);
    void updateParameterOscAddress();
    void setupGuiPositions();
    void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);
    void selectPoint(float x, float y);
    void updateValueString();
    
    vector<ofxControl2dPadPoint*> points;
    ofPoint min, max;
    
    int idxActive;
    string valueString, valueStringNext;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
    bool connectPoints;
    
    ofRectangle padRectangle;
    bool mouseOverPad;
};


template<typename L, typename M>
ofxControl2dPad::ofxControl2dPad(ofxControlParameter<ofPoint> *parameter, L *listener, M method) : ofxControlMultiElement(parameter->getName())
{
    setupPad(parameter->getMin(), parameter->getMax());
    addPoint(parameter);
    ofAddListener(padEvent, listener, method);
}

template<typename L, typename M>
ofxControl2dPad::ofxControl2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method) : ofxControlMultiElement(name)
{
    setupPad(min, max);
    addPoint(new ofxControlParameter<ofPoint>(name, value, min, max));
    ofAddListener(padEvent, listener, method);
}

template<typename L, typename M>
ofxControl2dPad::ofxControl2dPad(string name, ofPoint min, ofPoint max, L *listener, M method) : ofxControlMultiElement(name)
{
    setupPad(min, max);
    ofAddListener(padEvent, listener, method);
}
