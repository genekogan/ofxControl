#include "ofxControlElement.h"


ofxControlElement::ofxControlElement(string name) : ofxControlBase(name)
{
    hasParent = false;
    parent = NULL;
    display = "";
}

ofxControlElement::ofxControlElement() : ofxControlBase()
{
    hasParent = false;
    parent = NULL;
    display = "";
}

ofxControlElement::~ofxControlElement()
{

}

void ofxControlElement::setParent(ofxControlElement *parent)
{
    this->parent = parent;
    hasParent = true;
    display = "";
}

string ofxControlElement::getAddress()
{
    return hasParent ? parent->getAddress() + "/" + getName() : getName();
}

bool ofxControlElement::getActive()
{
    return hasParent ? parent->getActive() && active : active;
}

void ofxControlElement::setActive(bool active)
{
    ofxControlBase::setActive(active);
    resetGuiPositions();
}

bool ofxControlElement::getCollapsed()
{
    return hasParent ? parent->getCollapsed() : false;
}

void ofxControlElement::setMouseOver(bool mouseOver)
{
    ofxControlBase::setMouseOver(mouseOver);
    if (mouseOver && hasParent) {
        parent->setMouseOver(mouseOver);
    }
}

void ofxControlElement::setupGuiPositions()
{
    if (getCollapsed()){
        rectangle.set(0, 0, 0, 0);
    }
    else
    {
        rectangle.set(x, y, width, height);
        setupDisplayString();
    }
}

void ofxControlElement::resetGuiPositions()
{
    if (hasParent) {
        parent->resetGuiPositions();
    }
    else {
        setupGuiPositions();
    }
}

void ofxControlElement::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{

}

void ofxControlElement::updateParameterOscAddress()
{
    
}

void ofxControlElement::getXml(ofXml &xml)
{

}

void ofxControlElement::setFromXml(ofXml &xml)
{
    
}

void ofxControlElement::setupDisplayString()
{
    display = name;
    
    ofBitmapFont b;
    int displayWidth = b.getBoundingBox(display, 0, 0).width;
    while (displayWidth > getWidth())
    {
        display = display.substr(0, display.length()-1);
        displayWidth = b.getBoundingBox(display, 0, 0).width;
    }
    
    
    /*
    int displayWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
    while (displayWidth > getWidth())
    {
        display = display.substr(0, display.length()-1);
        displayWidth = ofBitmapStringGetBoundingBox(display, 0, 0).width;
    }
     */
}
