#include "ofxControl2dPad.h"


ofxControl2dPadPoint::ofxControl2dPadPoint(ofxControlParameter<ofPoint> *parameter)
{
    this->parameter = parameter;
    padValue.set(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                 ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0));
    previous.set(parameter->get());
    lerpFrame = 0;
    lerpNumFrames = 0;
    lerpPrevValue = padValue;
    lerpNextValue = padValue;
}

ofxControl2dPadPoint::~ofxControl2dPadPoint()
{
    delete parameter;
}

void ofxControl2dPadPoint::setValue(ofPoint padValue)
{
    this->padValue = padValue;
    parameter->set(parameter->getMax() * padValue + parameter->getMin() * (1.0 - padValue));
}

void ofxControl2dPadPoint::lerpTo(ofPoint nextValue, int numFrames)
{
    if (numFrames > 1)
    {
        lerpNextValue = nextValue;
        lerpPrevValue = padValue;
        lerpNumFrames = numFrames;
        lerpFrame = 0;
    }
    else {
        setValue(nextValue);
    }
}

void ofxControl2dPadPoint::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        float r = (float) lerpFrame / (lerpNumFrames-1);
        setValue(lerpPrevValue * (1.0 - r) + lerpNextValue * r);
        lerpFrame++;
    }
    if (previous != parameter->get())
    {
        padValue.set(ofClamp((parameter->get().x - parameter->getMin().x) / (parameter->getMax().x - parameter->getMin().x), 0.0, 1.0),
                     ofClamp((parameter->get().y - parameter->getMin().y) / (parameter->getMax().y - parameter->getMin().y), 0.0, 1.0));
        previous.set(parameter->get());
    }
}

void ofxControl2dPadPoint::increment(float x, float y)
{
    setValue(ofPoint(ofClamp(padValue.x + x, 0, 1),
                     ofClamp(padValue.y + y, 0, 1)));
}

string ofxControl2dPadPoint::getValueString()
{
    return ofToString(parameter->get().x, floor(parameter->get().x) == parameter->get().x ? 0 : 2) + "," + ofToString(parameter->get().y, floor(parameter->get().y) == parameter->get().y ? 0 : 2) + ")";
}

ofxControl2dPad::ofxControl2dPad(ofxControlParameter<ofPoint> *parameter) : ofxControlMultiElement(parameter->getName())
{
    setupPad(parameter->getMin(), parameter->getMax());
    addPoint(parameter);
}

ofxControl2dPad::ofxControl2dPad(string name, ofPoint *value, ofPoint min, ofPoint max) : ofxControlMultiElement(name)
{
    setupPad(min, max);
    addPoint(new ofxControlParameter<ofPoint>(name, value, min, max));
}

ofxControl2dPad::ofxControl2dPad(string name, ofPoint min, ofPoint max) : ofxControlMultiElement(name)
{
    setupPad(min, max);
}

void ofxControl2dPad::setMin(ofPoint min)
{
    this->min = min;
    setupGuiPositions();
}

void ofxControl2dPad::setMax(ofPoint max)
{
    this->max = max;
    setupGuiPositions();
}

ofxControl2dPadPoint * ofxControl2dPad::addPoint(ofxControlParameter<ofPoint> *parameter)
{
    ofxControl2dPadPoint *newPoint = new ofxControl2dPadPoint(parameter);
    points.push_back(newPoint);
    if (parameter->getMin().x < min.x)  min.x = parameter->getMin().x;
    if (parameter->getMax().x > max.x)  max.x = parameter->getMax().x;
    if (parameter->getMin().y < min.y)  min.y = parameter->getMin().y;
    if (parameter->getMax().y > max.y)  max.y = parameter->getMax().y;
    
    ofxControl2dPadEventArgs args(newPoint, newPoint->padValue);
    ofNotifyEvent(padEvent, args, this);
    return newPoint;
}

ofxControl2dPadPoint * ofxControl2dPad::addPoint(ofPoint *value)
{
    return addPoint(new ofxControlParameter<ofPoint>(name, value, min, max));
}

ofxControl2dPadPoint * ofxControl2dPad::addPoint()
{
    return addPoint(new ofxControlParameter<ofPoint>(name, new ofPoint(0, 0), min, max));
}

void ofxControl2dPad::removePoint(int idx)
{
    if (idx < 0 || idx >= points.size())  return;
    delete points[idx];
    points.erase(points.begin() + idx);
}

