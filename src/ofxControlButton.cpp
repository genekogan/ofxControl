#include "ofxControlButton.h"

ofxControlButtonBase::ofxControlButtonBase(ofxControlParameter<bool> *parameter) : ofxControlElement(parameter->getName())
{
    this->parameter = parameter;
    initializeButton();
}

ofxControlButtonBase::ofxControlButtonBase(string name, bool *value) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<bool>(name, value);
    initializeButton();
}

ofxControlButtonBase::ofxControlButtonBase(string name) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<bool>(name, new bool());
    initializeButton();
}

ofxControlButtonBase::~ofxControlButtonBase()
{
    delete parameter;
}

void ofxControlButtonBase::initializeButton()
{
    setValue(parameter->get());
    lerpFrame = 0;
    lerpNumFrames = 0;
    lerpNextValue = 0.0;
    setLeftJustified(false);
    changed = false;
}

void ofxControlButtonBase::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

void ofxControlButtonBase::setValue(bool value, bool sendChangeNotification)
{
    bool previous = parameter->get();
    changed = (value != previous);
    parameter->set(value);
    if (sendChangeNotification && changed)
    {
        ofxControlButtonEventArgs args(this, value);
        ofNotifyEvent(buttonEvent, args, this);
    }
}

void ofxControlButtonBase::lerpTo(float nextValue, int numFrames)
{
    this->lerpNextValue = nextValue;
    this->lerpNumFrames = numFrames;
    lerpFrame = 0;
}

void ofxControlButtonBase::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}

void ofxControlButtonBase::setValueFromSequence(ofxControlSequence &sequence)
{
    setValue(sequence.getValueAtCurrentIndex() > 0.5, true);
}

void ofxControlButtonBase::setSequenceFromValue(ofxControlSequence &sequence, int column)
{
    sequence.setValueAtCell(column, parameter->get() > 0.5);
}

void ofxControlButtonBase::setSequenceFromExplicitValue(ofxControlSequence &sequence, int column, float value)
{
    sequence.setValueAtCell(column, value > 0.5);
}

bool ofxControlButtonBase::getValue()
{
    return parameter->get();
}

void ofxControlButtonBase::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        lerpFrame++;
        if (lerpFrame == lerpNumFrames) {
            setValue(lerpNextValue > 0.5, true);
        }
    }
}

void ofxControlButtonBase::draw()
{
	ofPushStyle();
    
    ofFill();
    ofSetLineWidth(1);
    parameter->get() ? ofSetColor(colorForeground) : ofSetColor(colorBackground);
    ofRect(rectangle);
    
    ofNoFill();
    ofSetColor(colorOutline);
    ofRect(rectangle);
    
	if (mouseOver)
	{
	    ofNoFill();
	    ofSetLineWidth(2);
	    ofSetColor(colorActive);
	    ofRect(rectangle);
        ofSetLineWidth(1);
	}
    
    ofSetColor(colorText);
    ofDrawBitmapString(display,
                       rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)),
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);

	ofPopStyle();
}

void ofxControlButtonBase::setupGuiPositions()
{
    ofxControlElement::setupGuiPositions();
//    stringWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
//    stringHeight = ofBitmapStringGetBoundingBox(display, 0, 0).height;
    ofBitmapFont b;
    ofRectangle bb = b.getBoundingBox(display, 0, 0);
    stringWidth = bb.width;
    stringHeight = bb.height;
}

void ofxControlButtonBase::getXml(ofXml &xml)
{
    xml.addValue<bool>("Value", getValue());
}

void ofxControlButtonBase::setFromXml(ofXml &xml)
{
    setValue(xml.getValue<bool>("Value"), true);
}

void ofxControlButton::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{
    ofxControlTouchOscButton *button = page->addButton(getName(), 0.05, *y, 0.9, 1);
    ofxControlTouchOscLabel *label = page->addLabel(getName(), 0.05, *y, 0.9, 1);
    button->setOscAddress(parameter->getOscAddress());
    label->setType(0);
    label->setColor(RED);
    *y += 1.04;
}

void ofxControlToggle::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{
    ofxControlTouchOscToggle *toggle = page->addToggle(getName(), 0.05, *y, 0.9, 1);
    ofxControlTouchOscLabel *label = page->addLabel(getName(), 0.05, *y, 0.9, 1);
    toggle->setOscAddress(parameter->getOscAddress());
    label->setType(0);
    label->setColor(RED);
    *y += 1.05;
}

void ofxControlButtonBase::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{

}

string ofxControlButtonBase::getOscAddress()
{
    return parameter->getOscAddress();
}

void ofxControlButtonBase::sendOsc(ofxOscMessage &msg)
{
    msg.addIntArg(parameter->get() ? 1 : 0);
}

void ofxControlButtonBase::receiveOsc(ofxOscMessage &msg)
{
    setValue(msg.getArgAsInt32(0) > 0.5);
}

bool ofxControlButtonBase::valueChanged()
{
    if (changed) {
        changed = false;
        return true;
    }
    else {
        return false;
    }
}

bool ofxControlButton::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        setValue(true, true);
        return true;
    }
    return false;
}

bool ofxControlButton::mouseReleased(int mouseX, int mouseY)
{
    ofxControlElement::mouseReleased(mouseX, mouseY);
    if (getValue())
    {
        setValue(false, false);
        return true;
    }
    else {
        return false;
    }
}

bool ofxControlToggle::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        bool value = !parameter->get();
        setValue(value, true);
        return true;
    }
    else {
        return false;
    }
}

bool ofxControlButton::keyPressed(int key)
{
    if (mouseOver && key==' ')
    {
        setValue(true, true);
        setValue(false, false);
        return true;
    }
    else {
        return false;
    }
}

bool ofxControlToggle::keyPressed(int key)
{
    if (mouseOver && key==' ')
    {
        bool value = !parameter->get();
        setValue(value, true);
        return true;
    }
    else {
        return false;
    }
}

