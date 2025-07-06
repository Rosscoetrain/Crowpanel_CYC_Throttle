This folder contains the display board driver files.

The naming convention for the file is:

Brand_Model_X.h

Where:
Brand = the brand of the board
Model = the model number/name for the board
X = the type of touchscreen R = resistive C = capacitive


A template for the file is included in Brand_Template_X.h.txt
To create a new template copy this file to a new file for your board and edit that file as required. The new filename should be as above.

The gfx class is defined in this file as well it is the last item in the file.


The functions in the file are.

This is the initiator for the class

class LGFX : public lgfx::LGFX_Device {
public:

  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;
  lgfx::Touch_GT911 _touch_instance;

  LGFX(void) {
  }
}

This is the function to set the screen brightness, it can be called from anywhere in the main source.

void setBrightness(byte brightness)
 {
    // enter the method to control your screen brightness
 }

This is a function to initialize any additional hardware on the display board.  It is called in the setup() function of the main source.

void initHardware()
 {

 }