void ofxControl2dPad::clearPoints()
{
    vector<ofxControl2dPadPoint*>::iterator it = points.begin();
    while (it != points.end())
    {
        delete *it;
        points.erase(it);
    }
    points.clear();
}

ofxControl2dPad::~ofxControl2dPad()
{
    clearPoints();
}

void ofxControl2dPad::setParent(ofxControlElement *parent)
{
    this->parent = parent;
    hasParent = (parent != NULL);
    setCollapsible(hasParent);
}

void ofxControl2dPad::setupPad(ofPoint min, ofPoint max)
{
    setMin(min);
    setMax(max);
    idxActive = -1;
    setCollapsible(false);
    setHeight(width);
    setDrawConnectedPoints(false);
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
}

void ofxControl2dPad::updateParameterOscAddress()
{
    int idx = 0;
    for (auto p : points) {
        p->parameter->setOscAddress(getAddress()+"/"+ofToString(idx++));
    }
}

void ofxControl2dPad::setupGuiPositions()
{
    if (collapsible)
    {
        headerRectangle.set(x, y, width, headerHeight);
        if (getCollapsed())
        {
            padRectangle.set(0, 0, 0, 0);
            rectangle.set(x, y, width, headerHeight);
        }
        else
        {
            padRectangle.set(x + marginX, y + headerHeight + marginY, getWidth() - 2 * marginX, getHeight() - headerHeight - 2 * marginY);
            rectangle.set(x, y, width, height);
        }
    }
    else
    {
        headerRectangle.set(0, 0, 0, 0);
        padRectangle.set(rectangle);
    }
}

void ofxControl2dPad::addElementToTouchOscLayout(ofxControlTouchOscPage *page, float *y)
{

}

void ofxControl2dPad::setValue(int idx, ofPoint padValue)
{
    points[idx]->setValue(padValue);
    updateValueString();
    ofxControl2dPadEventArgs args(points[idx], padValue);
    ofNotifyEvent(padEvent, args, this);
}

void ofxControl2dPad::updateValueString()
{
    if (idxActive == -1)    return;
    valueStringNext = "(" + ofToString(idxActive) + " : " + points[idxActive]->getValueString();
    toUpdateValueString = true;
}

void ofxControl2dPad::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    for (auto p : points) {
        parameters.push_back(p->parameter);
    }
}

void ofxControl2dPad::lerpTo(int idx, ofPoint nextValue, int numFrames)
{
    points[idx]->lerpTo(nextValue, numFrames);
}

void ofxControl2dPad::selectPoint(float x, float y)
{
    float minDist = 2.0;
    idxActive = -1;
    for (int idx = 0; idx < points.size(); idx++)
    {
        float padValueDist = ofDist(x, y, points[idx]->padValue.x, points[idx]->padValue.y);
        if (padValueDist < minDist)
        {
            minDist = padValueDist;
            if (minDist < 0.1) {
                idxActive = idx;
            }
        }
    }
    if (idxActive != -1) {
        updateValueString();
    }
}

void ofxControl2dPad::update()
{
    for (auto p : points) {
        p->update();
    }
}

void ofxControl2dPad::draw()
{
    if (toUpdateValueString)
    {
        valueString = valueStringNext;
        valueStringWidth = ofBitmapStringGetBoundingBox(valueString, 0, 0).width;
        stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
        toUpdateValueString = false;
    }
    
    ofxControlMultiElement::draw();
    
    ofPushStyle();

    ofSetColor(colorBackground);
    ofFill();
    ofSetLineWidth(1);
    ofRect(padRectangle);
    
    ofSetColor(colorForeground);
    ofNoFill();
    
    if (mouseOverPad && idxActive != -1)
    {
        ofLine(padRectangle.x, padRectangle.y + points[idxActive]->padValue.y * padRectangle.height, padRectangle.x + padRectangle.width, padRectangle.y + points[idxActive]->padValue.y * padRectangle.height);
        ofLine(padRectangle.x + points[idxActive]->padValue.x * padRectangle.width, padRectangle.y, padRectangle.x + points[idxActive]->padValue.x * padRectangle.width, padRectangle.y + padRectangle.height);
    }
    for (auto p : points) {
        ofCircle(padRectangle.x + p->padValue.x * padRectangle.width, padRectangle.y + p->padValue.y * padRectangle.height, 6);
    }
    
    if (connectPoints)
    {
        ofBeginShape();
        for (auto p : points) {
            ofVertex(padRectangle.x + p->padValue.x * padRectangle.width, padRectangle.y + p->padValue.y * padRectangle.height);
        }
        ofEndShape(true);
    }
    
    ofSetColor(colorOutline, 150);
    ofRect(padRectangle);
    
    if (mouseOverPad)
    {
        ofSetLineWidth(2);
        ofSetColor(colorActive);
        ofRect(padRectangle);
        ofSetLineWidth(1);
        
        ofSetColor(colorText);
        if (!collapsible) {
            ofDrawBitmapString(display, padRectangle.x + 2, padRectangle.y + 2 + stringHeight);
        }
        if (idxActive != -1) {
            ofDrawBitmapString(valueString, padRectangle.x + padRectangle.width - valueStringWidth - 2, padRectangle.y + padRectangle.height - 2);
        }
    }
    
    ofPopStyle();
}

