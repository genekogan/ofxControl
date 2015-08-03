#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlTextBox.h"
#include "ofxControlButton.h"
#include "ofxControlSlider.h"
#include "ofxControlRangeSlider.h"
#include "ofxControlMultiElement.h"
#include "ofxControlMultiSlider.h"
#include "ofxControlMultiRangeSlider.h"
#include "ofxControl2dPad.h"
#include "ofxControlColor.h"
#include "ofxControlMenu.h"
#include "ofxControlTouchOsc.h"


class ofxControlOscManager;


class ofxControlWidget : public ofxControlMultiElement
{
public:
    ofxControlWidget(string name);
    ofxControlWidget();

    ofxControlWidget * addWidget(ofxControlWidget *newWidget);
    ofxControlWidget * addWidget(string widgetName);

    ofxControlButton * addButton(ofxControlParameter<bool> *parameter);
    ofxControlButton * addButton(string name, bool *value);
    template<typename L, typename M>
    ofxControlButton * addButton(ofxControlParameter<bool> *parameter, L *listener, M method);
    template<typename L, typename M>
    ofxControlButton * addButton(string name, bool *value, L *listener, M method);
    template<typename L, typename M>
    ofxControlButton * addButton(string name, L *listener, M method);
    
    ofxControlToggle * addToggle(ofxControlParameter<bool> *parameter);
    ofxControlToggle * addToggle(string name, bool *value);
    template<typename L, typename M>
    ofxControlToggle * addToggle(ofxControlParameter<bool> *parameter, L *listener, M method);
    template<typename L, typename M>
    ofxControlToggle * addToggle(string name, bool *value, L *listener, M method);
    template<typename L, typename M>
    ofxControlToggle * addToggle(string name, L *listener, M method);

    ofxControlTextBox * addTextBox(ofxControlParameter<string> *parameter);
    ofxControlTextBox * addTextBox(string name, string *value);
    template<typename L, typename M>
    ofxControlTextBox * addTextBox(ofxControlParameter<string> *parameter, L *listener, M method);
    template<typename L, typename M>
    ofxControlTextBox * addTextBox(string name, string *value, L *listener, M method);
    template<typename L, typename M>
    ofxControlTextBox * addTextBox(string name, L *listener, M method);
    
    template<typename T>
    ofxControlSlider<T> * addSlider(ofxControlParameter<T> *parameter);
    template<typename T>
    ofxControlSlider<T> * addSlider(string name, T *value, T min, T max);
    template<typename T, typename L, typename M>
    ofxControlSlider<T> * addSlider(ofxControlParameter<T> *parameter, L *listener, M method);
    template<typename T, typename L, typename M>
    ofxControlSlider<T> * addSlider(string name, T *value, T min, T max, L *listener, M method);

    template<typename T>
    ofxControlMultiSlider<T> * addMultiSlider(ofxControlParameter<T> *parameter);
    template<typename T>
    ofxControlMultiSlider<T> * addMultiSlider(string name, T *value, T min, T max);
    template<typename T, typename L, typename M>
    ofxControlMultiSlider<T> * addMultiSlider(ofxControlParameter<T> *parameter, L *listener, M method);
    template<typename T, typename L, typename M>
    ofxControlMultiSlider<T> * addMultiSlider(string name, T *value, T min, T max, L *listener, M method);

