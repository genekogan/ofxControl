#pragma once

#include "ofMain.h"
#include "ofxControlSlider.h"
#include "ofxControlWidget.h"
#include "ofxControlMenu.h"
#include "ofxControlBpm.h"
#include "ofxControlSequence.h"



class ofxControlSequencer : public ofxControlElement
{
public:
    ofxControlSequencer(string name, ofxControlWidget * panel, int numCols=8);
    ~ofxControlSequencer();
    
    void setActive(bool active);
    void setSmooth(bool smooth);
    void setBpm(int bpm);
    void setLerpNumFrames(int lerpNumFrames);
    void setNumberColumns(int numCols);
    
    bool getActive() {return active;}
    bool getSmooth() {return smooth;}
    int getBpm() {return bpm;}
    int getLerpNumFrames() {return lerpNumFrames;}
    int getNumberColumns() {return numCols;}
    
    void setAllSequencersActive(bool allSequencesActive);
    void randomizeSequencer();
    void selectColumn(int column);
    void setColumnToCurrentValues(int column);
    void next();
    
    void saveSequencerSet(string sequencerName="");
    void loadSequencerSet(string sequencerName);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    void setupGuiPositions();
    
protected:
    
    struct SequenceElementPair
    {
        ofxControlElement *element;
        ofxControlSequence *sequence;
        SequenceElementPair(ofxControlElement *element, int numCols);
        void eventSetSequenceFromKey(ofxControlSequence::SequenceKeyboardEventArgs & evt);
    };
    
    struct ElementSequenceGroup
    {
        ofxControlElement *element;
        vector<SequenceElementPair*> pairs;
        ElementSequenceGroup(ofxControlElement *element, int numCols);
        bool isSingleChoiceMenu();
        void randomizeSequencer();
        void menuEvent(ofxControlSequenceEventArgs & evt);
    };
    
    struct SequencerSavedSet
    {
        struct SequencerSavedSetGroup
        {
            map<string, vector<float> > sequences;
            map<string, bool> sequencesActive;
            void addSequence(string sequenceName, vector<float> sequence, bool active);
        };
        
        string name;
        bool smooth;
        int bpm;
        int lerpNumFrames;
        int numCols;
        map<string, SequencerSavedSetGroup> sequenceGroups;
        
        SequencerSavedSet() { }
        SequencerSavedSet(string name, bool smooth, int bpm, int lerpNumFrames, int numCols);
        void addSequence(string sequenceGroupName, string sequenceName, vector<float> sequence, bool active);
    };
    
    void setupSequencer();
    void addElement(ofxControlElement* & newElement);
    void removeElement(ofxControlElement* & element);
    
    void eventBpm(ofxControlSliderEventArgs<int> &s);
    void eventNumColumns(ofxControlSliderEventArgs<int> &s);
    void eventActive(ofxControlButtonEventArgs &b);
    void eventRandomizeSequencer(ofxControlButtonEventArgs &b);
    void eventSelectColumn(ofxControlSliderEventArgs<int> &b);
    void eventSequencerMenuSelection(ofxControlMenuEventArgs & evt);
    void eventBeat();
    
    ofxControlBpm clock;
    int numCols;
    int column;
    float cursor;
    int bpm;
    int bpmTime;
    float bpmInterval;
    int currentColumn;
    int lerpNumFrames;
    bool active;
    bool smooth;
    bool allSequencesActive;
    
    map<ofxControlElement*,ElementSequenceGroup*> groups;
    map<string, SequencerSavedSet> sequencerSets;
    ofxControlWidget * panel;
    
    vector<ofxControlElement*> buttons;
    ofxControlMenu *mChoose;
    ofRectangle bToggleAllSequencers;
    vector<ofRectangle> bColumnSelectors;
    int rectSelectColumnMouseOver;
    bool rectToggleAllSeqMouseOver;
    
    int sequencerPanelMargin;
    int sequencerWidth;
    int sequencerButtonHeight;
    int marginInner;
};

