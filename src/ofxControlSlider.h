#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlSequence.h"
#include "ofxControlTouchOsc.h"


template <typename T> class ofxControlSlider;

struct ofxControlSliderEventArgsBase { };

template<typename T>
struct ofxControlSliderEventArgs : public ofxControlSliderEventArgsBase
{
    ofxControlSlider<T> *slider;
    T value;
    
    ofxControlSliderEventArgs(ofxControlSlider<T> *slider, T value)
    {
        this->slider = slider;
        this->value = value;
    }
};


class ofxControlSliderBase : public ofxControlElement
{
public:
    ofxControlSliderBase(string name);
    virtual ~ofxControlSliderBase();
    
    virtual void setValue(float sliderValue);
    float getValue() {return sliderValue;}
    
    void lerpTo(float nextValue, int numFrames);
    
    virtual void setMin(float min) { }
    virtual void setMax(float max) { }
    
    void setWarp(float warp) {this->warp = warp;}
    float getWarp() {return warp;}
    
    virtual void setExponential(bool exp) {this->exp = exp;}
    
    virtual void update();
    virtual void draw();
    
    virtual bool mouseMoved(int mouseX, int mouseY);
    virtual bool mousePressed(int mouseX, int mouseY);
    virtual bool mouseReleased(int mouseX, int mouseY);
    virtual bool mouseDragged(int mouseX, int mouseY);
    virtual bool keyPressed(int key);

    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
protected:
    
    void setValueFromSequence(ofxControlSequence &sequence);
    void setSequenceFromValue(ofxControlSequence &sequence, int column);

    void setEditing(bool editing);
    void keyboardEdit(int key);
    
    virtual void decrement() { }
    virtual void increment() { }
    
    virtual void setSequenceFromExplicitValue(ofxControlSequence &sequence, int column, float value) { }
    virtual void setParameterValueFromString(string valueString) { }
    virtual void updateValueString() { }
    virtual string getParameterValueString() { }
    
    float sliderValue;
    float warp;
    bool exp;
    int numExpSteps;
    bool changed;
    string valueString, valueStringNext;
    float lerpPrevValue, lerpNextValue;;
    int lerpFrame, lerpNumFrames;
    bool editing;
    string editingString;
    bool toUpdateValueString;
    float valueStringWidth, stringHeight;
};


template<typename T>
class ofxControlSlider : public ofxControlSliderBase
{
public:
    ofxControlSlider(ofxControlParameter<T> *parameter);
    ofxControlSlider(string name, T *value, T min, T max);
    ofxControlSlider(string name, T min, T max);
    
    template <typename L, typename M>
    ofxControlSlider(ofxControlParameter<T> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlSlider(string name, T *value, T min, T max, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlSlider(string name, T min, T max, L *listener, M method);
    
    ~ofxControlSlider();
    
    void setValue(float sliderValue);
    void setParameterValue(T value);
    
    T getParameterValue() {return parameter->get();}
    void getParameters(vector<ofxControlParameterBase*> & parameters);

    void setMin(float min);
    void setMax(float max);

    void setExponential(bool exp)
    {
        ofxControlSliderBase::setExponential(exp);
        if (exp) {
            numExpSteps = ceil(log((float) parameter->getMax() / (float) parameter->getMin()) / log(2));
        }
    }
    
    void update();
    
    ofEvent<ofxControlSliderEventArgs<T> > sliderEvent;
    
private:

    void decrement();
    void increment();
    
    void setSequenceFromExplicitValue(ofxControlSequence &sequence, int column, float value);
    void setParameterValueFromString(string valueString);
    void adjustSliderValue();
    void updateValueString();
    string getParameterValueString() {return ofToString(parameter->get(), 2);}
    
    void updateParameterOscAddress();
    string getOscAddress() {return parameter->getOscAddress(); }
    void sendOsc(ofxOscMessage &msg);
    void receiveOsc(ofxOscMessage &msg) {setValue(msg.getArgAsFloat(0));}
    bool valueChanged();
    void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);

    ofxControlParameter<T> *parameter;
    T previous;
    ofxControlSequence *sequence;
};


template<typename T>
ofxControlSlider<T>::ofxControlSlider(ofxControlParameter<T> *parameter) : ofxControlSliderBase(parameter->getName())
{
    this->parameter = parameter;
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
}

template<typename T>
ofxControlSlider<T>::ofxControlSlider(string name, T *value, T min, T max) : ofxControlSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, value, min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
}

template<typename T>
ofxControlSlider<T>::ofxControlSlider(string name, T min, T max) : ofxControlSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, new T(), min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
}

template<typename T> template<typename L, typename M>
ofxControlSlider<T>::ofxControlSlider(ofxControlParameter<T> *parameter, L *listener, M method) : ofxControlSliderBase(parameter->getName())
{
    this->parameter = parameter;
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
ofxControlSlider<T>::ofxControlSlider(string name, T *value, T min, T max, L *listener, M method) : ofxControlSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, value, min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
    ofAddListener(sliderEvent, listener, method);
}