    template<typename T>
    ofxControlRangeSlider<T> * addRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh);
    template<typename T>
    ofxControlRangeSlider<T> * addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max);
    template<typename T, typename L, typename M>
    ofxControlRangeSlider<T> * addRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh, L *listener, M method);
    template<typename T, typename L, typename M>
    ofxControlRangeSlider<T> * addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method);
    template<typename T, typename L, typename M>
    ofxControlRangeSlider<T> * addRangeSlider(string name, T min, T max, L *listener, M method);
    
    template<typename T>
    ofxControlMultiRangeSlider<T> * addMultiRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh);
    template<typename T>
    ofxControlMultiRangeSlider<T> * addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max);
    template<typename T, typename L, typename M>
    ofxControlMultiRangeSlider<T> * addMultiRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh, L *listener, M method);
    template<typename T, typename L, typename M>
    ofxControlMultiRangeSlider<T> * addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method);
    template<typename T, typename L, typename M>
    ofxControlMultiRangeSlider<T> * addMultiRangeSlider(string name, T min, T max, L *listener, M method);

    ofxControl2dPad * add2dPad(ofxControlParameter<ofPoint> *parameter);
    ofxControl2dPad * add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max);
    ofxControl2dPad * add2dPad(string name, ofPoint min, ofPoint max);
    template<typename L, typename M>
    ofxControl2dPad * add2dPad(ofxControlParameter<ofPoint> *parameter, L *listener, M method);
    template<typename L, typename M>
    ofxControl2dPad * add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method);
    template<typename L, typename M>
    ofxControl2dPad * add2dPad(string name, ofPoint min, ofPoint max, L *listener, M method);
    
    ofxControlColor * addColor(ofxControlParameter<ofFloatColor> *parameter);
    ofxControlColor * addColor(string name, ofFloatColor *value);
    template<typename L, typename M>
    ofxControlColor * addColor(ofxControlParameter<ofFloatColor> *parameter, L *listener, M method);
    template<typename L, typename M>
    ofxControlColor * addColor(string name, ofFloatColor *value, L *listener, M method);
    
    ofxControlMenu * addMenu(string name, vector<string> choices, bool multipleChoice=false, bool autoClose=false);
    ofxControlMenu * addMenu(string name, bool multipleChoice=false, bool autoClose=false);
    template<typename L, typename M>
    ofxControlMenu * addMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    template<typename L, typename M>
    ofxControlMenu * addMenu(string name, L *listener, M method, bool multipleChoice=false, bool autoClose=false);

    void attachWidget(ofxControlWidget *other);
    void detachWidget(ofxControlWidget *other);

    virtual void createOscManager(ofxControlOscManager *osc) {}
    void makeTouchOscLayout(string filename);

    vector<ofxControlElement*> & getElementGroups() {return elementGroups;}
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    void setupGuiPositions();
    void addElementToTouchOscLayout(ofxControlElement *element, ofxControlTouchOscPage *page, float *y);
    
    virtual void getXml(ofXml &xml);
    virtual void setFromXml(ofXml &xml);
    
private:
    
    void initializeElement(ofxControlElement *element, bool sendNotification=true);
    void updateParameterOscAddress();
    void eventAddElement(ofxControlElement * &element);
    void eventRemoveElement(ofxControlElement * &element);
    void widgetNewElementAdded(ofxControlElement* & newElement);
    
    vector<ofxControlElement*> elementGroups;
    vector<ofxControlWidget*> attachedWidgets;
};


template<typename L, typename M>
ofxControlButton * ofxControlWidget::addButton(ofxControlParameter<bool> *parameter, L *listener, M method)
{
    ofxControlButton *button = addButton(parameter);
    ofAddListener(button->buttonEvent, listener, method);
    return button;
}

template<typename L, typename M>
ofxControlButton * ofxControlWidget::addButton(string name, bool *value, L *listener, M method)
{
    return addButton(new ofxControlParameter<bool>(name, value), listener, method);
}

template<typename L, typename M>
ofxControlButton * ofxControlWidget::addButton(string name, L *listener, M method)
{
    return addButton(new ofxControlParameter<bool>(name, new bool(false)), listener, method);
}

template<typename L, typename M>
ofxControlToggle * ofxControlWidget::addToggle(ofxControlParameter<bool> *parameter, L *listener, M method)
{
    ofxControlToggle *toggle = addToggle(parameter);
    ofAddListener(toggle->buttonEvent, listener, method);
    return toggle;
}

template<typename L, typename M>
ofxControlToggle * ofxControlWidget::addToggle(string name, bool *value, L *listener, M method)
{
    return addToggle(new ofxControlParameter<bool>(name, value), listener, method);
}

