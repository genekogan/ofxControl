#include "ofxControlOscManagerPanel.h"



ofxControlOscManagerPanel::ElementOsc::ElementOsc(ofxControlElement *element, ofxControlOscManager *osc)
{
    this->element = element;
    this->osc = osc;
    address = element->getAddress();
    ofStringReplace(address, " ", "_");
    tAddress = new ofxControlTextBox("address", new string(address), this, &ElementOsc::eventSetAddress);
    tAddress->setAutoUpdate(false);
    tAddress->setAutoDraw(false);
    tSend = new ofxControlToggle("s", &sending, this, &ElementOsc::eventSetSending);
    tSend->setAutoUpdate(false);
    tSend->setAutoDraw(false);
}

void ofxControlOscManagerPanel::ElementOsc::updateAddress()
{
    vector<ofxControlParameterBase*> parameters;
    element->getParameters(parameters);
    for (auto p : parameters) {
        p->setOscAddress(address);
    }
}

void ofxControlOscManagerPanel::ElementOsc::eventSetAddress(ofxControlTextBoxEventArgs &evt)
{
    address = evt.textBox->getValue();
    ofStringReplace(address, " ", "_");
    tAddress->setValue(address, false);
    if (sending) {
        updateAddress();
    }
}

void ofxControlOscManagerPanel::ElementOsc::eventSetSending(ofxControlButtonEventArgs &evt)
{
    updateAddress();
    if (sending) {
        osc->addElementToSender(element);
    }
    else {
        osc->removeElementFromSender(element);
    }
    
}

ofxControlOscManagerPanel::ElementOscGroup::ElementOscGroup(ofxControlElement *element, ofxControlOscManager *osc)
{
    this->element = element;
    this->osc = osc;
    
    if (element->isMultiElement())
    {
        for (auto e : ((ofxControlMultiElement *) element)->getElements()) {
            elements.push_back(new ElementOsc(e, osc));
        }
    }
    else {
        elements.push_back(new ElementOsc(element, osc));
    }
}

ofxControlOscManagerPanel::ofxControlOscManagerPanel(string name, ofxControlWidget * panel, ofxControlOscManager *osc) : ofxControlElement(name)
{
    this->panel = panel;
    this->osc = osc;
    this->ofxControlManagerWidth = GUI_DEFAULT_OSCMANAGER_WIDTH;
    
    tTouchOsc = new ofxControlButton("touchOsc", this, &ofxControlOscManagerPanel::eventMakeTouchOscLayout);
    tTouchOsc->setAutoUpdate(false);
    tTouchOsc->setAutoDraw(false);

    ofAddListener(panel->newElementEvent, this, &ofxControlOscManagerPanel::addElement);
    ofAddListener(panel->removeElementEvent, this, &ofxControlOscManagerPanel::removeElement);
    
    setupofxControlOscManagerPanel();
    
    ofxControlElement::setAutoUpdate(true);
    ofxControlElement::setAutoDraw(true);
}

ofxControlOscManagerPanel::~ofxControlOscManagerPanel()
{
    ofRemoveListener(panel->newElementEvent, this, &ofxControlOscManagerPanel::addElement);
    ofRemoveListener(panel->removeElementEvent, this, &ofxControlOscManagerPanel::removeElement);
}

void ofxControlOscManagerPanel::setupofxControlOscManagerPanel()
{
    for (auto element : panel->getElementGroups()) {
        addElement(element);
    }
}

void ofxControlOscManagerPanel::addElement(ofxControlElement* & newElement)
{
    if (groups.count(newElement) == 0)
    {
        groups[newElement] = new ElementOscGroup(newElement, osc);
        setupGuiPositions();
    }
}

void ofxControlOscManagerPanel::removeElement(ofxControlElement* & element)
{
    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    while (it != groups.end())
    {
        if (it->first == element ||
            (it->second->element->getHasParent() && (it->second->element->getParent() == element)))
        {
            delete it->second;
            groups.erase(it++);
            setupGuiPositions();
        }
        else
        {
            vector<ElementOsc*>::iterator ite = it->second->elements.begin();
            while (ite != it->second->elements.end())
            {
                if ((*ite)->element == element)
                {
                    delete *ite;
                    it->second->elements.erase(ite);
                    setupGuiPositions();
                }
                else
                {
                    ++ite;
                }
            }
            ++it;
        }
    }
}

