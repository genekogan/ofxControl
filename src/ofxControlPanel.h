#pragma once

#include "ofxControlWidget.h"
#include "ofxControlSequencer.h"
#include "ofxControlOscManagerPanel.h"


class ofxControlPanel : public ofxControlWidget
{
public:
    ofxControlPanel();
    ~ofxControlPanel();
    
    void setPosition(int x, int y);
    void setName(string name);
    
    bool mouseMoved(int mouseX, int mouseY);
    bool mousePressed(int mouseX, int mouseY);
    bool mouseDragged(int mouseX, int mouseY);
    bool mouseReleased(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void update();
    void draw();
    
    void enableControlRow();
    void disableControlRow();
    
    void createSequencer();
    void createOscManager(ofxControlOscManager *osc);

    bool getHasSequencer() {return sequencerMade;}
    ofxControlSequencer * getSequencer() {return sequencer;}

    bool getHasOscManagerPanel() {return oscManagerMade;}
    ofxControlOscManagerPanel * getOscManagerPanel() {return oscManager;}
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    void savePreset(string name);
    void loadPreset(string name);

    void setupGuiPositions();
    
protected:
    
    void eventToggleSequencer(ofxControlButtonEventArgs &e);
    void eventToggleOscManager(ofxControlButtonEventArgs &e);
    void eventTogglePresets(ofxControlButtonEventArgs &e);

    void savePresetPrompt(ofxControlButtonEventArgs &e);
    void loadPresetPrompt(ofxControlMenuEventArgs &e);
    void refreshPresetMenu();
    
    void saveSequencerToXml(ofXml &xml);
    void loadSequencerFromXml(ofXml &xml);

    ofxControlToggle *tOsc, *tSeq, *tXml;
    bool bOsc, bSeq, bXml;
    bool controlRow;
    
    ofxControlWidget *meta;
    ofxControlMenu *menuPresets;

    ofxControlSequencer *sequencer;
    bool sequencerMade;
    
    ofxControlOscManagerPanel *oscManager;
    bool oscManagerMade;
};