template<typename L, typename M>
ofxControlToggle * ofxControlWidget::addToggle(string name, L *listener, M method)
{
    return addToggle(new ofxControlParameter<bool>(name, new bool(false)), listener, method);
}

template<typename L, typename M>
ofxControlTextBox * ofxControlWidget::addTextBox(ofxControlParameter<string> *parameter, L *listener, M method)
{
    ofxControlTextBox *textBox = addTextBox(parameter);
    ofAddListener(textBox->textBoxEvent, listener, method);
    return textBox;
}

template<typename L, typename M>
ofxControlTextBox * ofxControlWidget::addTextBox(string name, string *value, L *listener, M method)
{
    return addTextBox(new ofxControlParameter<string>(name, value), listener, method);
}

template<typename L, typename M>
ofxControlTextBox * ofxControlWidget::addTextBox(string name, L *listener, M method)
{
    return addTextBox(new ofxControlParameter<string>(name, new string(name)), listener, method);
}

template<typename T>
ofxControlSlider<T> * ofxControlWidget::addSlider(ofxControlParameter<T> *parameter)
{
    ofxControlSlider<T> *slider = new ofxControlSlider<T>(parameter);
    initializeElement(slider);
    return slider;
}

template<typename T>
ofxControlSlider<T> * ofxControlWidget::addSlider(string name, T *value, T min, T max)
{
    return addSlider(new ofxControlParameter<T>(name, value, min, max));
}

template<typename T, typename L, typename M>
ofxControlSlider<T> * ofxControlWidget::addSlider(ofxControlParameter<T> *parameter, L *listener, M method)
{
    ofxControlSlider<T> *slider = addSlider(parameter);
    ofAddListener(slider->sliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
ofxControlSlider<T> * ofxControlWidget::addSlider(string name, T *value, T min, T max, L *listener, M method)
{
    return addSlider(new ofxControlParameter<T>(name, value, min, max), listener, method);
}

template<typename T>
ofxControlMultiSlider<T> * ofxControlWidget::addMultiSlider(ofxControlParameter<T> *parameter)
{
    ofxControlMultiSlider<T> *slider = new ofxControlMultiSlider<T>(parameter);
    initializeElement(slider);
    return slider;
}

template<typename T>
ofxControlMultiSlider<T> * ofxControlWidget::addMultiSlider(string name, T *value, T min, T max)
{
    return addMultiSlider(new ofxControlParameter<T>(name, value, min, max));
}

template<typename T, typename L, typename M>
ofxControlMultiSlider<T> * ofxControlWidget::addMultiSlider(ofxControlParameter<T> *parameter, L *listener, M method)
{
    ofxControlMultiSlider<T> *slider = addMultiSlider(parameter);
    ofAddListener(slider->sliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
ofxControlMultiSlider<T> * ofxControlWidget::addMultiSlider(string name, T *value, T min, T max, L *listener, M method)
{
    return addMultiSlider(new ofxControlParameter<T>(name, value, min, max), listener, method);
}

template<typename T>
ofxControlRangeSlider<T> * ofxControlWidget::addRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh)
{
    ofxControlRangeSlider<T> *slider = new ofxControlRangeSlider<T>(name, parameterLow, parameterHigh);
    initializeElement(slider);
    return slider;
}

template<typename T>
ofxControlRangeSlider<T> * ofxControlWidget::addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max)
{
    return addRangeSlider(name, new ofxControlParameter<T>(name+"-low", valueLow, min, max), new ofxControlParameter<T>(name+"-high", valueHigh, min, max));
}

template<typename T, typename L, typename M>
ofxControlRangeSlider<T> * ofxControlWidget::addRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh, L *listener, M method)
{
    ofxControlRangeSlider<T> *slider = addRangeSlider(name, parameterLow, parameterHigh);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
ofxControlRangeSlider<T> * ofxControlWidget::addRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method)
{
    return addRangeSlider(name, new ofxControlParameter<T>(name+"-low", valueLow, min, max), new ofxControlParameter<T>(name+"-high", valueHigh, min, max), listener, method);
}

template<typename T, typename L, typename M>
ofxControlRangeSlider<T> * ofxControlWidget::addRangeSlider(string name, T min, T max, L *listener, M method)
{
    ofxControlRangeSlider<T> *slider = addRangeSlider(name, new T(min), new T(max), min, max);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename T>
ofxControlMultiRangeSlider<T> * ofxControlWidget::addMultiRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh)
{
    ofxControlMultiRangeSlider<T> *slider = new ofxControlMultiRangeSlider<T>(parameterLow, parameterHigh);
    initializeElement(slider);
    return slider;
}

template<typename T>
ofxControlMultiRangeSlider<T> * ofxControlWidget::addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max)
{
    return addMultiRangeSlider(name, new ofxControlParameter<T>(name+"-low", valueLow, min, max), new ofxControlParameter<T>(name+"-high", valueHigh, min, max));
}

