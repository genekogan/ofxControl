#include "ofxControlWidget.h"


ofxControlWidget::ofxControlWidget(string name) : ofxControlMultiElement(name)
{
    
}

ofxControlWidget::ofxControlWidget() : ofxControlMultiElement("Widget")
{
    
}

void ofxControlWidget::initializeElement(ofxControlElement *element, bool sendNotification)
{
    ofxControlMultiElement::initializeElement(element, sendNotification);
    if (sendNotification) {
        elementGroups.push_back(element);
    }
    if (element->isMultiElement())
    {
        ofAddListener(((ofxControlMultiElement *) element)->newElementEvent, this, &ofxControlWidget::eventAddElement);
        ofAddListener(((ofxControlMultiElement *) element)->removeElementEvent, this, &ofxControlWidget::eventRemoveElement);
    }
    if (elements.size() > 1) {
        setCollapsible(true);
    }
}

void ofxControlWidget::eventAddElement(ofxControlElement * &element)
{
    ofNotifyEvent(newElementEvent, element, this);
}

void ofxControlWidget::eventRemoveElement(ofxControlElement * &element)
{
    ofNotifyEvent(removeElementEvent, element, this);
}

ofxControlWidget * ofxControlWidget::addWidget(ofxControlWidget *newWidget)
{
    initializeElement(newWidget, false);
    for (auto e : newWidget->getElementGroups()) {
        widgetNewElementAdded(e);
    }
    ofAddListener(newWidget->newElementEvent, this, &ofxControlWidget::widgetNewElementAdded);
    return newWidget;
}

ofxControlWidget * ofxControlWidget::addWidget(string widgetName)
{
    return addWidget(new ofxControlWidget(widgetName));
}

ofxControlButton * ofxControlWidget::addButton(ofxControlParameter<bool> *parameter)
{
    ofxControlButton *button = new ofxControlButton(parameter);
    initializeElement(button);
    return button;
}

ofxControlButton * ofxControlWidget::addButton(string name, bool *value)
{
    return addButton(new ofxControlParameter<bool>(name, value));
}

ofxControlToggle * ofxControlWidget::addToggle(ofxControlParameter<bool> *parameter)
{
    ofxControlToggle *toggle = new ofxControlToggle(parameter);
    initializeElement(toggle);
    return toggle;
}

ofxControlToggle * ofxControlWidget::addToggle(string name, bool *value)
{
    return addToggle(new ofxControlParameter<bool>(name, value));
}

ofxControlTextBox * ofxControlWidget::addTextBox(ofxControlParameter<string> *parameter)
{
    ofxControlTextBox *textBox = new ofxControlTextBox(parameter);
    initializeElement(textBox);
    return textBox;
}

ofxControlTextBox * ofxControlWidget::addTextBox(string name, string *value)
{
    return addTextBox(new ofxControlParameter<string>(name, value));
}

ofxControl2dPad * ofxControlWidget::add2dPad(ofxControlParameter<ofPoint> *parameter)
{
    ofxControl2dPad *pad = new ofxControl2dPad(parameter);
    initializeElement(pad);
    return pad;
}

ofxControl2dPad * ofxControlWidget::add2dPad(string name, ofPoint *value, ofPoint min, ofPoint max)
{
    return add2dPad(new ofxControlParameter<ofPoint>(name, value, min, max));
}

ofxControl2dPad * ofxControlWidget::add2dPad(string name, ofPoint min, ofPoint max)
{
    ofxControl2dPad *pad = new ofxControl2dPad(name, min, max);
    initializeElement(pad);
    return pad;
}

ofxControlColor * ofxControlWidget::addColor(ofxControlParameter<ofFloatColor> *parameter)
{
    ofxControlColor *color = new ofxControlColor(parameter);
    initializeElement(color);
    return color;
}

