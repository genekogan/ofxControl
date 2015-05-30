#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlRangeSlider.h"
#include "ofxControlMultiElement.h"


template<typename T> class ofxControlMultiRangeSlider;

struct ofxControlMultiRangeSliderEventArgsBase { };

template<typename T>
struct ofxControlMultiRangeSliderEventArgs : public ofxControlMultiRangeSliderEventArgsBase
{
    ofxControlMultiRangeSlider<T> *slider;
    T low;
    T high;
    
    ofxControlMultiRangeSliderEventArgs(ofxControlMultiRangeSlider<T> *slider, T low, T high)
    {
        this->slider = slider;
        this->low = low;
        this->high = high;
    }
};


class ofxControlMultiRangeSliderBase : public ofxControlMultiElement
{
public:
    ofxControlMultiRangeSliderBase(string name);
    virtual ~ofxControlMultiRangeSliderBase() { }
    
protected:
    
    virtual void createRangeSliders() { }
    virtual void initializeRangeSliders() { }
};


template<typename T>
class ofxControlMultiRangeSlider : public ofxControlMultiRangeSliderBase
{
public:
    
    ofxControlMultiRangeSlider(ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh);
    ofxControlMultiRangeSlider(string name, T *low, T *high, T min, T max);
    ofxControlMultiRangeSlider(string name, T min, T max);
    