template<typename T, typename L, typename M>
ofxControlMultiRangeSlider<T> * ofxControlWidget::addMultiRangeSlider(string name, ofxControlParameter<T> *parameterLow, ofxControlParameter<T> *parameterHigh, L *listener, M method)
{
    ofxControlRangeSlider<T> *slider = addMultiRangeSlider(name, parameterLow, parameterHigh);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename T, typename L, typename M>
ofxControlMultiRangeSlider<T> * ofxControlWidget::addMultiRangeSlider(string name, T *valueLow, T *valueHigh, T min, T max, L *listener, M method)
{
    return addMultiRangeSlider(name, new ofxControlParameter<T>(name+"-low", valueLow, min, max), new ofxControlParameter<T>(name+"-high", valueHigh, min, max), listener, method);
}

template<typename T, typename L, typename M>
ofxControlMultiRangeSlider<T> * ofxControlWidget::addMultiRangeSlider(string name, T min, T max, L *listener, M method)
{
    ofxControlMultiRangeSlider<T> *slider = addMultiRangeSlider(name, min, max);
    ofAddListener(slider->rangeSliderEvent, listener, method);
    return slider;
}

template<typename L, typename M>
ofxControl2dPad * ofxControlWidget::add2dPad(ofxControlParameter<ofPoint> *parameter, L *listener, M method)
{
    ofxControl2dPad *pad = new ofxControl2dPad(parameter);
    ofAddListener(pad->padEvent, listener, method);
    return pad;
}

template<typename L, typename M>
ofxControl2dPad * ofxControlWidget::add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max, L *listener, M method)
{
    return add2dPad(new ofxControlParameter<ofPoint>(name, value, min, max), listener, method);
}

template<typename L, typename M>
ofxControl2dPad * ofxControlWidget::add2dPad(string name, ofPoint min, ofPoint max, L *listener, M method)
{
    return add2dPad(new ofxControlParameter<ofPoint>(name, new ofPoint(0.5 * (min + max)), min, max), listener, method);
}

template<typename L, typename M>
ofxControlColor * ofxControlWidget::addColor(ofxControlParameter<ofFloatColor> *parameter, L *listener, M method)
{
    ofxControlColor *color = addColor(parameter);
    ofAddListener(color->colorEvent, listener, method);
    return color;
}

template<typename L, typename M>
ofxControlColor * ofxControlWidget::addColor(string name, ofFloatColor *value, L *listener, M method)
{
    return addColor(new ofxControlParameter<ofFloatColor>(name, value, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)), listener, method);
}

template<typename L, typename M>
ofxControlMenu * ofxControlWidget::addMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice, bool autoClose)
{
    ofxControlMenu *menu = addMenu(name, choices, multipleChoice, autoClose);
    ofAddListener(menu->menuEvent, listener, method);
    return menu;
}

template<typename L, typename M>
ofxControlMenu * ofxControlWidget::addMenu(string name, L *listener, M method, bool multipleChoice, bool autoClose)
{
    vector<string> choices;
    return addMenu(name, choices, listener, method);
}
