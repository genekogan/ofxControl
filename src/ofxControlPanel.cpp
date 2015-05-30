#include "ofxControlPanel.h"


ofxControlPanel::ofxControlPanel() : ofxControlWidget()
{
    marginX = GUI_DEFAULT_PANEL_MARGIN_OUTER_X;
    controlRow = false;
    sequencerMade = false;
    oscManagerMade = false;
    hasParent = false;
    bOsc = false;
    bSeq = false;
    bXml = false;
    setCollapsible(true);
    enableControlRow();
}

ofxControlPanel::~ofxControlPanel()
{
    if (controlRow) {
        disableControlRow();
    }
}

void ofxControlPanel::setPosition(int x, int y)
{
    ofxControlElement::setPosition(x, y);;
    if (controlRow) {
        meta->setPosition(rectangle.x + rectangle.width + 4, rectangle.y);
    }
}

void ofxControlPanel::setName(string name)
{
    ofxControlWidget::setName(name);
    refreshPresetMenu();
}

void ofxControlPanel::enableControlRow()
{
    if (!controlRow)
    {
        controlRow = true;
        controllerHeight = GUI_DEFAULT_PANEL_CONTROLLER_HEIGHT;
        
        tOsc = new ofxControlToggle("osc", &bOsc, this, &ofxControlPanel::eventToggleOscManager);
        tSeq = new ofxControlToggle("seq", &bSeq, this, &ofxControlPanel::eventToggleSequencer);
        tXml = new ofxControlToggle("xml", &bXml, this, &ofxControlPanel::eventTogglePresets);
        tOsc->setParent(this);
        tSeq->setParent(this);
        tXml->setParent(this);
        tOsc->setAutoUpdate(false);
        tSeq->setAutoUpdate(false);
        tXml->setAutoUpdate(false);
        tOsc->setAutoDraw(false);
        tSeq->setAutoDraw(false);
        tXml->setAutoDraw(false);
        
        meta = new ofxControlWidget("Meta");
        meta->setParent(this);
        meta->setPosition(rectangle.x + rectangle.width + 4, rectangle.y);
        meta->setAutoUpdate(false);
        meta->setAutoDraw(false);
        meta->addButton("save me", this, &ofxControlPanel::savePresetPrompt);
        
        menuPresets = meta->addMenu("load preset", this, &ofxControlPanel::loadPresetPrompt);
        
        
        refreshPresetMenu();
    }
}

void ofxControlPanel::refreshPresetMenu()
{
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    //
    
    return;
    
    /*
    menuPresets->clearToggles();
    
    ofDirectory dir;
    dir.allowExt("xml");
    dir.open("presets/"+getName()+"/");
    dir.listDir();
    vector<string> presetList;
    
    cout << "get -> "<<("presets/"+getName()+"/" )<<" " << dir.size() << endl;
    
    for(int i = 0; i < dir.size(); i++)
    {
        presetList.push_back(dir.getName(i));
        cout << " == toggle " << dir.getName(i) << endl;
        GuiToggle *temp = menuPresets->addToggle(dir.getName(i));
        
        cout << " -=== " << ofRectangle(temp->getRectangle()) << endl;
    }
     */
}

void ofxControlPanel::disableControlRow()
{
    if (controlRow)
    {
        controlRow = false;
        controllerHeight = 0;
        
        bOsc = false;
        bSeq = false;
        bXml = false;

        delete tOsc;
        delete tSeq;
        delete tXml;

        delete menuPresets;
        
        //
        //
        //
        //
        //
        //
        //
        // this is causing runtime error
        //delete meta;

        if (sequencerMade) {
            delete sequencer;
            sequencerMade = false;
        }
        if (oscManagerMade)
        {
            delete oscManager;
            oscManagerMade = false;
        }
    }
}

void ofxControlPanel::createSequencer()
{
    sequencer = new ofxControlSequencer(getName()+" sequencer", this, GUI_DEFAULT_SEQUENCER_NUMCOLS);
    sequencer->setParent(this);
    sequencer->setAutoUpdate(false);
    sequencer->setAutoDraw(false);
    sequencerMade = true;
}

