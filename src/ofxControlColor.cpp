#include "ofxControlColor.h"


ofxControlColor::ofxControlColor(ofxControlParameter<ofFloatColor> *parameter) : ofxControlMultiElement(parameter->getName())
{
    this->parameter = parameter;
    setupColor();
}

ofxControlColor::ofxControlColor(string name, ofFloatColor *color) : ofxControlMultiElement(name)
{
    parameter = new ofxControlParameter<ofFloatColor>(name, color, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

ofxControlColor::ofxControlColor(string name) : ofxControlMultiElement(name)
{
    parameter = new ofxControlParameter<ofFloatColor>(name, new ofFloatColor(0.5, 1.0), ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1));
    setupColor();
}

ofxControlColor::~ofxControlColor()
{
    delete parameter;
}

void ofxControlColor::setupColor()
{
    marginY = GUI_DEFAULT_MARGIN_Y_INNER;
    
    initializeElement(new ofxControlSlider<float>(name+".r", &parameter->getReference()->r, parameter->getMin().r, parameter->getMax().r));
    initializeElement(new ofxControlSlider<float>(name+".g", &parameter->getReference()->g, parameter->getMin().g, parameter->getMax().g));
    initializeElement(new ofxControlSlider<float>(name+".b", &parameter->getReference()->b, parameter->getMin().b, parameter->getMax().b));
    initializeElement(new ofxControlSlider<float>(name+".a", &parameter->getReference()->a, parameter->getMin().a, parameter->getMax().a));
    for (auto e : elements) {
        ofAddListener(((ofxControlSlider<float> *) e)->sliderEvent, this, &ofxControlColor::sliderChanged);
    }
    
    setCollapsible(true);
    setHeader(getName());
    updateParameterOscAddress();
}

void ofxControlColor::sliderChanged(ofxControlSliderEventArgs<float> &e)
{
    ofxControlColorEventArgs args(this, parameter->get());
    ofNotifyEvent(colorEvent, args, this);
}

void ofxControlColor::update()
{
    ofxControlMultiElement::update();
    ofColor currentColor = parameter->get();
    ofColor textColor = currentColor.r + currentColor.g + currentColor.b > 385 ? GUI_DEFAULT_COLOR_TEXT_DARK : GUI_DEFAULT_COLOR_TEXT_LIGHT;
    elements[0]->setColorForeground(ofColor(currentColor, 255));
    elements[1]->setColorForeground(ofColor(currentColor, 255));
    elements[2]->setColorForeground(ofColor(currentColor, 255));
    elements[3]->setColorForeground(currentColor);
    elements[0]->setColorText(textColor);
    elements[1]->setColorText(textColor);
    elements[2]->setColorText(textColor);
    elements[3]->setColorText(textColor);
}

void ofxControlColor::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}

void ofxControlColor::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

