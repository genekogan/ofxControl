#pragma once

#include "ofMain.h"
#include "ofxControlSlider.h"
#include "ofxControlRangeSlider.h"
#include "ofxControlMenu.h"
#include "ofxControlWidget.h"
#include "ofxControlBpm.h"
#include "ofxControlSequence.h"


struct MidiEventArgs
{
    int type;
    int note;
    int velocity;
    MidiEventArgs(int type, int note, int velocity);
};

class ofxControlMidiSequencer : public ofxControlWidget
{
public:

    class ofxControlMidiSequencerEvent : public ofxControlElement
    {
    public:
        ofxControlMidiSequencerEvent(int note, int velocity, int start, int end);
        void draw();
        int start, end;
        int note, velocity;
    };

    ofxControlMidiSequencer(string name);
    ~ofxControlMidiSequencer();
    
    void setBpm(int bpm);
    void setPeriod(int measures);
    void setActive(bool active);
    void setNoteDisplayRange(int noteMin, int noteMax);
    void setTimeDisplayRange(float start, float end);

    void draw();

    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    ofEvent<MidiEventArgs> midiEvent;

private:

    enum MidiEventMouseMode { MIDDLE, LEFT, RIGHT, TOP, BOTTOM };
    
    ofxControlMidiSequencerEvent * addMidiEvent(int row, int velocity, int start, int end);
    void removeSelectedEvents();
    void removeEvent(ofxControlMidiSequencerEvent *event, bool toDelete=true);
    void repositionMidiEvent(ofxControlMidiSequencerEvent* event);
    
    void setNumberBeats(int numBeats);
    
    void setupKeys();
    void setupGuiPositions();
    void setMidiEventRectangle(ofxControlMidiSequencerEvent *event);
    
    void drawGrid();
    void drawKeyboard();
    void drawVelocity();
    void drawToolbar();
    
    void eventBeat();
    void eventSelectRoot(ofxControlMenuEventArgs &e);
    void eventSelectKey(ofxControlMenuEventArgs &e);
    void eventPlay(ofxControlButtonEventArgs &e);
    void eventPause(ofxControlButtonEventArgs &e);
    void eventStop(ofxControlButtonEventArgs &e);
    void eventSetBpm(ofxControlSliderEventArgs<int> &e);
    void eventSetPeriod(ofxControlSliderEventArgs<int> &e);
    void eventSetTimeView(ofxControlRangeSliderEventArgs<float> &e);
    
    ofxControlMenu *keySelect;
    ofxControlMenu *rootSelect;
    ofxControlButton *play, *pause, *stop, *clear;
    ofxControlSlider<int> *sBpm, *sPeriod;
    ofxControlRangeSlider<float> *rTime;
    
    ofxControlMidiSequencerEvent *newEvent;
    ofxControlMidiSequencerEvent *selectedEvent, *mouseOverEvent;
    vector<ofxControlMidiSequencerEvent*> events;
    vector<vector<ofxControlMidiSequencerEvent*> > midiOnEvents;
    vector<vector<ofxControlMidiSequencerEvent*> > midiOffEvents;
    
    int numRows;
    float rowHeight, colWidth;
    int noteMin, noteMax;
    float start, end;

    int root, key;
    map<int, bool> rootIsMajor;
    map<int, bool> rootIsMinor;
    
    ofxControlBpm clock;
    int bpm;
    int measures;
    int numBeats;
    int beat;
    bool active;
    
    bool shift;
    ofPoint pMouse;
    int pNoteMin, pNoteMax;
    float pStart, pEnd;
    int selectedNote;
    bool noteSelected;
    MidiEventMouseMode mouseOverEventMode;
    ofPoint pMouseOverEventNoteVelocity;
    ofPoint pMouseOverEventStartEnd;
    ofRectangle selection;
    
    ofRectangle rectGrid;
    ofRectangle rectKeyboard;
    ofRectangle rectVelocity;
    ofRectangle rectToolbar;
    ofRectangle rectTimebar;
    bool mouseOverGrid;
    bool mouseOverKeyboard;
    bool mouseOverVelocity;
    int keyboardSkip;
    int keyboardWidth;
    int velocityHeight;
    int toolbarHeight;
    int timeScrollHeight;
};

