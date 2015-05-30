#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlSequence.h"


template<typename T> class ofxControlRangeSlider;

struct ofxControlRangeSliderEventArgsBase { };

template<typename T>
struct ofxControlRangeSliderEventArgs : public ofxControlRangeSliderEventArgsBase
{
    ofxControlRangeSlider<T> *slider;
    T low, high;
    
    ofxControlRangeSliderEventArgs(ofxControlRangeSlider<T> *slider, T low, T high)
    {
        this->slider = slider;
        this->low = low;
        this->high = high;
    }
};


class ofxControlRangeSliderBase : public ofxControlElement
{
public:
    enum {LOW, MIDDLE, HIGH};
    
    ofxControlRangeSliderBase(string name);
    virtual ~ofxControlRangeSliderBase() { }
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void setValue(float sliderValue);
    virtual void setValueLow(float sliderLow, bool toSendNotification=true);
    virtual void setValueHigh(float sliderHigh, bool toSendNotification=true);
    
    float getValueLow() {return sliderLow;}
    float getValueHigh() {return sliderHigh;}
    
    void lerpTo(float nextLow, float nextHigh, int numFrames);
    void setValueFromSequence(ofxControlSequence &sequence);
    
    virtual void update();
    void draw();
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
protected:
    
    void selectSlider(float sliderValue);
    void keyboardEdit(int key);
    virtual void updateValueString() { }
    
    virtual void decrement() { }
    virtual void increment() { }
    
    int selection;
    float sliderLow, sliderHigh, sliderValuePrev;
    string valueString, valueStringNext;
    float lerpPrevLow, lerpNextLow;
    float lerpPrevHigh, lerpNextHigh;
    int lerpFrame, lerpNumFrames;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
};


template<typename T>
class ofxControlRangeSlider : public ofxControlRangeSliderBase
{
public:

    ofxControlRangeSlider(string name, ofxControlParameter<T> *pLow, ofxControlParameter<T> *pHigh);
    ofxControlRangeSlider(string name, T *low, T *high, T min, T max);
    ofxControlRangeSlider(string name, T min, T max);
    
