#pragma once

#include "ofBitmapFont.h"
#include "ofxControlParameter.h"
#include "ofxControlElement.h"


class ofxControlTextBox;

struct ofxControlTextBoxEventArgs
{
    ofxControlTextBox *textBox;
    string value;
    
    ofxControlTextBoxEventArgs(ofxControlTextBox *textBox, string value)
    {
        this->textBox = textBox;
        this->value = value;
    }
};


class ofxControlTextBox : public ofxControlElement
{
public:
    
    template <typename L, typename M>
    ofxControlTextBox(ofxControlParameter<string> *parameter, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlTextBox(string name, string *value, L *listener, M method);
    
    template <typename L, typename M>
    ofxControlTextBox(string name, L *listener, M method);
    
    ofxControlTextBox(ofxControlParameter<string> *parameter);
    ofxControlTextBox(string name, string *value);
    ofxControlTextBox(string name);
    
    virtual ~ofxControlTextBox();
    
    void setLeftJustified(bool leftJustified) {this->leftJustified = leftJustified;}
    
    void getParameters(vector<ofxControlParameterBase*> & parameters);
    string getValue();
    void setValue(string value, bool sendChangeNotification=false);
    
    void update();
    void draw();
    
    bool mousePressed(int mouseX, int mouseY);
    bool keyPressed(int key);
    
    void getXml(ofXml &xml);
    void setFromXml(ofXml &xml);
    
    ofEvent<ofxControlTextBoxEventArgs> textBoxEvent;
    
protected:
    
    void setupTextBox();
    void updateParameterOscAddress();
    
    ofxControlParameter<string> *parameter;
    int cursorPosition;
    bool editing;
    float stringWidth, stringHeight;
    bool leftJustified;
};

template <typename L, typename M>
ofxControlTextBox::ofxControlTextBox(ofxControlParameter<string> *parameter, L *listener, M method) : ofxControlElement(parameter->getName())
{
    this->parameter = parameter;
    setupTextBox();
    ofAddListener(textBoxEvent, listener, method);
}

template <typename L, typename M>
ofxControlTextBox::ofxControlTextBox(string name, string *value, L *listener, M method) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<string>(name, value);
    setupTextBox();
    ofAddListener(textBoxEvent, listener, method);
}

template <typename L, typename M>
ofxControlTextBox::ofxControlTextBox(string name, L *listener, M method) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<string>(name, new string(name));
    setupTextBox();
    ofAddListener(textBoxEvent, listener, method);
}