void ofxControlPanel::createOscManager(ofxControlOscManager *osc)
{
    oscManager = new ofxControlOscManagerPanel(getName()+" oscManager", this, osc);
    oscManager->setParent(this);
    oscManager->setAutoUpdate(false);
    oscManager->setAutoDraw(false);
    oscManagerMade = true;
    setupGuiPositions();
}

void ofxControlPanel::setupGuiPositions()
{
    ofxControlWidget::setupGuiPositions();
    if (controlRow)
    {
        if (active)
        {
            if (oscManagerMade)
            {
                int width = rectangle.width / 3.0;
                tOsc->setPosition(rectangle.x + 4, rectangle.y + headerHeight + 5);
                tSeq->setPosition(rectangle.x + 4 + width, rectangle.y + headerHeight + 5);
                tXml->setPosition(rectangle.x + 4 + 2 * width, rectangle.y + headerHeight + 5);
                tOsc->setSize(width - 8, 15);
                tSeq->setSize(width - 8, 15);
                tXml->setSize(width - 8, 15);
            }
            else
            {
                int width = rectangle.width / 2.0;
                tOsc->setRectangle(0, 0, 0, 0);
                tSeq->setPosition(rectangle.x + 4, rectangle.y + headerHeight + 5);
                tXml->setPosition(rectangle.x + 4 + width, rectangle.y + headerHeight + 5);
                tSeq->setSize(width - 8, 15);
                tXml->setSize(width - 8, 15);
            }
        }
        else
        {
            tOsc->setRectangle(0, 0, 0, 0);
            tSeq->setRectangle(0, 0, 0, 0);
            tXml->setRectangle(0, 0, 0, 0);
        }
    }
    
    if (sequencerMade) {
        sequencer->setupGuiPositions();
    }
    if (oscManagerMade) {
        oscManager->setupGuiPositions();
    }
}

void ofxControlPanel::update()
{
    if (!active) {
        return;
    }
    
    ofxControlWidget::update();
    if (controlRow)
    {
        if (oscManagerMade) tOsc->update();
        tSeq->update();
        tXml->update();
    }
    if (sequencerMade) {
        sequencer->update();
    }
    if (oscManagerMade) {
        oscManager->update();
    }
    if (bXml) {
        meta->update();
    }
}

void ofxControlPanel::draw()
{    
    if (!active) {
        return;
    }

    ofPushStyle();
    ofSetLineWidth(1);
    ofSetCircleResolution(16);

    ofxControlWidget::draw();
    
    if (!getCollapsed())
    {
        ofPushStyle();
        ofSetColor(255, 50);
        ofRect(rectangle.x, rectangle.y + headerHeight, rectangle.width, controllerHeight);
        ofPopStyle();
        
        if (controlRow)
        {
            if (oscManagerMade) tOsc->draw();
            tSeq->draw();
            tXml->draw();
        }
        
        if (bOsc && oscManagerMade) {
            oscManager->draw();
        }

        if (bSeq && sequencerMade) {
            sequencer->draw();
        }
        
        if (bXml) {
            meta->draw();
        }
    }
    
    ofPopStyle();
}