    template <typename L, typename M>
    ofxControlMultiRangeSlider(ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlMultiRangeSlider(string name, T *low, T *high, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlMultiRangeSlider(string name, T min, T max, L *listener, M method);
    
    ~ofxControlMultiRangeSlider();
    
    void setParameterValueLow(T value) {parameterLow->set(value);}
    void setParameterValueHigh(T value) {parameterHigh->set(value);}
    T getParameterValueLow() {return parameterLow->get();}
    T getParameterValueHigh() {return parameterHigh->get();}
    
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    
    ofEvent<ofxControlMultiRangeSliderEventArgs<T> > rangeSliderEvent;
    
private:
    
    void createRangeSliders();
    void initializeRangeSliders();
    void updateParameterOscAddress();
    
    
//    string getOscAddress() { }
//    void sendOsc(ofxOscMessage &msg) { }
//    void receiveOsc(ofxOscMessage &msg) { }
//    bool valueChanged() {}

    
    
    void rangeSliderChanged(ofxControlRangeSliderEventArgs<float> &e);
    
    ofxControlParameter<T> *parameterLow, *parameterHigh;
};


template<typename T>
ofxControlMultiRangeSlider<T>::ofxControlMultiRangeSlider(ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh) : ofxControlMultiRangeSliderBase(parameterLow->getName())
{
    this->parameterLow = parameterLow;
    this->parameterHigh = parameterHigh;
    createRangeSliders();
    initializeRangeSliders();
}

template<typename T>
ofxControlMultiRangeSlider<T>::ofxControlMultiRangeSlider(string name, T *low, T *high, T min, T max) : ofxControlMultiRangeSliderBase(name)
{
    parameterLow = new ofxControlParameter<T>(name, low, min, max);
    parameterHigh = new ofxControlParameter<T>(name, high, min, max);
    createRangeSliders();
    initializeRangeSliders();
}

template<typename T>
ofxControlMultiRangeSlider<T>::ofxControlMultiRangeSlider(string name, T min, T max) : ofxControlMultiRangeSliderBase(name)
{
    parameterLow = new ofxControlParameter<T>(name, new T(min + 0.33 * (max - min)), min, max);
    parameterHigh = new ofxControlParameter<T>(name, new T(min + 0.67 * (max - min)), min, max);
    createRangeSliders();
    initializeRangeSliders();
}

template<typename T> template<typename L, typename M>
ofxControlMultiRangeSlider<T>::ofxControlMultiRangeSlider(ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh, L *listener, M method) : ofxControlMultiRangeSliderBase(parameterLow->getName())
{
    this->parameterLow = parameterLow;
    this->parameterHigh = parameterHigh;
    createRangeSliders();
    initializeRangeSliders();
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
ofxControlMultiRangeSlider<T>::ofxControlMultiRangeSlider(string name, T *low, T *high, T min, T max, L *listener, M method) : ofxControlMultiRangeSliderBase(name)
{
    parameterLow = new ofxControlParameter<T>(name, low, min, max);
    parameterHigh = new ofxControlParameter<T>(name, high, min, max);
    createRangeSliders();
    initializeRangeSliders();
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
ofxControlMultiRangeSlider<T>::ofxControlMultiRangeSlider(string name, T min, T max, L *listener, M method) : ofxControlMultiRangeSliderBase(name)
{
    parameterLow = new ofxControlParameter<T>(name, new T(min + 0.33 * (max - min)), min, max);
    parameterHigh = new ofxControlParameter<T>(name, new T(min + 0.67 * (max - min)), min, max);
    createRangeSliders();
    initializeRangeSliders();
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T>
ofxControlMultiRangeSlider<T>::~ofxControlMultiRangeSlider<T>()
{
    delete parameterLow;
    delete parameterHigh;
}

template<typename T>
void ofxControlMultiRangeSlider<T>::updateParameterOscAddress()
{
    parameterLow->setOscAddress(getAddress());
    parameterHigh->setOscAddress(getAddress());
}

template<typename T>
void ofxControlMultiRangeSlider<T>::createRangeSliders() { }

template<> inline void ofxControlMultiRangeSlider<ofVec2f>::createRangeSliders()
{
    initializeElement(new ofxControlRangeSlider<float>(name+".x", &parameterLow->getReference()->x, &parameterHigh->getReference()->x, min(parameterLow->getMin().x, parameterHigh->getMin().x), max(parameterLow->getMax().x, parameterHigh->getMax().x)));
    initializeElement(new ofxControlRangeSlider<float>(name+".y", &parameterLow->getReference()->y, &parameterHigh->getReference()->y, min(parameterLow->getMin().y, parameterHigh->getMin().y), max(parameterLow->getMax().y, parameterHigh->getMax().y)));
}

template<> inline void ofxControlMultiRangeSlider<ofVec3f>::createRangeSliders()
{
    initializeElement(new ofxControlRangeSlider<float>(name+".x", &parameterLow->getReference()->x, &parameterHigh->getReference()->x, min(parameterLow->getMin().x, parameterHigh->getMin().x), max(parameterLow->getMax().x, parameterHigh->getMax().x)));
    initializeElement(new ofxControlRangeSlider<float>(name+".y", &parameterLow->getReference()->y, &parameterHigh->getReference()->y, min(parameterLow->getMin().y, parameterHigh->getMin().y), max(parameterLow->getMax().y, parameterHigh->getMax().y)));
    initializeElement(new ofxControlRangeSlider<float>(name+".z", &parameterLow->getReference()->z, &parameterHigh->getReference()->z, min(parameterLow->getMin().z, parameterHigh->getMin().z), max(parameterLow->getMax().z, parameterHigh->getMax().z)));
}

template<> inline void ofxControlMultiRangeSlider<ofVec4f>::createRangeSliders()
{
    initializeElement(new ofxControlRangeSlider<float>(name+".x", &parameterLow->getReference()->x, &parameterHigh->getReference()->x, min(parameterLow->getMin().x, parameterHigh->getMin().x), max(parameterLow->getMax().x, parameterHigh->getMax().x)));
    initializeElement(new ofxControlRangeSlider<float>(name+".y", &parameterLow->getReference()->y, &parameterHigh->getReference()->y, min(parameterLow->getMin().y, parameterHigh->getMin().y), max(parameterLow->getMax().y, parameterHigh->getMax().y)));
    initializeElement(new ofxControlRangeSlider<float>(name+".z", &parameterLow->getReference()->z, &parameterHigh->getReference()->z, min(parameterLow->getMin().z, parameterHigh->getMin().z), max(parameterLow->getMax().z, parameterHigh->getMax().z)));
    initializeElement(new ofxControlRangeSlider<float>(name+".w", &parameterLow->getReference()->w, &parameterHigh->getReference()->w, min(parameterLow->getMin().w, parameterHigh->getMin().w), max(parameterLow->getMax().w, parameterHigh->getMax().w)));
}

template<typename T>
void ofxControlMultiRangeSlider<T>::initializeRangeSliders()
{
    for (auto e : elements) {
        ofAddListener(((ofxControlRangeSlider<float> *) e)->rangeSliderEvent, this, &ofxControlMultiRangeSlider<T>::rangeSliderChanged);
    }
}

template<typename T>
void ofxControlMultiRangeSlider<T>::rangeSliderChanged(ofxControlRangeSliderEventArgs<float> &e)
{
    ofxControlMultiRangeSliderEventArgs<T> args(this, parameterLow->get(), parameterHigh->get());
    ofNotifyEvent(rangeSliderEvent, args, this);
}

template<typename T>
void ofxControlMultiRangeSlider<T>::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(parameterLow);
    parameters.push_back(parameterHigh);
}
