#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlSlider.h"
#include "ofxControlMultiElement.h"


class ofxControlColor;

struct ofxControlColorEventArgs
{
    ofxControlColor *slider;
    ofFloatColor color;
    
    ofxControlColorEventArgs(ofxControlColor *slider, ofFloatColor color)
    {
        this->slider = slider;
        this->color = color;
    }
};


class ofxControlColor : public ofxControlMultiElement
{
public:
    ofxControlColor(ofxControlParameter<ofFloatColor> *parameter);
    ofxControlColor(string name, ofFloatColor *color);
    ofxControlColor(string name);
    
    template <typename L, typename M>
    ofxControlColor(ofxControlParameter<ofFloatColor> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlColor(string name, ofFloatColor *color, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlColor(string name, L *listener, M method);
    
    ~ofxControlColor();
    
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    void setParameterValue(ofFloatColor color) {parameter->set(color);}
    ofFloatColor getParameterValue() {return parameter->get();}
    
    void update();
    
    ofEvent<ofxControlColorEventArgs> colorEvent;
    
private:
    
    void setupColor();
    void updateParameterOscAddress();
    void sliderChanged(ofxControlSliderEventArgs<float> &e);
    
    ofxControlParameter<ofFloatColor> *parameter;
};


template<typename L, typename M>
ofxControlColor::ofxControlColor(ofxControlParameter<ofFloatColor> *parameter, L *listener, M method) : ofxControlMultiElement(parameter->getName())
{
    this->parameter = parameter;
    setupColor();
    ofAddListener(colorEvent, listener, method);
}

template<typename L, typename M>
ofxControlColor::ofxControlColor(string name, ofFloatColor *color, L *listener, M method) : ofxControlMultiElement(name)
{
    parameter = new ofxControlParameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
    ofAddListener(colorEvent, listener, method);
}

template<typename L, typename M>
ofxControlColor::ofxControlColor(string name, L *listener, M method) : ofxControlMultiElement(name)
{
    parameter = new ofxControlParameter<ofFloatColor>(name, new ofFloatColor(0.5, 0.5, 0.5, 1.0), ofFloatColor(0), ofFloatColor(1, 1, 1, 1));
    setupColor();
    ofAddListener(colorEvent, listener, method);
}
