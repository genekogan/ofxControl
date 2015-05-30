#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlSlider.h"
#include "ofxControlMultiElement.h"
#include "ofxControlTouchOSC.h"



template<typename T> class ofxControlMultiSlider;

struct ofxControlMultiSliderEventArgsBase { };

template<typename T>
struct ofxControlMultiSliderEventArgs : public ofxControlMultiSliderEventArgsBase
{
    ofxControlMultiSlider<T> *slider;
    T value;
    
    ofxControlMultiSliderEventArgs(ofxControlMultiSlider<T> *slider, T value)
    {
        this->slider = slider;
        this->value = value;
    }
};


class ofxControlMultiSliderBase : public ofxControlMultiElement
{
public:
    ofxControlMultiSliderBase(string name);
    virtual ~ofxControlMultiSliderBase() { }
    
protected:
    
    virtual void createSliders() { }
    virtual void initializeSliders() { }
};


template<typename T>
class ofxControlMultiSlider : public ofxControlMultiSliderBase
{
public:

    ofxControlMultiSlider(ofxControlParameter<T> *parameter);
    ofxControlMultiSlider(string name, T *value, T min, T max);
    ofxControlMultiSlider(string name, T min, T max);
    
    template <typename L, typename M>
    ofxControlMultiSlider(ofxControlParameter<T> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlMultiSlider(string name, T *value, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlMultiSlider(string name, T min, T max, L *listener, M method);
    
    ~ofxControlMultiSlider();
    
    void setParameterValue(T value) {parameter->set(value);}
    T getParameterValue() {return parameter->get();}
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    
    ofEvent<ofxControlMultiSliderEventArgs<T> > sliderEvent;
    
    //void addElementToTouchOscLayout(TouchOscPage *page, float *y);
    

    
private:
    
    void createSliders();
    void initializeSliders();
    void updateParameterOscAddress();
    void sliderChanged(ofxControlSliderEventArgs<float> &e);
    
    
//    string getOscAddress() { }
//    void sendOsc(ofxOscMessage &msg) { }
//    void receiveOsc(ofxOscMessage &msg) { }
//    bool valueChanged() {}
//
    
    
    ofxControlParameter<T> *parameter;
};


/*
template<typename T>
void ofxControlMultiSlider<T>::addElementToTouchOscLayout(TouchOscPage *page, float *y)
{
    TouchOscMultiFader *fader = page->addMultiFader(getName(), 0.01, *y, 0.9, elements.size());
    
    fader->setOscAddress(parameter->getOscAddress());
    
    fader->setNumber(elements.size());
    
    fader->setCentered(false);
    fader->setInverted(false);
    
    fader->setType(0);

    //fader->setResponseRelative(false);

    *y += (elements.size() + 0.05);
}
 */



template<typename T>
ofxControlMultiSlider<T>::ofxControlMultiSlider(ofxControlParameter<T> *parameter) : ofxControlMultiSliderBase(parameter->getName())
{
    this->parameter = parameter;
    createSliders();
    initializeSliders();
}

template<typename T>
ofxControlMultiSlider<T>::ofxControlMultiSlider(string name, T *value, T min, T max) : ofxControlMultiSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, value, min, max);
    createSliders();
    initializeSliders();
}

template<typename T>
ofxControlMultiSlider<T>::ofxControlMultiSlider(string name, T min, T max) : ofxControlMultiSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, new T(), min, max);
    createSliders();
    initializeSliders();
}

template<typename T> template<typename L, typename M>
ofxControlMultiSlider<T>::ofxControlMultiSlider(ofxControlParameter<T> *parameter, L *listener, M method) : ofxControlMultiSliderBase(parameter->getName())
{
    this->parameter = parameter;
    createSliders();
    initializeSliders();
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
ofxControlMultiSlider<T>::ofxControlMultiSlider(string name, T *value, T min, T max, L *listener, M method) : ofxControlMultiSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, value, min, max);
    createSliders();
    initializeSliders();
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
ofxControlMultiSlider<T>::ofxControlMultiSlider(string name, T min, T max, L *listener, M method) : ofxControlMultiSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, new T(), min, max);
    createSliders();
    initializeSliders();
    ofAddListener(sliderEvent, listener, method);
}

template<typename T>
ofxControlMultiSlider<T>::~ofxControlMultiSlider<T>()
{
    delete parameter;
}

template<typename T>
void ofxControlMultiSlider<T>::createSliders() { }

template<> inline void ofxControlMultiSlider<ofVec2f>::createSliders()
{
    initializeElement(new ofxControlSlider<float>(name+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x));
    initializeElement(new ofxControlSlider<float>(name+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y));
}

template<> inline void ofxControlMultiSlider<ofVec3f>::createSliders()
{
    initializeElement(new ofxControlSlider<float>(name+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x));
    initializeElement(new ofxControlSlider<float>(name+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y));
    initializeElement(new ofxControlSlider<float>(name+".z", &parameter->getReference()->z, parameter->getMin().z, parameter->getMax().z));
}

template<> inline void ofxControlMultiSlider<ofVec4f>::createSliders()
{
    initializeElement(new ofxControlSlider<float>(name+".x", &parameter->getReference()->x, parameter->getMin().x, parameter->getMax().x));
    initializeElement(new ofxControlSlider<float>(name+".y", &parameter->getReference()->y, parameter->getMin().y, parameter->getMax().y));
    initializeElement(new ofxControlSlider<float>(name+".z", &parameter->getReference()->z, parameter->getMin().z, parameter->getMax().z));
    initializeElement(new ofxControlSlider<float>(name+".w", &parameter->getReference()->w, parameter->getMin().w, parameter->getMax().w));
}

template<typename T>
void ofxControlMultiSlider<T>::initializeSliders()
{
    for (auto e : elements) {
        ofAddListener(((ofxControlSlider<float> *) e)->sliderEvent, this, &ofxControlMultiSlider<T>::sliderChanged);
    }
}

template<typename T>
void ofxControlMultiSlider<T>::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

template<typename T>
void ofxControlMultiSlider<T>::sliderChanged(ofxControlSliderEventArgs<float> &e)
{
    ofxControlMultiSliderEventArgs<T> args(this, parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void ofxControlMultiSlider<T>::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}
