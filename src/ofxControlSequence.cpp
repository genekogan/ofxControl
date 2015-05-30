#include "ofxControlSequence.h"


ofxControlSequence::SequenceKeyboardEventArgs::SequenceKeyboardEventArgs(int column, float value)
{
    this->column = column;
    this->value = value;
}

ofxControlSequence::ofxControlSequence(string name, int numCells) : ofxControlBase(name)
{
    setNumberCells(numCells);
    setupSequence();
}

ofxControlSequence::ofxControlSequence(string name) : ofxControlBase(name)
{
    setNumberCells(1);
    setupSequence();
}

ofxControlSequence::~ofxControlSequence()
{
    values.clear();
    setActive(false);
}

void ofxControlSequence::setupSequence()
{
    setActive(true);
    colorForeground = GUI_DEFAULT_SEQUENCER_COLOR_FOREGROUND;
    colorOutline = GUI_DEFAULT_SEQUENCER_COLOR_OUTLINE;
    mouseOverActive = false;
    mouseOverSequencer = false;
}

void ofxControlSequence::setupGuiPositions()
{
    activeRectangle = ofRectangle(rectangle.x, rectangle.y + rectangle.height / 8, rectangle.height - rectangle.height / 4, rectangle.height - rectangle.height / 4);
    sequenceRectangle = ofRectangle(rectangle.x + rectangle.height, rectangle.y, rectangle.width - rectangle.height, rectangle.height);
    cellWidth = (float) sequenceRectangle.width / numCells;
}

void ofxControlSequence::setActive(bool active)
{
    this->active = active;
}

void ofxControlSequence::setDiscrete(bool discrete)
{
    this->discrete = discrete;
}

void ofxControlSequence::setNumberCells(int numCells)
{
    this->numCells = numCells;
    values.resize(numCells);
}

void ofxControlSequence::setCursor(float cursor)
{
    this->cursor = fmodf(cursor, (float) numCells);
    cursorLerp = cursor - floor(cursor);
}

void ofxControlSequence::setValueAtCell(int idx, float value, bool sendNotification)
{
    if (idx >= numCells)
    {
        ofLog(OF_LOG_ERROR, "Warning: no cell at index "+ofToString(idx));
        return;
    }
    values[idx] = value;
    
    if (sendNotification)
    {
        ofxControlSequenceEventArgs args(this, idx, value);
        ofNotifyEvent(sequenceEvent, args, this);
    }
}

void ofxControlSequence::setFromValues(vector<float> values)
{
    this->values = values;
    numCells = values.size();
}

void ofxControlSequence::randomize(float density, float range)
{
    for (int i = 0; i < numCells; i++) {
        setValueAtCell(i, discrete ? ofRandom(1) < density : ofLerp(0.5 - 0.5 * range, 0.5 + 0.5 * range, ofRandom(1)), false);
    }
}

float ofxControlSequence::getValueAtIndex(int idx)
{
    return values[idx];
}

float ofxControlSequence::getValueAtCurrentIndex()
{
    return values[floor(cursor)];
}

float ofxControlSequence::getValueAtCursor(float cursor_)
{
    int idx1 = floor(cursor_);
    float cursorLerp_ = cursor_ - floor(cursor_);
    return ofLerp(values[idx1], values[(1+idx1) % values.size()], cursorLerp_);
}

float ofxControlSequence::getValueAtCurrentCursor()
{
    int idx1 = floor(cursor);
    return ofLerp(values[idx1], values[(1+idx1) % values.size()], cursorLerp);
}

bool ofxControlSequence::mouseMoved(int mouseX, int mouseY)
{
    ofxControlBase::mouseMoved(mouseX, mouseY);
    mouseOverActive = activeRectangle.inside(mouseX, mouseY);
    mouseOverSequencer = sequenceRectangle.inside(mouseX, mouseY);
    if (mouseOverSequencer)
    {
        int activeCellNext = floor((float) (ofGetMouseX() - sequenceRectangle.x) / cellWidth);
        if (activeCellNext != activeCell) {editing = false;}
        activeCell = activeCellNext;
    }
    return mouseOver || mouseOverActive || mouseOverSequencer;
}

