#include "ofxControlRangeSlider.h"


ofxControlRangeSliderBase::ofxControlRangeSliderBase(string name) : ofxControlElement(name)
{
    sliderLow = 0.333;
    sliderHigh = 0.667;
    lerpFrame = 0;
    lerpNumFrames = 0;
    toUpdateValueString = false;
}

void ofxControlRangeSliderBase::selectSlider(float sliderValue)
{
    float dLow = abs(sliderValue - sliderLow);
    float dHigh = abs(sliderValue - sliderHigh);
    float dMiddle = abs(sliderValue - (0.5 * (sliderLow + sliderHigh)));
    
    if (dMiddle < dLow && dMiddle < dHigh)
    {
        selection = MIDDLE;
        sliderValuePrev = sliderValue;
    }
    else {
        selection = dLow < dHigh ? LOW : HIGH;
    }
}

void ofxControlRangeSliderBase::setValue(float sliderValue)
{
    if (selection == MIDDLE)
    {
        float diff = ofClamp(sliderValue - sliderValuePrev, -sliderLow, 1.0 - sliderHigh);
        setValueLow(sliderLow + diff);
        setValueHigh(sliderHigh + diff);
        sliderValuePrev = sliderValue;
    }
    else
    {
        if (sliderLow > sliderHigh)
        {
            swap(sliderLow, sliderHigh);
            selection = selection == LOW ? HIGH : LOW;
        }
        selection == LOW ? setValueLow(sliderValue) : setValueHigh(sliderValue);
    }
}

void ofxControlRangeSliderBase::setValueLow(float sliderLow, bool toSendNotification)
{
    this->sliderLow = sliderLow;
}

void ofxControlRangeSliderBase::setValueHigh(float sliderHigh, bool toSendNotification)
{
    this->sliderHigh = sliderHigh;
}

void ofxControlRangeSliderBase::lerpTo(float nextLow, float nextHigh, int numFrames)
{
    if (numFrames > 1)
    {
        this->lerpNextLow = nextLow;
        this->lerpNextHigh = nextHigh;
        this->lerpPrevLow = sliderLow;
        this->lerpPrevHigh = sliderHigh;
        this->lerpNumFrames = numFrames;
        this->lerpFrame = 0;
    }
    else {
        setValueLow(nextLow);
        setValueHigh(nextHigh);
    }
}

void ofxControlRangeSliderBase::setValueFromSequence(ofxControlSequence &sequence)
{
    float diff = sequence.getValueAtCurrentCursor() - sliderValuePrev;
    setValueLow(max(0.0f, sliderLow + diff));
    setValueHigh(min(1.0f, sliderHigh + diff));
    sliderValuePrev = sequence.getValueAtCurrentCursor();
}

void ofxControlRangeSliderBase::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        setValueLow(ofLerp(lerpPrevLow, lerpNextLow, (float) lerpFrame / (lerpNumFrames-1)));
        setValueHigh(ofLerp(lerpPrevHigh, lerpNextHigh, (float) lerpFrame / (lerpNumFrames-1)));
        lerpFrame++;
    }
}

void ofxControlRangeSliderBase::draw()
{
    if (toUpdateValueString)
    {
        valueString = valueStringNext;
        valueStringWidth = ofBitmapStringGetBoundingBox(valueString, 0, 0).width;
        stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
        toUpdateValueString = false;
    }
    
    ofPushStyle();
    
    ofFill();
    ofSetColor(colorBackground);
    ofSetLineWidth(1);
    ofRect(rectangle);
    
    ofSetColor(colorForeground);
    ofRect(rectangle.x + rectangle.width * sliderLow,
           rectangle.y,
           rectangle.width * (sliderHigh - sliderLow),
           rectangle.height);
    
    ofNoFill();
    ofSetColor(colorOutline);
    ofRect(rectangle);
    
    if (mouseOver)
    {
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(colorActive);
        ofRect(rectangle);
        ofSetLineWidth(1);
    }
    
    ofSetColor(colorText);
    ofDrawBitmapString(display,
                       rectangle.x + 3,
                       rectangle.y + 1 + 0.5 * (rectangle.height + 0.5 * stringHeight));
    ofDrawBitmapString(valueString,
                       rectangle.x + rectangle.width - valueStringWidth - 1,
                       rectangle.y + 1 + 0.5 * (rectangle.height + 0.5 * stringHeight));
    
    ofPopStyle();
}

void ofxControlRangeSliderBase::keyboardEdit(int key)
{
    if (key == OF_KEY_LEFT)
    {
        decrement();
    }
    else if (key == OF_KEY_RIGHT)
    {
        increment();
    }
}

bool ofxControlRangeSliderBase::mouseMoved(int mouseX, int mouseY)
{
    return ofxControlElement::mouseMoved(mouseX, mouseY);
}

bool ofxControlRangeSliderBase::mousePressed(int mouseX, int mouseY)
{
    ofxControlElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        float sliderValue = ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1);
        selectSlider(sliderValue);
        setValue(sliderValue);
    }
    return mouseOver;
}

bool ofxControlRangeSliderBase::mouseReleased(int mouseX, int mouseY)
{
    return ofxControlElement::mouseReleased(mouseX, mouseY);
}

bool ofxControlRangeSliderBase::mouseDragged(int mouseX, int mouseY)
{
    ofxControlElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        setValue(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1));
    }
    return mouseOver;
}

bool ofxControlRangeSliderBase::keyPressed(int key)
{
    ofxControlElement::keyPressed(key);
    if (mouseOver)
    {
        float sliderValue = ofClamp((float)(ofGetMouseX() - rectangle.x) / rectangle.width, 0, 1);
        selectSlider(sliderValue);
        keyboardEdit(key);
    }
    return mouseOver;
}

void ofxControlRangeSliderBase::getXml(ofXml &xml)
{
    xml.addValue<float>("ValueLow", getValueLow());
    xml.addValue<float>("ValueHigh", getValueHigh());
}

void ofxControlRangeSliderBase::setFromXml(ofXml &xml)
{
    setValueLow(xml.getValue<float>("ValueLow"));
    setValueHigh(xml.getValue<float>("ValueHigh"));
}

