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
x saving sequencer sets
examples
 - widget
 - panel
 - sequencer
object management
 - dynamically adding/removing widgets/elements
 - shared_ptr (http://www.umich.edu/~eecs381/handouts/C++11_smart_ptrs.pdf)
specific GuiElementEventArgs *L, M
subclass accessors
---------
examples
 - customized menu
style customization (color, size, font)
sortable menu widget
keyboard editing
slider warp
non-linked sequencer


OSC MANAGER
===========
 - osc in, osc out
 - generate touch osc layouts
 - osc summary interface
 
 
PRESETS
=======
 - parameters load/save xml
 - save/load interface
 
 
 
*/