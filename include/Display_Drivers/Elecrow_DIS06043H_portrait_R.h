/*
 *
 * @file Elecrow_DIS06043H_portrait_C.h
 * 
 * 
 *
 */


#ifndef ELECROW_DIS06043H_PORTRAIT_R_H
#define ELECROW_DIS06043H_PORTRAIT_R_H

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

#define LCD_H_RES 480
#define LCD_V_RES 272


/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/

#define GFX_BL 2 // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define ROTATION 1
#define SCREEN_WIDTH 272          //Using EEZ Orientation
#define SCREEN_HEIGHT 480
#define DISPLAY_WIDTH 480         //Physical Display Properties ex rotation
#define DISPLAY_HEIGHT 272
#define AUTO_FLUSH true

#define NAME_COL_WIDTH 210

const uint8_t TFT_BACKLIGHT = 246;


// Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//     40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
//     45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
//     5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
//     8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
//     0 /* hsync_polarity */, 1 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
//     0 /* vsync_polarity */, 3 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
//     1 /* pclk_active_neg */, 9000000 /* prefer_speed */);



//
// From Elecrow website
// https://www.elecrow.com/wiki/esp32-display-432727-intelligent-touch-screen-wi-fi26ble-480272-hmi-display.html
//
//Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
//  GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
//  40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
//  45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
//  5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
//  8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
//);

//Arduino_RGB_Display *gfx = new Arduino_RGB_Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, rgbpanel, ROTATION , AUTO_FLUSH);

//Arduino_DataBus *bus = create_default_Arduino_DataBus();

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#define screenWidth   480
#define screenHeight  272

class LGFX : public lgfx::LGFX_Device
{
public:
    lgfx::Bus_RGB _bus_instance;
    lgfx::Panel_RGB _panel_instance;
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_XPT2046 _touch_instance;
    LGFX(void)
    {
        {
            auto cfg = _panel_instance.config();
            cfg.memory_width = screenWidth;
            cfg.memory_height = screenHeight;
            cfg.panel_width = screenWidth;
            cfg.panel_height = screenHeight;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            _panel_instance.config(cfg);
        }

        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;

            cfg.pin_d0 = GPIO_NUM_8;  // B0
            cfg.pin_d1 = GPIO_NUM_3;  // B1
            cfg.pin_d2 = GPIO_NUM_46; // B2
            cfg.pin_d3 = GPIO_NUM_9;  // B3
            cfg.pin_d4 = GPIO_NUM_1;  // B4

            cfg.pin_d5 = GPIO_NUM_5;  // G0
            cfg.pin_d6 = GPIO_NUM_6;  // G1
            cfg.pin_d7 = GPIO_NUM_7;  // G2
            cfg.pin_d8 = GPIO_NUM_15; // G3
            cfg.pin_d9 = GPIO_NUM_16; // G4
            cfg.pin_d10 = GPIO_NUM_4; // G5

            cfg.pin_d11 = GPIO_NUM_45; // R0
            cfg.pin_d12 = GPIO_NUM_48; // R1
            cfg.pin_d13 = GPIO_NUM_47; // R2
            cfg.pin_d14 = GPIO_NUM_21; // R3
            cfg.pin_d15 = GPIO_NUM_14; // R4

            cfg.pin_henable = GPIO_NUM_40;
            cfg.pin_vsync = GPIO_NUM_41;
            cfg.pin_hsync = GPIO_NUM_39;
            cfg.pin_pclk = GPIO_NUM_42;
            cfg.freq_write = 8000000;

            cfg.hsync_polarity    = 0;
            cfg.hsync_front_porch = 8;
            cfg.hsync_pulse_width = 4;
            cfg.hsync_back_porch  = 43;

            cfg.vsync_polarity    = 0;
            cfg.vsync_front_porch = 8;
            cfg.vsync_pulse_width = 4;
            cfg.vsync_back_porch  = 12;

            cfg.pclk_active_neg = 1;
            cfg.de_idle_high = 0;
            cfg.pclk_idle_high = 0;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _light_instance.config();
            cfg.pin_bl = GPIO_NUM_2;
            _light_instance.config(cfg);
            _panel_instance.light(&_light_instance);
        }