bool ofxControlSequence::mousePressed(int mouseX, int mouseY)
{
    ofxControlBase::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        if (mouseOverSequencer)
        {
            if (discrete) {
                setValueAtCell(activeCell, (1.0 - values[activeCell]) > 0.5);
            }
            else {
                mousePos.set(mouseX, mouseY);
            }
            return true;
        }
        else if (mouseOverActive)
        {
            setActive(!active);
            return true;
        }
    }
    return false;
}

bool ofxControlSequence::mouseReleased(int mouseX, int mouseY)
{
    ofxControlBase::mouseReleased(mouseX, mouseY);
    return mouseOver || mouseOverActive || mouseOverSequencer;
}

bool ofxControlSequence::mouseDragged(int mouseX, int mouseY)
{
    ofxControlBase::mouseDragged(mouseX, mouseY);
    if (mouseDragging && !discrete)
    {
        values[activeCell] = ofClamp(values[activeCell] - 0.005 * (mouseY - mousePos.y), 0, 1);
        mousePos.set(mouseX, mouseY);
    }
    return mouseOver || mouseOverActive || mouseOverSequencer;
}

bool ofxControlSequence::keyPressed(int key)
{
    ofxControlBase::keyPressed(key);
    if (mouseOverSequencer)
    {
        if (key == 46 || (key >= 48 && key <= 57))
        {
            if (editing) {
                editingValue += key;
            }
            else
            {
                editing = true;
                editingValue = key;
            }
            return true;
        }
        else if (key == ' ')
        {
            if (getDiscrete())
            {
                setValueAtCell(activeCell, 1.0 - values[activeCell]);
            }
        }
        else if (key == OF_KEY_RETURN)
        {
            SequenceKeyboardEventArgs args(activeCell, ofToFloat(editingValue));
            ofNotifyEvent(keyboardEvent, args, this);
            editing = false;
            return true;
        }
    }
    return false;
}

void ofxControlSequence::update()
{
    
}

void ofxControlSequence::draw()
{
    ofPushStyle();
    
    active ? ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_ACTIVE) : ofSetColor(GUI_DEFAULT_SEQUENCER_COLOR_INACTIVE);
    ofFill();
    ofCircle(activeRectangle.x + activeRectangle.width / 2 + 1, activeRectangle.y + activeRectangle.height / 2, activeRectangle.width / 2);
    
    if (mouseOverActive)
    {
        ofSetColor(editing ? GUI_DEFAULT_COLOR_ACTIVE_EDIT : colorActive);
        ofNoFill();
        ofSetLineWidth(2);
        ofCircle(activeRectangle.x + activeRectangle.width / 2, activeRectangle.y + activeRectangle.height / 2, activeRectangle.width / 2);
        ofSetLineWidth(1);
    }
    
    if (active)
    {
        for (int i = 0; i < numCells; i++)
        {
            ofFill();
            ofSetColor(colorBackground);
            ofRect(sequenceRectangle.x + i * cellWidth,
                   sequenceRectangle.y,
                   cellWidth,
                   sequenceRectangle.height);
            
            ofSetColor(colorForeground);
            ofRect(sequenceRectangle.x + cellWidth * (i + 0.5 * (1.0 - values[i])),
                   sequenceRectangle.y + sequenceRectangle.height * (0.5 * (1.0 - values[i])),
                   cellWidth * values[i],
                   sequenceRectangle.height * values[i]);
            
            ofNoFill();
            ofSetColor(colorOutline);
            ofRect(sequenceRectangle.x + i * cellWidth, sequenceRectangle.y, cellWidth, sequenceRectangle.height);
        }
        
        if (mouseOverSequencer)
        {
            ofSetColor(colorActive);
            ofNoFill();
            ofSetLineWidth(2);
            ofRect(sequenceRectangle.x + activeCell * cellWidth, sequenceRectangle.y, cellWidth, sequenceRectangle.height);
            ofSetLineWidth(1);
        }
    }
    
    ofPopStyle();
}
