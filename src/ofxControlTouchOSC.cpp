#include "ofxControlTouchOsc.h"



ofxControlTouchOscLabel* ofxControlTouchOscPage::addLabel(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscLabel *widget = new ofxControlTouchOscLabel(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscTime* ofxControlTouchOscPage::addTime(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscTime *widget = new ofxControlTouchOscTime(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscBattery* ofxControlTouchOscPage::addBattery(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscBattery *widget = new ofxControlTouchOscBattery(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscFader* ofxControlTouchOscPage::addFader(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscFader *widget = new ofxControlTouchOscFader(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscRotary* ofxControlTouchOscPage::addRotary(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscRotary *widget = new ofxControlTouchOscRotary(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscLed* ofxControlTouchOscPage::addLed(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscLed *widget = new ofxControlTouchOscLed(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscButton* ofxControlTouchOscPage::addButton(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscButton *widget = new ofxControlTouchOscButton(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscToggle* ofxControlTouchOscPage::addToggle(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscToggle *widget = new ofxControlTouchOscToggle(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscEncoder* ofxControlTouchOscPage::addEncoder(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscEncoder *widget = new ofxControlTouchOscEncoder(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscXy* ofxControlTouchOscPage::addXy(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscXy *widget = new ofxControlTouchOscXy(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscMultiPush* ofxControlTouchOscPage::addMultiPush(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscMultiPush *widget = new ofxControlTouchOscMultiPush(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscMultiToggle* ofxControlTouchOscPage::addMultiToggle(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscMultiToggle *widget = new ofxControlTouchOscMultiToggle(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscMultiFader* ofxControlTouchOscPage::addMultiFader(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscMultiFader *widget = new ofxControlTouchOscMultiFader(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscMultiXy* ofxControlTouchOscPage::addMultiXy(string name, float x, float y, float w, float h)
{
    ofxControlTouchOscMultiXy *widget = new ofxControlTouchOscMultiXy(name, x, y, w, h, defaultColor);
    widgets.push_back(widget);
    return widget;
}

ofxControlTouchOscPage::ofxControlTouchOscPage(string name, ofxControlTouchOscColor color)
{
    this->name = name;
    this->defaultColor = color;
}

void ofxControlTouchOscPage::setScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
}

void ofxControlTouchOscPage::setDefaultWidgetColor(ofxControlTouchOscColor color)
{
    this->defaultColor = color;
}

string ofxControlTouchOscPage::getXml()
{
    string b64name = base64_encode(reinterpret_cast<const unsigned char*>(name.c_str()), name.length());
    
    string xml;
    xml += "<tabpage name=\""+b64name+"\" scalef=\"0.0\" scalet=\"1.0\" >\n";
    for (int i=0; i<widgets.size(); i++) {
        widgets[i]->setScale(scaleX, scaleY);
        xml += widgets[i]->getXmlInner();
    }
    xml += "</tabpage>\n";
    return xml;
}

ofxControlTouchOsc::ofxControlTouchOsc()
{
    defaultColor = RED;
    customResolution = false;
}

ofxControlTouchOscPage* ofxControlTouchOsc::addPage(string name)
{
    ofxControlTouchOscPage *newPage = new ofxControlTouchOscPage(name, defaultColor);
    pages.push_back(newPage);
    return newPage;
}

void ofxControlTouchOsc::addPage(ofxControlTouchOscPage* newPage)
{
    pages.push_back(newPage);
}

string ofxControlTouchOsc::getXml()
{
    string xml;
    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    if (customResolution)
        xml += "<layout version=\"13\" mode=\"3\" w=\""+ofToString((int)scaleX)+"\" h=\""+ofToString((int)scaleY)+"\" orientation=\"horizontal\">";
    else
        xml += "<layout version=\"13\" mode=\"0\" orientation=\"horizontal\">";
    for (int i=0; i<pages.size(); i++) {
        pages[i]->setScale(scaleX, scaleY);
        xml += pages[i]->getXml();
    }
    xml += "</layout>";
    return xml;
}

void ofxControlTouchOsc::setDefaultColor(ofxControlTouchOscColor color)
{
    this->defaultColor = color;
}

void ofxControlTouchOsc::setScale(float scaleX, float scaleY)
{
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    customResolution = true;
}

void ofxControlTouchOsc::save(string name)
{
    ofFile file("index.xml", ofFile::WriteOnly);
    file << getXml();
    file.close();
    string cmd;
    cmd += "cd "+ofToString(ofToDataPath("")) + "; ";
    cmd += "zip -r "+name+".ofxControlTouchosc index.xml; ";
    cmd += "rm index.xml; ";
    cmd += "open "+name+".ofxControlTouchosc; ";
    ofSystem(cmd);
}

ofxControlTouchOscPage::~ofxControlTouchOscPage()
{
    for (int i=0; i<widgets.size(); i++) {
        delete widgets[i];
    }
}

ofxControlTouchOsc::~ofxControlTouchOsc()
{
    for (int i=0; i<pages.size(); i++) {
        delete pages[i];
    }
}