bool ofxControl2dPad::mouseMoved(int mouseX, int mouseY)
{
    ofxControlMultiElement::mouseMoved(mouseX, mouseY);
    mouseOverPad = padRectangle.inside(mouseX, mouseY);
    if (mouseOverPad)
    {
        selectPoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                    ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1));
    }
    return mouseOverPad;
}

bool ofxControl2dPad::mousePressed(int mouseX, int mouseY)
{
    ofxControlMultiElement::mousePressed(mouseX, mouseY);
    if (mouseOverPad && idxActive != -1)
    {
        setValue(idxActive,
                 ofPoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                         ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1)));
    }
    return mouseOverPad;
}

bool ofxControl2dPad::mouseReleased(int mouseX, int mouseY)
{
    return ofxControlMultiElement::mouseReleased(mouseX, mouseY);
}

bool ofxControl2dPad::mouseDragged(int mouseX, int mouseY)
{
    ofxControlMultiElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging && idxActive != -1)
    {
        setValue(idxActive,
                 ofPoint(ofClamp((float)(mouseX - padRectangle.x) / padRectangle.width, 0, 1),
                         ofClamp((float)(mouseY - padRectangle.y) / padRectangle.height, 0, 1)));
    }
    return mouseOverPad;
}

bool ofxControl2dPad::keyPressed(int key)
{
    if (mouseOverPad)
    {
        if (key == 'n')
        {
            float x = ofMap(ofClamp((float) (ofGetMouseX() - padRectangle.x) / padRectangle.width,  0, 1), 0, 1, min.x, max.x);
            float y = ofMap(ofClamp((float) (ofGetMouseY() - padRectangle.y) / padRectangle.height, 0, 1), 0, 1, min.y, max.y);
            addPoint(new ofPoint(x, y));
            idxActive = points.size() - 1;
            return true;
        }
        else if (idxActive != -1)
        {
            if (key == OF_KEY_BACKSPACE)
            {
                removePoint(idxActive);
                idxActive = -1;
                return true;
            }
            else if (key == OF_KEY_LEFT)
            {
                points[idxActive]->increment(-0.01, 0.0);
                return true;
            }
            else if (key == OF_KEY_RIGHT)
            {
                points[idxActive]->increment(+0.01, 0.0);
                return true;
            }
            else if (key == OF_KEY_UP)
            {
                points[idxActive]->increment(0.0, -0.01);
                return true;
            }
            else if (key == OF_KEY_DOWN)
            {
                points[idxActive]->increment(0.0, +0.01);
                return true;
            }
        }
    }
    return false;
}

void ofxControl2dPad::getXml(ofXml &xml)
{
    xml.addValue<ofPoint>("Min", min);
    xml.addValue<ofPoint>("Max", max);
    xml.addChild("Points");
    xml.setTo("Points");
    for (auto p : points) {
        xml.addValue<ofPoint>("Point", p->padValue);
    }
    xml.setToParent();
}

void ofxControl2dPad::setFromXml(ofXml &xml)
{
    setMin(xml.getValue<ofPoint>("Min"));
    setMax(xml.getValue<ofPoint>("Max"));

    xml.setTo("Points");
    if (xml.exists("Point[0]"))
    {
        int idx = 0;
        do {
            ofPoint pt = xml.getValue<ofPoint>("Point["+ofToString(idx)+"]");
            if (idx < points.size()) {
                points[idx]->setValue(pt);
            }
            else
            {
                ofxControl2dPadPoint * newPoint = addPoint();
                newPoint->setValue(pt);
            }
            idx++;
        }
        while(idx < xml.getNumChildren());
        
        while (idx < points.size()) {
            removePoint(idx);
        }
    }
    xml.setToParent();
}