ofxControlColor * ofxControlWidget::addColor(string name, ofFloatColor *value)
{
    return addColor(new ofxControlParameter<ofFloatColor>(name, value, ofFloatColor(0, 0, 0, 0), ofFloatColor(1, 1, 1, 1)));
}

ofxControlMenu * ofxControlWidget::addMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose)
{
    ofxControlMenu *menu = new ofxControlMenu(name, choices, multipleChoice, autoClose);
    initializeElement(menu);
    return menu;
}

ofxControlMenu * ofxControlWidget::addMenu(string name, bool multipleChoice, bool autoClose)
{
    vector<string> choices;
    return addMenu(name, choices, multipleChoice, autoClose);
}

void ofxControlWidget::widgetNewElementAdded(ofxControlElement* & newElement)
{
    elementGroups.push_back(newElement);
    ofNotifyEvent(newElementEvent, newElement, this);
}

void ofxControlWidget::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    for (auto e : elementGroups) {
        e->getParameters(parameters);
    }
}

void ofxControlWidget::updateParameterOscAddress()
{
    for (auto e : elementGroups) {
        e->updateParameterOscAddress();
    }
}

void ofxControlWidget::setupGuiPositions()
{
    ofxControlMultiElement::setupGuiPositions();
    for (auto w : attachedWidgets) {
        w->setPosition(rectangle.x + rectangle.width + marginX, rectangle.y);
    }
}

void ofxControlWidget::attachWidget(ofxControlWidget *other)
{
    attachedWidgets.push_back(other);
    other->setPosition(rectangle.x + rectangle.width + marginX, rectangle.y);
}

void ofxControlWidget::detachWidget(ofxControlWidget *other)
{
    vector<ofxControlWidget*>::iterator it = attachedWidgets.begin();
    while (it != attachedWidgets.end())
    {
        if (*it == other) {
            attachedWidgets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ofxControlWidget::makeTouchOscLayout(string filename)
{
    ofxControlTouchOsc touchOsc;
    touchOsc.setScale(320, 560);
    touchOsc.setDefaultColor(GREEN);
    
    ofxControlTouchOscPage *page1 = touchOsc.addPage("widget");
    float y = 0;
    for (auto e : elements) {
        addElementToTouchOscLayout(e, page1, &y);
    }
    
    for (auto w : page1->getWidgets())
    {
        w->y = ofMap(w->y, 0, y, 0, 0.9);
        w->h = 0.9 * w->h / y;
    }
    touchOsc.save(filename);
}

void ofxControlWidget::addElementToTouchOscLayout(ofxControlElement *element, ofxControlTouchOscPage *page, float *y)
{
    if (element->isMultiElement())
    {
        for (auto e : ((ofxControlMultiElement *) element)->getElements()) {
            addElementToTouchOscLayout(e, page, y);
        }
    }
    else
    {
        element->updateParameterOscAddress();
        element->addElementToTouchOscLayout(page, y);
    }
}

void ofxControlWidget::getXml(ofXml &xml)
{
    xml.addChild("Elements");
    xml.setTo("Elements");
    for (auto e : elements)
    {
        ofXml xmlElement;
        xmlElement.addChild("Element");
        xmlElement.setTo("Element");
        xmlElement.addValue("Address", e->getAddress());
        e->getXml(xmlElement);
        xml.addXml(xmlElement);
    }
    xml.setToParent();
}

void ofxControlWidget::setFromXml(ofXml &xml)
{
    if (!xml.exists("Elements"))
    {
        ofLog(OF_LOG_ERROR, "No elements found in preset");
        return;
    }
    
    xml.setTo("Elements");
    if (xml.exists("Element[0]"))
    {
        xml.setTo("Element[0]");
        do {
            string address = xml.getValue<string>("Address");
            for (auto e : elements)
            {
                if (e->getAddress() == address) {
                    e->setFromXml(xml);
                }
            }
        }
        while(xml.setToSibling());
        xml.setToParent();
    }
    xml.setToParent();
}