bool ofxControlPanel::mouseMoved(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (oscManagerMade && tOsc->mouseMoved(mouseX, mouseY)) return true;
        if (tSeq->mouseMoved(mouseX, mouseY)) return true;
        if (tXml->mouseMoved(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mouseMoved(mouseX, mouseY)) return true;
    }
    if (bOsc) {
        if (oscManager->mouseMoved(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mouseMoved(mouseX, mouseY)) return true;
    }
    return ofxControlWidget::mouseMoved(mouseX, mouseY);
}

bool ofxControlPanel::mousePressed(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (oscManagerMade && tOsc->mousePressed(mouseX, mouseY)) return true;
        if (tSeq->mousePressed(mouseX, mouseY)) return true;
        if (tXml->mousePressed(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mousePressed(mouseX, mouseY)) return true;
    }
    if (bOsc) {
        if (oscManager->mousePressed(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mousePressed(mouseX, mouseY)) return true;
    }
    return ofxControlWidget::mousePressed(mouseX, mouseY);
}

bool ofxControlPanel::mouseDragged(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (oscManagerMade && tOsc->mouseDragged(mouseX, mouseY)) return true;
        if (tSeq->mouseDragged(mouseX, mouseY)) return true;
        if (tXml->mouseDragged(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mouseDragged(mouseX, mouseY)) return true;
    }
    if (bOsc) {
        if (oscManager->mouseDragged(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mouseDragged(mouseX, mouseY)) return true;
    }
    return ofxControlWidget::mouseDragged(mouseX, mouseY);
}

bool ofxControlPanel::mouseReleased(int mouseX, int mouseY)
{
    if (controlRow)
    {
        if (oscManagerMade && tOsc->mouseReleased(mouseX, mouseY)) return true;
        if (tSeq->mouseReleased(mouseX, mouseY)) return true;
        if (tXml->mouseReleased(mouseX, mouseY)) return true;
    }
    if (bSeq) {
        if (sequencer->mouseReleased(mouseX, mouseY)) return true;
    }
    if (bOsc) {
        if (oscManager->mouseReleased(mouseX, mouseY)) return true;
    }
    if (bXml) {
        if (meta->mouseReleased(mouseX, mouseY)) return true;
    }
    return ofxControlWidget::mouseReleased(mouseX, mouseY);
}

bool ofxControlPanel::keyPressed(int key)
{
    if (controlRow)
    {
        if (oscManagerMade && tOsc->keyPressed(key)) return true;
        if (tSeq->keyPressed(key)) return true;
        if (tXml->keyPressed(key)) return true;
    }
    if (bSeq) {
        if (sequencer->keyPressed(key)) return true;
    }
    if (bOsc) {
        if (oscManager->keyPressed(key)) return true;
    }
    if (bXml) {
        if (meta->keyPressed(key)) return true;
    }
    return ofxControlWidget::keyPressed(key);
}

void ofxControlPanel::eventToggleSequencer(ofxControlButtonEventArgs &e)
{
    if (!sequencerMade) {
        createSequencer();
    }
    if (bSeq)
    {
        bOsc = false;
        bXml = false;
    }
}

void ofxControlPanel::eventToggleOscManager(ofxControlButtonEventArgs &e)
{
    if (bOsc)
    {
        bSeq = false;
        bXml = false;
    }
}

void ofxControlPanel::eventTogglePresets(ofxControlButtonEventArgs &e)
{
    if (bXml)
    {
        bOsc = false;
        bSeq = false;
    }
}

void ofxControlPanel::savePresetPrompt(ofxControlButtonEventArgs &e)
{
    string name = ofSystemTextBoxDialog("Preset name");
    savePreset(name);
}

void ofxControlPanel::loadPresetPrompt(ofxControlMenuEventArgs &e)
{
    loadPreset(e.toggle->getName());
}

void ofxControlPanel::savePreset(string name)
{
    ofXml xml;
    xml.addChild("Preset");
    xml.setTo("Preset");
    getXml(xml);
    xml.save("presets/"+getName()+"/"+name+".xml");
    menuPresets->addToggle(name);
}

void ofxControlPanel::loadPreset(string name)
{
    ofXml xml;
    xml.load("presets/"+getName()+"/"+name);
    xml.setTo("Preset");
    setFromXml(xml);
}

void ofxControlPanel::saveSequencerToXml(ofXml &xml)
{
    if (sequencerMade)
    {
        ofXml xmlSequencer;
        xmlSequencer.addChild("Sequencer");
        xmlSequencer.setTo("Sequencer");
        sequencer->getXml(xmlSequencer);
        xml.addXml(xmlSequencer);
    }
}

void ofxControlPanel::loadSequencerFromXml(ofXml &xml)
{
    if (xml.exists("Sequencer"))
    {
        xml.setTo("Sequencer");
        if (!sequencerMade) {
            createSequencer();
        }
        sequencer->setFromXml(xml);
        xml.setToParent();
    }
    else {
        ofLog(OF_LOG_ERROR, "No sequencer found in preset");
    }
}

void ofxControlPanel::getXml(ofXml &xml)
{
    ofxControlWidget::getXml(xml);
    if (sequencerMade) {
        sequencer->getXml(xml);
    }
}

void ofxControlPanel::setFromXml(ofXml &xml)
{
    ofxControlWidget::setFromXml(xml);
    if (xml.exists("Sequencer"))
    {
        if (!sequencerMade) {
            createSequencer();
        }
        sequencer->setFromXml(xml);
    }
    else {
        ofLog(OF_LOG_ERROR, "No sequencer found in preset");
    }
}

