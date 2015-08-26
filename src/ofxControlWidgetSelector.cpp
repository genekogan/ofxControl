#include "ofxControlWidgetSelector.h"


ofxControlWidgetSelector::ofxControlWidgetSelector() : ofxControlWidget()
{
    setCollapsible(true);
    multiple = false;
}

ofxControlWidgetSelector::~ofxControlWidgetSelector()
{

}

void ofxControlWidgetSelector::addWidget(string groupName, ofxControlWidget *widget)
{
    ofxControlMenu *menu;
    if (menus.count(groupName) == 0) {
        menu = addMenu(groupName, this, &ofxControlWidgetSelector::eventSelectWidget);
        menu->setMultipleChoice(multiple);
        menus[groupName] = menu;
    }
    else {
        menu = menus[groupName];
    }
    ofxControlToggle *newButton = menu->addToggle(widget->getName());
    widgets[newButton] = widget;
    widget->setAutoDraw(false);
}

void ofxControlWidgetSelector::eventSelectWidget(ofxControlMenuEventArgs &evt)
{
    hideAll();
    if (multiple)
    {
        int left = getRectangle().getRight() + 2;
        map<ofxControlToggle*, ofxControlWidget*>::iterator it = widgets.begin();
        for (; it != widgets.end(); ++it) {
            if (it->first->getValue()) {
                it->second->setAutoDraw(true);
                it->second->setPosition(left, rectangle.y);
                left = it->second->getRectangle().getRight() + 2;
            }
        }
    }
    else {
        widgets[evt.toggle]->setAutoDraw(true);
        widgets[evt.toggle]->setPosition(getRectangle().getRight() + 2, rectangle.y);
    }
}

void ofxControlWidgetSelector::hideAll()
{
    map<ofxControlToggle*, ofxControlWidget*>::iterator it = widgets.begin();
    for (; it != widgets.end(); ++it) {
        it->second->setAutoDraw(false);
    }
}

void ofxControlWidgetSelector::setAllowMultiple(bool multiple)
{
    this->multiple = multiple;
    map<string, ofxControlMenu*>::iterator itm = menus.begin();
    for (; itm != menus.end(); ++itm) {
        itm->second->setMultipleChoice(multiple);
    }
}

bool ofxControlWidgetSelector::mouseDragged(int mouseX, int mouseY)
{
    bool mouseDragged = ofxControlMultiElement::mouseDragged(mouseX, mouseY);
    int left = getRectangle().getRight() + 2;
    map<ofxControlToggle*, ofxControlWidget*>::iterator it = widgets.begin();
    for (; it != widgets.end(); ++it) {
        if (it->second->getAutoDraw()) {
            it->second->setPosition(left, rectangle.y);
            left = it->second->getRectangle().getRight() + 2;
        }
    }
    return mouseDragged;
}