    template <typename L, typename M>
    ofxControlRangeSlider(string name, ofxControlParameter<T> *pLow, ofxControlParameter<T> *pHigh, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlRangeSlider(string name, T *low, T *high, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlRangeSlider(string name, T min, T max, L *listener, M method);
    
    ~ofxControlRangeSlider();
    
    void setValueLow(float sliderLow, bool toSendNotification=true);
    void setValueHigh(float sliderHigh, bool toSendNotification=true);
    
    void setParameterLowValue(T low, bool toSendNotification=true);
    void setParameterHighValue(T high, bool toSendNotification=true);
    
    T getParameterLowValue() {return pLow->get();}
    T getParameterHighValue() {return pHigh->get();}
    
    void getParameters(vector<ofxControlParameterBase*> & parameters);

    void update();
    
    ofEvent<ofxControlRangeSliderEventArgs<T> > rangeSliderEvent;
    
private:
    
    void decrement();
    void increment();
    
    void updateValueString();
    void adjustSliderValueLow();
    void adjustSliderValueHigh();
    void updateParameterOscAddress();
    
    ofxControlParameter<T> *pLow, *pHigh;
    T previousLow, previousHigh;
};

template<typename T>
ofxControlRangeSlider<T>::ofxControlRangeSlider(string name, ofxControlParameter<T> *pLow, ofxControlParameter<T> *pHigh) : ofxControlRangeSliderBase(name)
{
    this->pLow = pLow;
    this->pHigh = pHigh;
    setValueLow(ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
}

template<typename T>
ofxControlRangeSlider<T>::ofxControlRangeSlider(string name, T *low, T *high, T min, T max) : ofxControlRangeSliderBase(name)
{
    pLow = new ofxControlParameter<T>(name, low, min, max);
    pHigh = new ofxControlParameter<T>(name, high, min, max);
    setValueLow(ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
}

template<typename T>
ofxControlRangeSlider<T>::ofxControlRangeSlider(string name, T min, T max) : ofxControlRangeSliderBase(name)
{
    pLow = new ofxControlParameter<T>(name, new T(), min, max);
    pHigh = new ofxControlParameter<T>(name, new T(), min, max);
    setValueLow(ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
}

template<typename T> template <typename L, typename M>
ofxControlRangeSlider<T>::ofxControlRangeSlider(string name, ofxControlParameter<T> *pLow, ofxControlParameter<T> *pHigh, L *listener, M method) : ofxControlRangeSliderBase(name)
{
    this->pLow = pLow;
    this->pHigh = pHigh;
    setValueLow(ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T> template <typename L, typename M>
ofxControlRangeSlider<T>::ofxControlRangeSlider(string name, T *low, T *high, T min, T max, L *listener, M method) : ofxControlRangeSliderBase(name)
{
    pLow = new ofxControlParameter<T>(name, low, min, max);
    pHigh = new ofxControlParameter<T>(name, high, min, max);
    setValueLow(ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T> template <typename L, typename M>
ofxControlRangeSlider<T>::ofxControlRangeSlider(string name, T min, T max, L *listener, M method) : ofxControlRangeSliderBase(name)
{
    pLow = new ofxControlParameter<T>(name, new T(), min, max);
    pHigh = new ofxControlParameter<T>(name, new T(), min, max);
    setValueLow(ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0));
    setValueHigh(ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0));
    ofAddListener(rangeSliderEvent, listener, method);
}

template<typename T>
ofxControlRangeSlider<T>::~ofxControlRangeSlider<T>()
{
    delete pLow;
    delete pHigh;
}

template<typename T>
void ofxControlRangeSlider<T>::setValueLow(float sliderLow, bool toSendNotification)
{
    this->sliderLow = sliderLow;
    pLow->set(sliderLow * pLow->getMax() + (1.0 - sliderLow) * pLow->getMin());
    updateValueString();
    adjustSliderValueLow();
    ofxControlRangeSliderEventArgs<T> args(this, pLow->get(), pHigh->get());
    if (toSendNotification) {
        ofNotifyEvent(rangeSliderEvent, args, this);
    }
}

template<typename T>
void ofxControlRangeSlider<T>::setValueHigh(float sliderHigh, bool toSendNotification)
{
    this->sliderHigh = sliderHigh;
    pHigh->set(sliderHigh * pHigh->getMax() + (1.0 - sliderHigh) * pHigh->getMin());
    updateValueString();
    adjustSliderValueHigh();
    ofxControlRangeSliderEventArgs<T> args(this, pLow->get(), pHigh->get());
    if (toSendNotification) {
        ofNotifyEvent(rangeSliderEvent, args, this);
    }
}

template<typename T>
void ofxControlRangeSlider<T>::setParameterLowValue(T low, bool toSendNotification)
{
    pLow->set(low);
    sliderLow = (float) ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValueLow();
    ofxControlRangeSliderEventArgs<T> args(this, pLow->get(), pHigh->get());
    if (toSendNotification) {
        ofNotifyEvent(rangeSliderEvent, args, this);
    }
}

template<typename T>
void ofxControlRangeSlider<T>::setParameterHighValue(T high, bool toSendNotification)
{
    pHigh->set(high);
    sliderHigh = (float) ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0);
    updateValueString();
    adjustSliderValueHigh();
    ofxControlRangeSliderEventArgs<T> args(this, pLow->get(), pHigh->get());
    if (toSendNotification) {
        ofNotifyEvent(rangeSliderEvent, args, this);
    }
}

template<typename T>
void ofxControlRangeSlider<T>::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(pLow);
    parameters.push_back(pHigh);
}

template<typename T>
void ofxControlRangeSlider<T>::updateValueString()
{
    valueStringNext = "("+ofToString(pLow->get(), floor(pLow->get()) == pLow->get() ? 0 : 2)+","+ofToString(pHigh->get(), floor(pHigh->get()) == pHigh->get() ? 0 : 2)+")";
    toUpdateValueString = true;
}

template<typename T>
void ofxControlRangeSlider<T>::update()
{
    ofxControlRangeSliderBase::update();
    if (previousLow != pLow->get())
    {
        this->sliderLow = (float) ofClamp((pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0);
        updateValueString();
        adjustSliderValueLow();
        previousLow = pLow->get();
    }
    if (previousHigh != pHigh->get())
    {
        this->sliderHigh = (float) ofClamp((pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0);
        updateValueString();
        adjustSliderValueHigh();
        previousHigh = pHigh->get();
    }
}

template<> inline void ofxControlRangeSlider<int>::decrement()
{
    if (selection == LOW) {
        setParameterLowValue((float) ofClamp(getParameterLowValue() - 1, pLow->getMin(), pLow->getMax()));
    }
    else if (selection == HIGH) {
        setParameterHighValue((float) ofClamp(getParameterHighValue() - 1, pHigh->getMin(), pHigh->getMax()));
    }
    else if (selection == MIDDLE)
    {
        setParameterLowValue((float) ofClamp(getParameterLowValue() - 1, pLow->getMin(), pLow->getMax()));
        setParameterHighValue((float) ofClamp(getParameterHighValue() - 1, pHigh->getMin(), pHigh->getMax()));
    }
}

template<typename T>
void ofxControlRangeSlider<T>::decrement()
{
    if (selection == LOW) {
        setValueLow(ofClamp(sliderLow - 0.01, 0.0, 1.0));
    }
    else if (selection == HIGH) {
        setValueHigh(ofClamp(sliderHigh - 0.01, 0.0, 1.0));
    }
    else if (selection == MIDDLE)
    {
        setValueLow(ofClamp(sliderLow - 0.01, 0.0, 1.0));
        setValueHigh(ofClamp(sliderHigh - 0.01, 0.0, 1.0));
    }
}

template<> inline void ofxControlRangeSlider<int>::increment()
{
    if (selection == LOW) {
        setParameterLowValue((float) ofClamp(getParameterLowValue() + 1, pLow->getMin(), pLow->getMax()));
    }
    else if (selection == HIGH) {
        setParameterHighValue((float) ofClamp(getParameterHighValue() + 1, pHigh->getMin(), pHigh->getMax()));
    }
    else if (selection == MIDDLE)
    {
        setParameterLowValue((float) ofClamp(getParameterLowValue() + 1, pLow->getMin(), pLow->getMax()));
        setParameterHighValue((float) ofClamp(getParameterHighValue() + 1, pHigh->getMin(), pHigh->getMax()));
    }
}

template<typename T>
void ofxControlRangeSlider<T>::increment()
{
    if (selection == LOW) {
        setValueLow(ofClamp(sliderLow + 0.01, 0.0, 1.0));
    }
    else if (selection == HIGH) {
        setValueHigh(ofClamp(sliderHigh + 0.01, 0.0, 1.0));
    }
    else if (selection == MIDDLE)
    {
        setValueLow(ofClamp(sliderLow + 0.01, 0.0, 1.0));
        setValueHigh(ofClamp(sliderHigh + 0.01, 0.0, 1.0));
    }
}

template<typename T> inline void ofxControlRangeSlider<T>::adjustSliderValueLow() { }
template<typename T> inline void ofxControlRangeSlider<T>::adjustSliderValueHigh() { }

template<> inline void ofxControlRangeSlider<int>::adjustSliderValueLow()
{
    sliderLow = ofClamp((float) (pLow->get() - pLow->getMin()) / (pLow->getMax() - pLow->getMin()), 0.0, 1.0);
}
template<> inline void ofxControlRangeSlider<int>::adjustSliderValueHigh()
{
    sliderHigh = ofClamp((float) (pHigh->get() - pHigh->getMin()) / (pHigh->getMax() - pHigh->getMin()), 0.0, 1.0);
}

template<typename T>
void ofxControlRangeSlider<T>::updateParameterOscAddress()
{
    pLow->setOscAddress(getAddress()+"/low");
    pHigh->setOscAddress(getAddress()+"/high");
}
