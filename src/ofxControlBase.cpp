#include "ofxControlBase.h"


ofxControlBase::ofxControlBase(string name)
{
    setName(name);
    initialize();
}

ofxControlBase::ofxControlBase()
{
    setName("Element");
    initialize();
}

ofxControlBase::~ofxControlBase()
{
    setActive(false);
}

void ofxControlBase::initialize()
{
    x = 0;
    y = 0;
    width = GUI_DEFAULT_ELEMENT_WIDTH;
    height = GUI_DEFAULT_ELEMENT_HEIGHT;
    setRectangle(0, 0, width, height);
    
    colorBackground = GUI_DEFAULT_COLOR_BACKGROUND;
    colorForeground = GUI_DEFAULT_COLOR_FOREGROUND;
    colorOutline = GUI_DEFAULT_COLOR_OUTLINE;
    colorText = GUI_DEFAULT_COLOR_TEXT;
    colorActive = GUI_DEFAULT_COLOR_ACTIVE;
    
    mouseDragging = false;
    mouseOver = false;
    
    autoUpdate = true;
    autoDraw = true;
    setActive(true);
}

void ofxControlBase::setActive(bool active)
{
    this->active = active;
    if (active)
    {
        setAutoUpdate(autoUpdate);
        setAutoDraw(autoDraw);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &ofxControlBase::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxControlBase::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxControlBase::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxControlBase::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &ofxControlBase::keyPressed);
        ofRemoveListener(ofEvents().update, this, &ofxControlBase::update);
        ofRemoveListener(ofEvents().draw, this, &ofxControlBase::draw);
    }
}

void ofxControlBase::setAutoUpdate(bool autoUpdate)
{
    this->autoUpdate = autoUpdate;
    if (autoUpdate)
    {
        ofAddListener(ofEvents().mouseMoved, this, &ofxControlBase::mouseMoved);
        ofAddListener(ofEvents().mousePressed, this, &ofxControlBase::mousePressed);
        ofAddListener(ofEvents().mouseDragged, this, &ofxControlBase::mouseDragged);
        ofAddListener(ofEvents().mouseReleased, this, &ofxControlBase::mouseReleased);
        ofAddListener(ofEvents().keyPressed, this, &ofxControlBase::keyPressed);
        ofAddListener(ofEvents().update, this, &ofxControlBase::update);
    }
    else
    {
        ofRemoveListener(ofEvents().mouseMoved, this, &ofxControlBase::mouseMoved);
        ofRemoveListener(ofEvents().mousePressed, this, &ofxControlBase::mousePressed);
        ofRemoveListener(ofEvents().mouseDragged, this, &ofxControlBase::mouseDragged);
        ofRemoveListener(ofEvents().mouseReleased, this, &ofxControlBase::mouseReleased);
        ofRemoveListener(ofEvents().keyPressed, this, &ofxControlBase::keyPressed);
        ofRemoveListener(ofEvents().update, this, &ofxControlBase::update);
    }
}

void ofxControlBase::setAutoDraw(bool autoDraw)
{
    this->autoDraw = autoDraw;
    if (autoDraw) {
        ofAddListener(ofEvents().draw, this, &ofxControlBase::draw);
    }
    else {
        ofRemoveListener(ofEvents().draw, this, &ofxControlBase::draw);
    }
}

void ofxControlBase::setX(int x)
{
    this->x = x;
    setRectangle(x, y, width, height);
}

void ofxControlBase::setY(int y)
{
    this->y = y;
    setRectangle(x, y, width, height);
}

void ofxControlBase::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
    setRectangle(x, y, width, height);
}

void ofxControlBase::setWidth(int width)
{
    this->width = width;
    setRectangle(x, y, width, height);
}

void ofxControlBase::setHeight(int height)
{
    this->height = height;
    setRectangle(x, y, width, height);
}

void ofxControlBase::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
    setRectangle(x, y, width, height);
}

void ofxControlBase::setRectangle(ofRectangle rectangle)
{
    this->rectangle = rectangle;
    setupGuiPositions();
}

void ofxControlBase::setMouseOver(bool mouseOver)
{
    this->mouseOver = mouseOver;
}

void ofxControlBase::setupGuiPositions()
{
    rectangle.set(x, y, width, height);
}

bool ofxControlBase::mouseMoved(int mouseX, int mouseY)
{
    mouseOver = active && rectangle.inside(mouseX, mouseY);
    return mouseOver;
}

bool ofxControlBase::mousePressed(int mouseX, int mouseY)
{
    if (mouseOver)
    {
        mouseDragging = true;
    }
    return mouseOver;
}

bool ofxControlBase::mouseDragged(int mouseX, int mouseY)
{
    return mouseOver && mouseDragging;
}

bool ofxControlBase::mouseReleased(int mouseX, int mouseY)
{
    mouseDragging = false;
    return mouseOver;
}

bool ofxControlBase::keyPressed(int key)
{
    return false;
}

void ofxControlBase::update()
{
    
}

void ofxControlBase::draw()
{
    ofPushStyle();
    ofSetColor(colorBackground);
    ofPopStyle();
}

void ofxControlBase::mouseMoved(ofMouseEventArgs &evt)
{
    mouseMoved(evt.x, evt.y);
}

void ofxControlBase::mousePressed(ofMouseEventArgs &evt)
{
    mousePressed(evt.x, evt.y);
}

void ofxControlBase::mouseDragged(ofMouseEventArgs &evt)
{
    mouseDragged(evt.x, evt.y);
}

void ofxControlBase::mouseReleased(ofMouseEventArgs &evt)
{
    mouseReleased(evt.x, evt.y);
}

void ofxControlBase::keyPressed(ofKeyEventArgs &evt)
{
    keyPressed(evt.key);
}

void ofxControlBase::update(ofEventArgs &data)
{
    update();
}

void ofxControlBase::draw(ofEventArgs &data)
{
    draw();
}
