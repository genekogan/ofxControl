#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"
#include "ofxControlSlider.h"


class ofxControlMultiElement : public ofxControlElement
{
public:
    ofxControlMultiElement(string name);
    virtual ~ofxControlMultiElement();
    
    virtual bool isMultiElement() {return true;}
    virtual bool isMenu() {return false;}
    
    vector<ofxControlElement*> & getElements() {return elements;}
    ofxControlElement * getElement(string name);
    
    virtual void getParameters(vector<ofxControlParameterBase*> & parameters_);

    bool removeElement(string name);
    void clearElements();
    
    void setHeader(string header);
    void setCollapsible(bool collapsible);
    void setNumberVisibleElements(int nView);

    bool getCollapsible() {return collapsible;}
    bool getCollapsed();
    void setCollapsed(bool collapsed);

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool keyPressed(int key);

    void setHeaderColor(ofColor headerColor) {this->headerColor = headerColor;}
    void setHeaderHeight(int headerHeight) {this->headerHeight = headerHeight;}
    void setControllerHeight(int controllerHeight) {this->controllerHeight = controllerHeight;}
    void setMarginX(int marginX) {this->marginX = marginX;}
    void setMarginY(int marginY) {this->marginY = marginY;}

    ofColor getHeaderColor() {return headerColor;}
    int getHeaderHeight() {return headerHeight;}
    int getControllerHeight() {return controllerHeight;}
    int getMarginX() {return marginX;}
    int getMarginY() {return marginY;}
    
    virtual void update();
    virtual void draw();
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    ofEvent<ofxControlElement*> newElementEvent;
    ofEvent<ofxControlElement*> removeElementEvent;
    
protected:
    
    void setupGuiPositions();
    
    void setPage(int page);
    void nextPage();
    void prevPage();
    
    void addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y);
    void updateParameterOscAddress();
    virtual void initializeElement(ofxControlElement *element, bool sendNotification=true);
    
    vector<ofxControlElement*> elements;

    // multiple pages
    int nView, nPages, page, start, end;
    ofRectangle rPrev, rNext;
    bool rPrevActive, rNextActive;

    // header
    ofRectangle headerRectangle;
    string header;
    bool headerActive;
    bool collapsible, collapsed;
    float headerStringHeight;
    bool draggingWidget;
    
    // style
    ofColor headerColor;
    int headerHeight;
    int controllerHeight;
    int marginX;
    int marginY;
};
