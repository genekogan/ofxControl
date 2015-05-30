#pragma once

#include "ofMain.h"
#include "ofxControlParameter.h"
#include "ofxControlBase.h"


class ofxControlSequence;

struct ofxControlSequenceEventArgs
{
    ofxControlSequence *sequence;
    int cell;
    float value;
    
    ofxControlSequenceEventArgs(ofxControlSequence *sequence, int cell, float value)
    {
        this->sequence = sequence;
        this->cell = cell;
        this->value = value;
    }
};


class ofxControlSequence : public ofxControlBase
{
public:
    
    struct SequenceKeyboardEventArgs
    {
        int column;
        float value;
        SequenceKeyboardEventArgs(int column, float value);
    };
    
    ofxControlSequence(string name, int numCells);
    ofxControlSequence(string name);
    ~ofxControlSequence();
    
    void setActive(bool active);
    void setDiscrete(bool discrete);
    void setNumberCells(int numCells);
    void setCursor(float cursor);
    
    bool getActive() {return active;}
    bool getDiscrete() {return discrete;}
    int getSize() {return numCells;}
    float getCursor() {return cursor;}
    
    void setValueAtCell(int idx, float value, bool sendNotification=true);
    void setFromValues(vector<float> values);
    void randomize(float density, float range);
    
    float getValueAtIndex(int idx);
    float getValueAtCurrentIndex();
    float getValueAtCursor(float cursor_);
    float getValueAtCurrentCursor();
    
    vector<float> getAllValues() {return values;}
    
    ofRectangle getActiveRectangle() {return activeRectangle;}
    ofRectangle getSequenceRectangle() {return sequenceRectangle;}
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    ofEvent<ofxControlSequenceEventArgs> sequenceEvent;
    ofEvent<SequenceKeyboardEventArgs> keyboardEvent;
    
protected:
    
    void setupSequence();
    void setupGuiPositions();
    
    bool active;
    bool discrete;
    vector<float> values;
    int numCells;
    int activeCell;
    float cellWidth;
    ofPoint mousePos;
    float cursor, cursorLerp;
    ofRectangle activeRectangle, sequenceRectangle;
    bool mouseOverActive, mouseOverSequencer;
    bool editing;
    string editingValue;
};
