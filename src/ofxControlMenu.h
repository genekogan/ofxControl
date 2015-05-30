#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlButton.h"
#include "ofxControlMultiElement.h"



class ofxControlMenu;

struct ofxControlMenuEventArgs
{
    ofxControlToggle *toggle;
    int index;
    bool value;
    
    ofxControlMenuEventArgs(ofxControlToggle *toggle, int index, bool value)
    {
        this->toggle = toggle;
        this->index = index;
        this->value = value;
    }
};


class ofxControlMenu : public ofxControlMultiElement
{
public:

    struct MenuElement
    {
        ofxControlToggle *toggle;
        int index;
        ofxControlParameter<bool> *parameter;
        MenuElement(ofxControlToggle *toggle, ofxControlParameter<bool> *parameter, int index);
    };
    
    ofxControlMenu(string name, vector<string> choices, bool multipleChoice=false, bool autoClose=false);
    ofxControlMenu(string name, bool multipleChoice=false, bool autoClose=false);
    
    template <typename L, typename M>
    ofxControlMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice=false, bool autoClose=false);
    
    template <typename L, typename M>
    ofxControlMenu(string name, L *listener, M method, bool multipleChoice=false, bool autoClose=false);

    ~ofxControlMenu();

    bool isMenu() {return true;}
    
    ofxControlToggle * addToggle(ofxControlParameter<bool> *parameter);
    ofxControlToggle * addToggle(string choice, bool *value);
    ofxControlToggle * addToggle(string choice);
    
    template <typename L, typename M>
    ofxControlToggle * addToggle(ofxControlParameter<bool> *parameter, L *listener, M method);

    template <typename L, typename M>
    ofxControlToggle * addToggle(string choice, bool *value, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlToggle * addToggle(string choice, L *listener, M method);

    void removeToggle(string toggleName);
    void clearToggles();
    
    void setToggle(string toggleName, bool value);
    bool getToggle(string toggleName);
    
    void setAutoClose(bool autoClose);
    void setMultipleChoice(bool multipleChoice) {this->multipleChoice = multipleChoice;}
    
    bool getAutoClose() {return autoClose;}
    bool getMultipleChoice() {return multipleChoice;}
    
    void getParameters(vector<ofxControlParameterBase*> & parameters_);
    
    ofEvent<ofxControlMenuEventArgs> menuEvent;

private:

    void setupMenu(string name, vector<string> & choices, bool multipleChoice, bool autoClose);
    void updateParameterOscAddress();
    void resetIndices();
    void buttonChanged(ofxControlButtonEventArgs &e);
    
    bool multipleChoice;
    bool autoClose;

    vector<ofxControlParameter<bool>*> parameters;
    map<string, MenuElement*> menuElements;
};


template <typename L, typename M>
ofxControlMenu::ofxControlMenu(string name, vector<string> choices, L *listener, M method, bool multipleChoice, bool autoClose) : ofxControlMultiElement(name)
{
    setupMenu(name, choices, multipleChoice, autoClose);
    ofAddListener(menuEvent, listener, method);
}

template <typename L, typename M>
ofxControlMenu::ofxControlMenu(string name, L *listener, M method, bool multipleChoice, bool autoClose) : ofxControlMultiElement(name)
{
    vector<string> choices;
    setupMenu(name, choices, multipleChoice, autoClose);
    ofAddListener(menuEvent, listener, method);
}

template <typename L, typename M>
ofxControlToggle * ofxControlMenu::addToggle(ofxControlParameter<bool> *parameter, L *listener, M method)
{
    ofxControlToggle *toggle = addToggle(parameter);
    if (toggle != NULL) {
        ofAddListener(menuElements[toggle->getName()], listener, method);
    }
    return toggle;
}

template <typename L, typename M>
ofxControlToggle * ofxControlMenu::addToggle(string choice, bool *value, L *listener, M method)
{
    ofxControlToggle *toggle = addToggle(choice, value);
    if (toggle != NULL) {
        ofAddListener(menuElements[toggle->getName()], listener, method);
    }
    return toggle;
}

template <typename L, typename M>
ofxControlToggle * ofxControlMenu::addToggle(string choice, L *listener, M method)
{
    ofxControlToggle *toggle = addToggle(choice);
    if (toggle != NULL) {
        ofAddListener(menuElements[toggle->getName()], listener, method);
    }
    return toggle;
}
