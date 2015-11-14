#include "ofxControlMultiElement.h"



ofxControlMultiElement::ofxControlMultiElement(string name) : ofxControlElement(name)
{
    collapsed = false;
    hasParent = false;
    parent = NULL;

    headerHeight = GUI_DEFAULT_HEADER_HEIGHT;
    headerColor = GUI_DEFAULT_HEADER_COLOR;
    controllerHeight = GUI_DEFAULT_CONTROLLER_HEIGHT;
    marginX = GUI_DEFAULT_MARGIN_X;
    marginY = GUI_DEFAULT_MARGIN_Y;

    setHeader(getName());
    setCollapsible(false);
    setCollapsed(false);
}

ofxControlMultiElement::~ofxControlMultiElement()
{
    clearElements();
}

ofxControlElement * ofxControlMultiElement::getElement(string name)
{
    vector<ofxControlElement*>::iterator it = elements.begin();
    for (; it != elements.end(); ++it)
    {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    ofLog(OF_LOG_ERROR, "Error: no element named "+name+" found in "+getName());
    return NULL;
}

bool ofxControlMultiElement::removeElement(string name)
{
    vector<ofxControlElement*>::iterator it = elements.begin();
    while (it != elements.end())
    {
        if ((*it)->getName() == name)
        {
            ofNotifyEvent(removeElementEvent, *it, this);
            delete *it;
            elements.erase(it);
            setupGuiPositions();
            return true;
        }
        else
        {
            if ((*it)->isMultiElement())
            {
                if (((ofxControlMultiElement *)(*it))->removeElement(name))
                {
                    setupGuiPositions();
                    return true;
                }
            }
            ++it;
        }
    }
    return false;
}

void ofxControlMultiElement::clearElements()
{
    vector<ofxControlElement*>::iterator it = elements.begin();
    while (it != elements.end())
    {
        ofNotifyEvent(removeElementEvent, *it, this);
        delete *it;
        elements.erase(it);
    }
    elements.clear();
}

void ofxControlMultiElement::getParameters(vector<ofxControlParameterBase*> & parameters_)
{
    for (auto e : elements) {
        e->getParameters(parameters_);
    }
}

void ofxControlMultiElement::setHeader(string header)
{
    this->header = header;
    ofBitmapFont b;
    headerStringHeight = b.getBoundingBox(header, 0, 0).height;
}

void ofxControlMultiElement::updateParameterOscAddress()
{
    for (auto e : elements) {
        e->updateParameterOscAddress();
    }
}

void ofxControlMultiElement::setupGuiPositions()
{
    ofPoint topLeft = ofPoint(rectangle.x, rectangle.y);
    if (collapsible && getActive())
    {
        headerRectangle.set(x, y, width, headerHeight);
        topLeft.y += headerHeight;
    }
    else {
        headerRectangle.set(0, 0, 0, 0);
    }

    if (!getCollapsed() && getActive())
    {
        topLeft.y += controllerHeight;
        topLeft.y += collapsible ? marginY : 0;
    
        for (auto e : elements)
        {
            if (e->getActive())
            {
                e->setPosition(topLeft.x + (collapsible ? marginX : 0), topLeft.y);
                e->setWidth(getWidth() - (collapsible ? 2 * marginX : 0));
                topLeft.y += e->getRectangle().height + marginY;
            }
            else
            {
                e->setRectangle(0, 0, 0, 0);
            }
        }
    }
    rectangle.height = topLeft.y - rectangle.y;
}

void ofxControlMultiElement::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{
    for (auto e : elements) {
        addElementToTouchOscLayout(page, y);
    }
}

void ofxControlMultiElement::initializeElement(ofxControlElement *element, bool sendNotification)
{
    elements.push_back(element);
    element->setAutoUpdate(false);
    element->setAutoDraw(false);
    element->setParent(this);
    resetGuiPositions();
    if (sendNotification) {
        ofNotifyEvent(newElementEvent, element, this);
    }
}

void ofxControlMultiElement::setCollapsible(bool collapsible)
{
    this->collapsible = collapsible;
    setupGuiPositions();
}

void ofxControlMultiElement::setCollapsed(bool collapsed)
{
    this->collapsed = collapsed;
    resetGuiPositions();
}

bool ofxControlMultiElement::getCollapsed()
{
    return hasParent ? collapsed || parent->getCollapsed() : collapsed;
}

void ofxControlMultiElement::update()
{
    for (auto e : elements)
    {
        e->update();
    }
}

void ofxControlMultiElement::draw()
{
    ofPushStyle();
    
    ofSetLineWidth(1);
    ofSetCircleResolution(16);
    
    ofSetColor(colorBackground, mouseOver ? 110 : 80);
    ofFill();
    ofRect(rectangle);
    
    if (collapsible)
    {
        ofFill();
        ofSetColor(headerColor);
        ofRect(headerRectangle);
        if (headerActive)
        {
            ofSetColor(colorActive);
            ofSetLineWidth(2);
            ofNoFill();
            ofRect(headerRectangle);
        }
        ofSetColor(colorText);
        ofDrawBitmapString(name,
                           rectangle.x + 4,
                           rectangle.y + 1 + 0.5 * (headerHeight + 0.5 * headerStringHeight));
        ofDrawBitmapString(collapsed ? "+" : "-",
                           rectangle.x + rectangle.width - 16,
                           rectangle.y + 1 + 0.5 * (headerHeight + 0.5 * headerStringHeight));
    }
    
    if (!getCollapsed())
    {
        for (auto e : elements)
        {
            if (e->getActive()) {
                e->draw();
            }
        }
    }

    ofPopStyle();
}

bool ofxControlMultiElement::mouseMoved(int mouseX, int mouseY)
{
    ofxControlElement::mouseMoved(mouseX, mouseY);
    headerActive = headerRectangle.inside(mouseX, mouseY);
    if (!getCollapsed())
    {
        for (auto e : elements) {
            e->mouseMoved(mouseX, mouseY);
        }
    }
    return false;
}

bool ofxControlMultiElement::mousePressed(int mouseX, int mouseY)
{
    ofxControlElement::mousePressed(mouseX, mouseY);
    if (mouseOver && !getCollapsed())
    {
        for (auto e : elements) {
            if (e->mousePressed(mouseX, mouseY))  return true;
        }
    }
    return false;
}

bool ofxControlMultiElement::mouseDragged(int mouseX, int mouseY)
{
    if (!hasParent && (headerActive || draggingWidget))
    {
        setPosition(x + ofGetMouseX() - ofGetPreviousMouseX(), y + ofGetMouseY() - ofGetPreviousMouseY());
        draggingWidget = true;
        return true;
    }
    else if (mouseOver && !getCollapsed())
    {
        for (auto e : elements) {
            if (e->mouseDragged(mouseX, mouseY))  return true;
        }
    }
    return false;
}

bool ofxControlMultiElement::mouseReleased(int mouseX, int mouseY)
{
    if (headerActive && !draggingWidget)
    {
        draggingWidget = false;
        setCollapsed(!collapsed);
        return true;
    }
    else if (mouseOver && !getCollapsed())
    {
        draggingWidget = false;
        for (auto e : elements) {
            if (e->mouseReleased(mouseX, mouseY))  return true;
        }
    }
    draggingWidget = false;
    return false;
}

bool ofxControlMultiElement::keyPressed(int key)
{
    ofxControlElement::keyPressed(key);
    if (mouseOver)
    {
        for (auto e : elements) {
            if (e->keyPressed(key)) return true;
        }
        return true;
    }
    return false;
}

void ofxControlMultiElement::getXml(ofXml &xml)
{
    for (auto e : elements)
    {
        ofXml xmlElement;
        xmlElement.addChild("Element");
        xmlElement.setTo("Element");
        xmlElement.addValue("Address", e->getAddress());
        e->getXml(xmlElement);
        xml.addXml(xmlElement);
    }
}

void ofxControlMultiElement::setFromXml(ofXml &xml)
{
    if (!xml.exists("Element[0]"))
    {
        ofLog(OF_LOG_ERROR, "No elements found in preset");
        return;
    }
    
    if (xml.exists("Element[0]"))
    {
        xml.setTo("Element[0]");
        do {
            string name = xml.getValue<string>("Address");
            for (auto e : elements)
            {
                if (e->getAddress() == name) {
                    e->setFromXml(xml);
                }
            }
        }
        while(xml.setToSibling());
        xml.setToParent();
    }
}
