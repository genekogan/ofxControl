#include "ofxControlSlider.h"


ofxControlSliderBase::ofxControlSliderBase(string name) : ofxControlElement(name)
{
    sliderValue = 0.5;
    warp = 1.0;
    lerpFrame = 0;
    lerpNumFrames = 0;
    changed = false;
    toUpdateValueString = false;
    exp = false;
}

ofxControlSliderBase::~ofxControlSliderBase()
{
    
}

void ofxControlSliderBase::setValue(float sliderValue)
{
    this->sliderValue = sliderValue;
    changed = true;
}

void ofxControlSliderBase::lerpTo(float nextValue, int numFrames)
{
    if (numFrames > 1)
    {
        this->lerpNextValue = nextValue;
        this->lerpPrevValue = sliderValue;
        this->lerpNumFrames = numFrames;
        this->lerpFrame = 0;
    }
    else {
        setValue(nextValue);
    }
}

void ofxControlSliderBase::setValueFromSequence(ofxControlSequence &sequence)
{
    setValue(sequence.getValueAtCurrentCursor());
}

void ofxControlSliderBase::setSequenceFromValue(ofxControlSequence &sequence, int column)
{
    sequence.setValueAtCell(column, getValue());
}

void ofxControlSliderBase::setEditing(bool editing)
{
    this->editing = editing;
    if (editing) {
        colorActive = GUI_DEFAULT_COLOR_ACTIVE_EDIT;
    }
    else
    {
        editingString = "";
        colorActive = GUI_DEFAULT_COLOR_ACTIVE;
        updateValueString();
    }
}

void ofxControlSliderBase::update()
{
    if (lerpFrame < lerpNumFrames)
    {
        setValue(ofLerp(lerpPrevValue, lerpNextValue, (float) lerpFrame / (lerpNumFrames-1)));
        lerpFrame++;
    }
}

void ofxControlSliderBase::draw()
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
    ofRect(rectangle.x,
           rectangle.y,
           rectangle.width * sliderValue,
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

bool ofxControlSliderBase::mouseMoved(int mouseX, int mouseY)
{
    ofxControlElement::mouseMoved(mouseX, mouseY);
    if (editing && !mouseOver) {
        setEditing(false);
    }
    return mouseOver;
}

bool ofxControlSliderBase::mousePressed(int mouseX, int mouseY)
{
    ofxControlElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        setValue(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1));
    }
    return mouseOver;
}

bool ofxControlSliderBase::mouseReleased(int mouseX, int mouseY)
{
    return ofxControlElement::mouseReleased(mouseX, mouseY);
}

bool ofxControlSliderBase::mouseDragged(int mouseX, int mouseY)
{
    ofxControlElement::mouseDragged(mouseX, mouseY);
    if (mouseDragging)
    {
        setValue(ofClamp((float)(mouseX - rectangle.x) / rectangle.width, 0, 1));
    }
    return mouseOver;
}

bool ofxControlSliderBase::keyPressed(int key)
{
    ofxControlElement::keyPressed(key);
    if (mouseOver)
    {
        keyboardEdit(key);
        return true;
    }
    return false;
}

void ofxControlSliderBase::keyboardEdit(int key)
{
    if (key==OF_KEY_RETURN)
    {
        if (editing)
        {
            setParameterValueFromString(editingString);
            setEditing(false);
        }
        else
        {
            setEditing(true);
            editingString = getParameterValueString();
        }
    }
    else if (key==OF_KEY_BACKSPACE)
    {
        if (!editing)
        {
            setEditing(true);
            editingString = getParameterValueString();
        }
        if (editingString.length() > 1)
        {
            editingString = editingString.substr(0, editingString.length()-1);
            valueStringNext = editingString;
            toUpdateValueString = true;
        }
    }
    else if (key == OF_KEY_LEFT)
    {
        decrement();
    }
    else if (key == OF_KEY_RIGHT)
    {
        increment();
    }
    else if ((key >= 48 && key <= 57) || key == 46)
    {
        if (!editing) {
            setEditing(true);
        }
        editingString += key;
        valueStringNext = editingString;
        toUpdateValueString = true;
    }
}

void ofxControlSliderBase::getXml(ofXml &xml)
{
    xml.addValue<float>("Value", getValue());
}

void ofxControlSliderBase::setFromXml(ofXml &xml)
{
    setValue(xml.getValue<float>("Value"));
}