template<typename T> template<typename L, typename M>
ofxControlSlider<T>::ofxControlSlider(string name, T min, T max, L *listener, M method) : ofxControlSliderBase(name)
{
    parameter = new ofxControlParameter<T>(name, new T(), min, max);
    setValue(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
    ofAddListener(sliderEvent, listener, method);
}

template<typename T>
ofxControlSlider<T>::~ofxControlSlider<T>()
{
    delete parameter;
}

template<typename T>
void ofxControlSlider<T>::setValue(float sliderValue)
{
    if (exp) {
        ofxControlSliderBase::setValue(sliderValue - fmodf(sliderValue, 1.0 / numExpSteps));
    }
    else {
        ofxControlSliderBase::setValue(sliderValue);
    }

    //parameter->set(sliderValue * parameter->getMax() + (1.0 - sliderValue) * parameter->getMin());
    
    if (exp) {
        sliderValue = sliderValue - fmodf(sliderValue, 1.0 / numExpSteps);
        parameter->set(parameter->getMin() * pow(2.0f, sliderValue * numExpSteps));
    }
    else if (warp == 1.0) {
        parameter->set(sliderValue * parameter->getMax() + (1.0 - sliderValue) * parameter->getMin());
    }
    else {
        float sliderValueWarped = pow(sliderValue, warp);
        parameter->set(sliderValueWarped * parameter->getMax() + (1.0 - sliderValueWarped) * parameter->getMin());
    }

    updateValueString();
    adjustSliderValue();
    ofxControlSliderEventArgs<T> args(this, parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void ofxControlSlider<T>::setParameterValue(T value)
{
    parameter->set(value);
    
    //sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
//    sliderValue = pow((float) (ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0)), 1.0f / warp);
    if (exp) {
        sliderValue = log(parameter->get() / parameter->getMin()) / (numExpSteps * log(2));
    }
    else {
        sliderValue = pow((float) (ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0)), 1.0f / warp);
    }
    
    
    updateValueString();
    adjustSliderValue();    // this can just be an inline int for setValue<int> instead
    ofxControlSliderEventArgs<T> args(this, parameter->get());
    ofNotifyEvent(sliderEvent, args, this);
}

template<typename T>
void ofxControlSlider<T>::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}

template<typename T>
void ofxControlSlider<T>::setParameterValueFromString(string valueString)
{
    setParameterValue(ofToDouble(valueString));
}

template<typename T>
void ofxControlSlider<T>::decrement()
{
    setValue(ofClamp(sliderValue - 0.01, 0.0, 1.0));
}

template<> inline void ofxControlSlider<int>::decrement()
{
    setParameterValue(ofClamp(getParameterValue() - 1, parameter->getMin(), parameter->getMax()));
}

template<typename T>
void ofxControlSlider<T>::increment()
{
    setValue(ofClamp(sliderValue + 0.01, 0.0, 1.0));
}

template<> inline void ofxControlSlider<int>::increment()
{
    setParameterValue(ofClamp(getParameterValue() + 1, parameter->getMin(), parameter->getMax()));
}

template<typename T>
void ofxControlSlider<T>::setSequenceFromExplicitValue(ofxControlSequence &sequence, int column, float value)
{
    float sequenceValue = ofClamp((float) (value - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    sequence.setValueAtCell(column, sequenceValue);
}

template<typename T>
void ofxControlSlider<T>::updateValueString()
{
    valueStringNext = ofToString(parameter->get(), floor(parameter->get()) == parameter->get() ? 0 : 2);
    toUpdateValueString = true;
}

template<typename T>
void ofxControlSlider<T>::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

template<typename T>
void ofxControlSlider<T>::sendOsc(ofxOscMessage &msg)
{
    msg.addFloatArg(parameter->get());
}

template<>
inline void ofxControlSlider<int>::sendOsc(ofxOscMessage &msg)
{
    msg.addIntArg(parameter->get());
}

template<typename T>
bool ofxControlSlider<T>::valueChanged()
{
    if (changed)
    {
        changed = false;
        return true;
    }
    else {
        return false;
    }
}

template<typename T>
void ofxControlSlider<T>::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{
    ofxControlTouchOscFader *fader = page->addFader(getName(), 0.01, *y, 0.98, 1);
    ofxControlTouchOscLabel *label = page->addLabel(getName(), 0.01, *y, 0.98, 1);
    fader->setOscAddress(parameter->getOscAddress());
    fader->setType(0);
    label->setType(0);
    label->setColor(RED);
    *y += 1.04;
}

template<typename T>
void ofxControlSlider<T>::update()
{
    ofxControlSliderBase::update();
    if (previous != parameter->get())
    {
        //ofxControlSliderBase::setValue(ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0));
        if (exp) {
            ofxControlSliderBase::setValue(log(parameter->get() / parameter->getMin()) / (numExpSteps * log(2)));
        }
        else {
            ofxControlSliderBase::setValue(pow((float) (ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0)), 1.0f / warp));
        }
        
        updateValueString();
        adjustSliderValue();
        previous = parameter->get();
    }
}

template<typename T>
void ofxControlSlider<T>::setMin(float min)
{
    parameter->setMin(min);
    
    
    //sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    if (exp) {
        sliderValue = log(parameter->get() / parameter->getMin()) / (numExpSteps * log(2));
    }
    else {
        sliderValue = pow((float) (ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0)), 1.0f / warp);
    }
    
    updateValueString();
    adjustSliderValue();
}

template<typename T>
void ofxControlSlider<T>::setMax(float max)
{
    parameter->setMax(max);
    
    
    //sliderValue = ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    if (exp) {
        sliderValue = log(parameter->get() / parameter->getMin()) / (numExpSteps * log(2));
    }
    else {
        sliderValue = pow((float) (ofClamp((parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0)), 1.0f / warp);
    }
    
    updateValueString();
    adjustSliderValue();
}

template<typename T> inline void ofxControlSlider<T>::adjustSliderValue() { }

template<> inline void ofxControlSlider<int>::adjustSliderValue()
{
    //sliderValue = ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0);
    
    if (exp) {
        sliderValue = log(parameter->get() / parameter->getMin()) / (numExpSteps * log(2));
    }
    else {
        sliderValue = pow(ofClamp((float) (parameter->get() - parameter->getMin()) / (parameter->getMax() - parameter->getMin()), 0.0, 1.0), 1.0f / warp);
    }
}
