#pragma once


#include "ofMain.h"

#include "ofxControlParameter.h"

#include "ofxControlConstants.h"

#include "ofxControlBase.h"
#include "ofxControlElement.h"

#include "ofxControlButton.h"

#include "ofxControlTextBox.h"

#include "ofxControlSlider.h"
#include "ofxControlRangeSlider.h"

#include "ofxControlMultiElement.h"
#include "ofxControlMultiSlider.h"
#include "ofxControlMultiRangeSlider.h"

#include "ofxControlColor.h"
#include "ofxControlMenu.h"
#include "ofxControl2dPad.h"

#include "ofxControlWidget.h"

#include "ofxControlPanel.h"

#include "ofxControlBpm.h"
#include "ofxControlSequence.h"
#include "ofxControlSequencer.h"

#include "Base64.h"
#include "ofxControlTouchOSC.h"

#include "ofxControlOscManager.h"
#include "ofxControlOscManagerPanel.h"
#include "ofxControlMidiSequencer.h"


/*
TO-DO
=====
object management
 - debug dynamically adding/removing widgets/elements
 - switch to shared_ptr
specific GuiElementEventArgs *L, M
style customization (color, size, font)
sortable menu widget
keyboard editing
slider warp
*/