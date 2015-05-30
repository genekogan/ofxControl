#include "ofxControlMultiSlider.h"


ofxControlMultiSliderBase::ofxControlMultiSliderBase(string name) : ofxControlMultiElement(name)
{
    marginY = GUI_DEFAULT_MARGIN_Y_INNER;
    setHeader(getName());
}
