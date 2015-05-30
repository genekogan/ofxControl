#include "ofxControlMenu.h"


ofxControlMenu::MenuElement::MenuElement(ofxControlToggle *toggle, ofxControlParameter<bool> *parameter, int index)
{
    this->toggle = toggle;
    this->parameter = parameter;
    this->index = index;
}

ofxControlMenu::ofxControlMenu(string name, vector<string> choices, bool multipleChoice, bool autoClose) : ofxControlMultiElement(name)
{
    setupMenu(name, choices, multipleChoice, autoClose);
}

ofxControlMenu::ofxControlMenu(string name, bool multipleChoice, bool autoClose) : ofxControlMultiElement(name)
{
    vector<string> choices;
    setupMenu(name, choices, multipleChoice, autoClose);
}

ofxControlMenu::~ofxControlMenu()
{
    clearToggles();
}

void ofxControlMenu::setupMenu(string name, vector<string> & choices, bool multipleChoice, bool autoClose)
{
    marginY = GUI_DEFAULT_MARGIN_Y_INNER;
    setName(name);
    setMultipleChoice(multipleChoice);
    setAutoClose(autoClose);
    setCollapsible(true);
    setCollapsed(false);
    for (auto choice : choices) {
        addToggle(choice);
    }
}

ofxControlToggle * ofxControlMenu::addToggle(ofxControlParameter<bool> *parameter)
{
    int idx = 1;
    string newToggleName = parameter->getName();
    while (menuElements.count(newToggleName) != 0) {
        newToggleName = parameter->getName()+"("+ofToString(++idx)+")";
    }
    parameter->setName(newToggleName);
    
    if (menuElements.count(parameter->getName()) == 0)
    {
        parameters.push_back(parameter);
        ofxControlToggle *toggle = new ofxControlToggle(parameter);
        menuElements[parameter->getName()] = new MenuElement(toggle, parameter, elements.size());
        initializeElement(toggle);
        ofAddListener(toggle->buttonEvent, this, &ofxControlMenu::buttonChanged);
        return toggle;
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Note: toggle "+parameter->getName()+" already exists");
        delete parameter;
        return NULL;
    }
}

ofxControlToggle * ofxControlMenu::addToggle(string name, bool *value)
{
    return addToggle(new ofxControlParameter<bool>(name, value));
}

ofxControlToggle * ofxControlMenu::addToggle(string name)
{
    return addToggle(new ofxControlParameter<bool>(name, new bool(false)));
}

void ofxControlMenu::setToggle(string toggleName, bool value)
{
    if (!multipleChoice)
    {
        for (auto e : elements) {
            ((ofxControlToggle *) e)->setValue(e->getName() == toggleName, false);
        }
    }
    if (autoClose)
    {
        setCollapsed(true);
        setHeader(toggleName);
    }
    if (menuElements.count(toggleName))
    {
        ofxControlMenuEventArgs evt(menuElements[toggleName]->toggle, menuElements[toggleName]->index, value);
        ofNotifyEvent(menuEvent, evt, this);
    }
}

bool ofxControlMenu::getToggle(string toggleName)
{
    if (menuElements.count(toggleName) == 0) {
        return menuElements[toggleName]->toggle->getValue();
    }
    else
    {
        ofLog(OF_LOG_ERROR, "Error: no toggle named "+toggleName);
        return false;
    }
}

void ofxControlMenu::removeToggle(string toggleName)
{
    if (menuElements.count(toggleName) != 0)
    {
        vector<ofxControlElement*>::iterator it = elements.begin();
        vector<ofxControlParameter<bool>*>::iterator itp = parameters.begin();
        while (itp != parameters.end())
        {
            if ((*itp)->getName() == toggleName) {
                parameters.erase(itp);
            }
            else {
                ++itp;
            }
        }
        while (it != elements.end())
        {
            if ((*it)->getName() == toggleName) {
                elements.erase(it);
            }
            else {
                ++it;
            }
        }
        menuElements.erase(toggleName);
        resetIndices();
        resetGuiPositions();
    }
}

void ofxControlMenu::clearToggles()
{
    vector<ofxControlParameter<bool>*>::iterator itp = parameters.begin();
    map<string, MenuElement*>::iterator itm = menuElements.begin();
    vector<ofxControlElement*>::iterator it = elements.begin();

    while (itp != parameters.end())
    {
        parameters.erase(itp);
    }
    while (itm != menuElements.end())
    {
        delete itm->second;
        menuElements.erase(itm++);
    }
    while (it != elements.end())
    {
        ofNotifyEvent(removeElementEvent, *it, this);
        delete *it;
        elements.erase(it);
    }
    parameters.clear();
    menuElements.clear();
    elements.clear();
    resetGuiPositions();
}

void ofxControlMenu::resetIndices()
{
    int index = 0;
    for (auto e : elements) {
        menuElements[e->getName()]->index = index++;
    }
}

void ofxControlMenu::buttonChanged(ofxControlButtonEventArgs &e)
{
    setToggle(e.button->getName(), e.value);
}

void ofxControlMenu::setAutoClose(bool autoClose)
{
    this->autoClose = autoClose;
    if (!autoClose && !multipleChoice) {
        setHeader(getName());
    }
}

void ofxControlMenu::getParameters(vector<ofxControlParameterBase*> & parameters_)
{
    for (auto p : parameters) {
        parameters_.push_back(p);
    }
}

void ofxControlMenu::updateParameterOscAddress()
{
    int idx = 0;
    for (auto p : parameters) {
        p->setOscAddress(getAddress()+"/"+ofToString(idx++));
    }
}