        {
            auto touch_cfg = _touch_instance.config();
            touch_cfg.x_min      = 100;    // タッチスクリーンから得られる最小のX値(生の値)
            touch_cfg.x_max      = 4000;  // タッチスクリーンから得られる最大のX値(生の値)
            touch_cfg.y_min      = 100;    // タッチスクリーンから得られる最小のY値(生の値)
            touch_cfg.y_max      = 4000;  // タッチスクリーンから得られる最大のY値(生の値)
            touch_cfg.pin_int    = 36;   // INTが接続されているピン番号
            touch_cfg.bus_shared = true; // 画面と共通のバスを使用している場合 trueを設定
            touch_cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

            // SPI接続の場合
            touch_cfg.spi_host   = SPI2_HOST; //HSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
            touch_cfg.freq       = 1000000;     // SPIクロックを設定
            touch_cfg.pin_sclk   = GPIO_NUM_12;     // SCLKが接続されているピン番号
            touch_cfg.pin_mosi   = GPIO_NUM_11;     // MOSIが接続されているピン番号
            touch_cfg.pin_miso   = GPIO_NUM_13;     // MISOが接続されているピン番号
            touch_cfg.pin_cs     = GPIO_NUM_0;     //   CSが接続されているピン番号

            _touch_instance.config(touch_cfg);
            _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
        }
        setPanel(&_panel_instance);
    }
};


LGFX gfx;


//#include <xpt2046.h>
/*
int16_t touch_map_x1 = 4000;      //90;
int16_t touch_map_x2 = 0;      //1900;
int16_t touch_map_y1 = 0;      //3800;    //1863;  //1870;
int16_t touch_map_y2 = 4000;      //400;     //120; //180;
*/
/*
bool touch_swap_xy = false;
int16_t touch_map_x1 = 190;   //12;  //297;
int16_t touch_map_x2 = 1880;    //1850;  //3825;
int16_t touch_map_y1 = 1910;   //80;  //3860;
int16_t touch_map_y2 = 110;    //1800;  //171;

#define TOUCHSCREEN_SCLK_PIN (12)
#define TOUCHSCREEN_MISO_PIN (13)
#define TOUCHSCREEN_MOSI_PIN (11)
#define TOUCHSCREEN_CS_PIN   (0)                // (38)     // RT
#define TOUCHSCREEN_IRQ_PIN  (36)               // (18)     // RT

XPT2046 ts = XPT2046(SPI, TOUCHSCREEN_CS_PIN, TOUCHSCREEN_IRQ_PIN);

//Initialize the Touch Controller
void initTouch()
{
  SPI.begin(TOUCHSCREEN_SCLK_PIN, TOUCHSCREEN_MISO_PIN, TOUCHSCREEN_MOSI_PIN);
  ts.begin(SCREEN_WIDTH, SCREEN_HEIGHT);
//  ts.setCal(touch_map_x1, touch_map_x2, touch_map_y1, touch_map_y2, DISPLAY_WIDTH, DISPLAY_HEIGHT); // Raw xmin, xmax, ymin, ymax, width, height
  ts.setRotation(ROTATION);
}

//create a touch object
void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
  if (ts.pressed()) 
  {
    data->state = LV_INDEV_STATE_PR;
//    data->point.x = ts.X(); //map(p.x, touch_map_x1, touch_map_x2, 1, SCREEN_WIDTH);
//    data->point.y = ts.Y(); //map(p.y, touch_map_y1, touch_map_y2, 1, SCREEN_HEIGHT);

    data->point.x = map(ts.RawY(), touch_map_x1, touch_map_x2, 1, SCREEN_WIDTH);
    data->point.y = map(ts.RawX(), touch_map_y1, touch_map_y2, 1, SCREEN_HEIGHT);
 //   Serial.printf("RawX: %d X: %d - RawY: %d Y: %d\n", ts.RawX(), data->point.x, ts.RawY(), data->point.y );
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}
*/


void setBacklight(uint8_t brightness)
 {
   
 }

void init_Hardware()
 {

 }


#endif  // ELECROW_DIS06043H_PORTRAIT_R_H
