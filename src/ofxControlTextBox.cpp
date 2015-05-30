#include "ofxControlTextBox.h"

ofxControlTextBox::ofxControlTextBox(ofxControlParameter<string> *parameter) : ofxControlElement(parameter->getName())
{
    this->parameter = parameter;
    setupTextBox();
}

ofxControlTextBox::ofxControlTextBox(string name, string *value) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<string>(name, value);
    setupTextBox();
}

ofxControlTextBox::ofxControlTextBox(string name) : ofxControlElement(name)
{
    parameter = new ofxControlParameter<string>(name, new string(name));
    setupTextBox();
}

ofxControlTextBox::~ofxControlTextBox()
{
    delete parameter;
}

void ofxControlTextBox::setupTextBox()
{
    setValue(parameter->get());
    stringHeight = ofBitmapStringGetBoundingBox(name, 0, 0).height;
    setLeftJustified(true);
}

void ofxControlTextBox::updateParameterOscAddress()
{
    parameter->setOscAddress(getAddress());
}

void ofxControlTextBox::setValue(string value, bool sendChangeNotification)
{
    string previous = parameter->get();
    parameter->set(value);
    stringWidth = ofBitmapStringGetBoundingBox(parameter->get(), 0, 0).width;
    if (sendChangeNotification && (value != previous))
    {
        ofxControlTextBoxEventArgs args(this, parameter->get());
        ofNotifyEvent(textBoxEvent, args, this);
    }
}

void ofxControlTextBox::getParameters(vector<ofxControlParameterBase*> & parameters)
{
    parameters.push_back(parameter);
}

string ofxControlTextBox::getValue()
{
    return parameter->get();
}

void ofxControlTextBox::update()
{
    
}

void ofxControlTextBox::draw()
{
	ofPushStyle();
    
    ofFill();
    ofSetLineWidth(1);
    ofSetColor(colorBackground);
    ofRect(rectangle);
    
    ofNoFill();
    ofSetColor(colorOutline);
    ofRect(rectangle);
    
	if (mouseOver)
	{
	    ofNoFill();
	    ofSetLineWidth(2);
        ofSetColor(editing ? GUI_DEFAULT_COLOR_ACTIVE_EDIT : colorActive);
	    ofRect(rectangle);
        ofSetLineWidth(1);
	}
    
    ofSetColor(colorText);
    
    ofDrawBitmapString(parameter->get(),
                       rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)),
                       rectangle.y + 0.5 * (rectangle.height + 0.5 * stringHeight) + 1);
    if (editing)
    {
        int x = rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)) + GUI_DEFAULT_CHAR_WIDTH * cursorPosition;
        ofSetColor(GUI_DEFAULT_COLOR_ACTIVE_EDIT, fmodf(floor(2.0 * ofGetElapsedTimef()), 2.0) == 0.0 ? 255 : 0);
        ofSetLineWidth(1);
        ofLine(x, rectangle.y + 1, x, rectangle.y + rectangle.height - 2);
    }
    
	ofPopStyle();
}

bool ofxControlTextBox::mousePressed(int mouseX, int mouseY)
{
    ofxControlElement::mousePressed(mouseX, mouseY);
    if (mouseOver)
    {
        editing = true;
        cursorPosition = ofClamp(round((float) (mouseX - (rectangle.x + (leftJustified ? 4 : 0.5 * (rectangle.width - stringWidth)))) / GUI_DEFAULT_CHAR_WIDTH), 0, parameter->get().length());
        return true;
    }
    else
    {
        editing = false;
        return false;
    }
}

bool ofxControlTextBox::keyPressed(int key)
{
    ofxControlElement::keyPressed(key);
    
    if (mouseOver && key == OF_KEY_RETURN)
    {
        editing = !editing;
        return true;
    }
    
    if (editing)
    {
        if (key == OF_KEY_BACKSPACE)
        {
            if (cursorPosition > 0)
            {
                cursorPosition = ofClamp(cursorPosition - 1, 0, parameter->get().size());
                setValue(parameter->get().erase(cursorPosition, 1), true);
                return true;
            }
        }
        else if (key == OF_KEY_LEFT)
        {
            cursorPosition = ofClamp(cursorPosition - 1, 0, parameter->get().size());
            return true;
        }
        else if (key == OF_KEY_RIGHT)
        {
            cursorPosition = ofClamp(cursorPosition + 1, 0, parameter->get().size());
            return true;
        }
        else if (key > 31 && key < 127)
        {
            setValue(parameter->get().insert(cursorPosition, 1, key), true);
            cursorPosition++;
            return true;
        }
    }
    return false;
}

void ofxControlTextBox::getXml(ofXml &xml)
{
    xml.addValue<string>("Value", parameter->get());
}

void ofxControlTextBox::setFromXml(ofXml &xml)
{
    parameter->set(xml.getValue<string>("Value"));
}
