/*
 *
 *  Elecrow wifi throttle.
 *  Copyright 2025 by Ross Scanlon iam@rosscoe.com
 * 
 *  Base on the great work of Norman Halland (NormHal@gmail.com) and others in the
 *  Cheap Yellow Controller wifi throttle.
 *
 *  This uses many parts from the Cheap Yellow Controller and those files are anotated as such in the copyright.
 * 
 *  This is for the following Elecrow Crowpanel display boards;
 * 
 *  7inch 800x480 Advanced  tested working
 *  7inch 800x480 Basic     in development
 *  4.3 inch 320x240 Basic  in development
 * 
 * 
*/

/*
 *  add your display driver details as an #elif before the #endif at the end of these directives.
 *
 */


#ifdef ESP32DIS02170A            // Elecrow DIS02170A 7in 800x480 advanced
#include "Display_Drivers/Elecrow_DIS02170A_C.h"
#elif ESP32DIS02170A_LS
#include "Display_Drivers/Elecrow_DIS02170A_landscape_C.h"
#elif ESP32DIS0604R              // Elecrow DIS0604A 4.3in 320x240
//#include "Display_Drivers/DIS0604R_LovyanGFX_R.h"
#elif ESP32DIS08070H             // Elecrow DIS08070A 7in 800x480 basic
#include "Display_Drivers/Elecrow_DIS08070H_C.h"
#endif


#include <Arduino.h>
#include <lvgl.h>
#include <SPI.h>

#include <stdbool.h>

#include <WiFi.h>

#include <DCCEXProtocol.h>


/*
 * This #ifdef EEZ is only here to provide a means to compile a test program using the standard SquareLine Studio files.
 *
 */

#ifdef EEZ

#include <CSV_Parser.h>
#include "FS.h"
#include <LittleFS.h>

#include "defines.h"
#include "vars.h"

#include "EEZ/Little_FS.h"


#ifdef RES8048
#include "EEZ/8048/actions.h"
#include "EEZ/8048/ui.h"
#include "EEZ/8048/screens.h"
#include "EEZ/8048/images.h"
#endif

#ifdef RES8048_LS
#include "EEZ/8048_landscape/actions.h"
#include "EEZ/8048_landscape/ui.h"
#include "EEZ/8048_landscape/screens.h"
#include "EEZ/8048_landscape/images.h"
#endif


#include "EEZ/functions.h"
#include "EEZ/Acc_Page.h"
#include "EEZ/Config_Page.h"
#include "EEZ/Edit_Acc_Page.h"
#include "EEZ/Throttle_Page.h"
#include "EEZ/Edit_Loco_Page.h"
#include "EEZ/General_Page.h"
#include "EEZ/Menu_Page.h"
#include "EEZ/Program_Page.h"
#include "EEZ/Roster_Page.h"
#include "EEZ/WiFi_Page.h"
#include "EEZ/Scan_Page.h"

#include "EEZ/callbacks.h"

#else
#include <Wire.h>
#include "SLS/ui.h"
#endif



/* Change to your screen resolution */
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
static lv_color_t *buf1;

uint16_t touch_x, touch_y;


//  Display refresh
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  if (gfx.getStartCount() > 0) {
    gfx.endWrite();
  }
  gfx.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::rgb565_t *)&color_p->full);

  lv_disp_flush_ready(disp);  //	Tell lvgl that the refresh is complete
}


//  Read touch
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
  data->state = LV_INDEV_STATE_REL;// The state of data existence when releasing the finger
  bool touched = gfx.getTouch( &touch_x, &touch_y );
  if (touched)
  {
    data->state = LV_INDEV_STATE_PR;

    //  Set coordinates
    data->point.x = touch_x;
    data->point.y = touch_y;
//    Serial.print("x = ");
//    Serial.println(touch_x);
//    Serial.print("y = ");
//    Serial.println(touch_y);
  }
}

void setup()
{
  Serial.begin(115200); 

  pinMode(19, OUTPUT);//uart1

  Wire.begin(15, 16);
  delay(100);

// Init additional display hardware.
  init_Hardware();


// Init Display
  if (!gfx.init())
   {
    Serial.println("gfx failed");
    while (1)
     {
      
     }
   }

  gfx.initDMA();

#ifdef EEZ
#ifndef ESP32DIS02170A_LS
  gfx.setRotation(1);
#endif
#endif

  gfx.startWrite();
  gfx.fillScreen(TFT_BLACK);

//  gfx.fillScreen(TFT_BLUE);
  delay(1000);

//  gfx.setTextColor(TFT_WHITE, TFT_BLUE);
//  gfx.setTextSize(4);
//  gfx.drawString("Wifi Throttle", 80, 200);
//  delay(1000);
  

  lv_init();
  // size_t buffer_size = sizeof(lv_color_t) * LCD_H_RES * LCD_V_RES;
  size_t buffer_size = sizeof(lv_color_t) * LCD_H_RES * LCD_V_RES * 4;
  buf = (lv_color_t *)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);
  buf1 = (lv_color_t *)heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);

  lv_disp_draw_buf_init(&draw_buf, buf, buf1, LCD_H_RES * LCD_V_RES);

  // Initialize display
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  // Change the following lines to your display resolution
  disp_drv.hor_res = LCD_H_RES;
  disp_drv.ver_res = LCD_V_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Initialize input device driver program
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  delay(100);
  gfx.fillScreen(TFT_BLACK);

// Init UI
  ui_init();

#ifdef EEZ
// callbacks init these are in the file callbacks.h
  init_callbacks();

  LittleFS.begin();
  delay(100);

  init_LittleFS();
#endif

  Serial.println( "Setup done" );
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(10);
}