void ofxControlOscManagerPanel::setupGuiPositions()
{
    ofRectangle panelRect = panel->getRectangle();
    rectangle.set(panelRect.x + panelRect.getWidth() + 5, panelRect.y, ofxControlManagerWidth, panelRect.getHeight());
    
    tTouchOsc->setPosition(rectangle.x + 5, rectangle.y + 15);
    tTouchOsc->setSize(100, 14);
    
    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getCollapsed() || getCollapsed())
        {
            for (auto e : it->second->elements)
            {
                e->tAddress->setRectangle(ofRectangle(0, 0, 0, 0));
                e->tSend->setRectangle(ofRectangle(0, 0, 0, 0));
            }
        }
        else
        {
            for (auto e : it->second->elements)
            {
                e->tAddress->setPosition(rectangle.x, e->element->getRectangle().y);
                e->tAddress->setSize(ofxControlManagerWidth - 20, e->element->getRectangle().height);
                e->tSend->setPosition(rectangle.x + ofxControlManagerWidth - 16, e->element->getRectangle().y);
                e->tSend->setSize(12, e->element->getRectangle().height);
            }
        }
    }
}

void ofxControlOscManagerPanel::eventMakeTouchOscLayout(ofxControlButtonEventArgs &evt)
{
    panel->makeTouchOscLayout("OSC "+panel->getName());
}

void ofxControlOscManagerPanel::update()
{
    tTouchOsc->update();
    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        for (auto e : it->second->elements)
        {
            e->tAddress->update();
            e->tSend->update();
        }
    }
}

void ofxControlOscManagerPanel::draw()
{
    ofSetColor(colorBackground, 50);
    ofFill();
    ofRect(rectangle);
    
    tTouchOsc->draw();

    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            for (auto e : it->second->elements)
            {
                e->tAddress->draw();
                e->tSend->draw();
            }
        }
    }
}

bool ofxControlOscManagerPanel::mouseMoved(int mouseX, int mouseY)
{
    ofxControlElement::mouseMoved(mouseX, mouseY);
    
    tTouchOsc->mouseMoved(mouseX, mouseY);
    
    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            for (auto e : it->second->elements)
            {
                e->tAddress->mouseMoved(mouseX, mouseY);
                e->tSend->mouseMoved(mouseX, mouseY);
            }
        }
    }
    return mouseOver;
}

bool ofxControlOscManagerPanel::mousePressed(int mouseX, int mouseY)
{
    if (tTouchOsc->mousePressed(mouseX, mouseY)) return true;
    
    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            for (auto e : it->second->elements)
            {
                if      (e->tAddress->mousePressed(mouseX, mouseY)) return true;
                else if (e->tSend->mousePressed(mouseX, mouseY)) return true;
            }
        }
    }
    
    return false;
}

bool ofxControlOscManagerPanel::mouseDragged(int mouseX, int mouseY)
{
    if (tTouchOsc->mouseDragged(mouseX, mouseY)) return true;

    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            for (auto e : it->second->elements)
            {
                if      (e->tAddress->mouseDragged(mouseX, mouseY)) return true;
                else if (e->tSend->mouseDragged(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool ofxControlOscManagerPanel::mouseReleased(int mouseX, int mouseY)
{
    if (tTouchOsc->mouseReleased(mouseX, mouseY)) return true;

    map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
    for (; it != groups.end(); ++it)
    {
        if (it->second->element->getActive())
        {
            for (auto e : it->second->elements)
            {
                if      (e->tAddress->mouseReleased(mouseX, mouseY)) return true;
                else if (e->tSend->mouseReleased(mouseX, mouseY)) return true;
            }
        }
    }
    return false;
}

bool ofxControlOscManagerPanel::keyPressed(int key)
{
    if (mouseOver)
    {
        if (tTouchOsc->keyPressed(key)) return true;

        map<ofxControlElement*,ElementOscGroup*>::iterator it = groups.begin();
        for (; it != groups.end(); ++it)
        {
            if (it->second->element->getActive())
            {
                for (auto e : it->second->elements)
                {
                    if      (e->tAddress->keyPressed(key)) return true;
                    else if (e->tSend->keyPressed(key)) return true;
                }
            }
        }
    }
    return false;
}

