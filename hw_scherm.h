#if HARDWARE <= 1  // Pi pico
  #define TFT_BL   10
  #define TFT_CS   17 
  #define TFT_DC   15
  #define TFT_MISO 16
  #define TFT_MOSI 19
  #define TFT_SCK  18
  #define TFT_RST  14

  #define TOUCH_CS  13

#elif HARDWARE == 2  // CYD  (ESP32-2432S028)
  #define TFT_BL   21
  #define TFT_CS   15 
  #define TFT_DC    2
  #define TFT_MISO 12
  #define TFT_MOSI 13
  #define TFT_SCK  14
  #define TFT_RST  -1

  #define TOUCH_CS  33


#elif HARDWARE == 3  // ESP32 (38 pin WROOM32 devkit)
  #define TFT_BL   19
  #define TFT_CS    0 
  #define TFT_DC   23
  #define TFT_MISO 12
  #define TFT_MOSI 13
  #define TFT_SCK  14
  #define TFT_RST  17

  #define TOUCH_CS  22
  
#elif HARDWARE == 4
  #include <Arduino_GFX_Library.h>

  #define GFX_DEV_DEVICE ESP32_8048S043
  #define TFT_BL 2
  #define RGB_PANEL
  Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
      40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
      45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
      5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
      8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
      0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
      0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
      1 /* pclk_active_neg */, 14000000 /* prefer_speed */);
  Arduino_RGB_Display tft = Arduino_RGB_Display(
      800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#elif HARDWARE == 5
  #include <Arduino_GFX_Library.h>

  #define GFX_DEV_DEVICE ESP32_8048S070  // was 43
  #define TFT_BL 2
  #define RGB_PANEL
  Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
      41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
      14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
      9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
      15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */,
      0 /* hsync_polarity */, 210 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
      0 /* vsync_polarity */, 22 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
      1 /* pclk_active_neg */, 16000000 /* prefer_speed */);
  Arduino_RGB_Display tft = Arduino_RGB_Display(
      800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

#endif



#if RESOLUTIE == 2432
  #define ILI9341_DRIVER

  // # if HARDWARE == 2
  //   #define SX_MIN    0
  //   #define SX_MAX  320
  //   #define SY_MIN    0
  //   #define SY_MAX  240
  // #else
  #define SX_MIN    0
  #define SX_MAX  240
  #define SY_MIN    0
  #define SY_MAX  320
  // #endif

#elif RESOLUTIE == 3248
  #define ILI9488_DRIVER

  #define SX_MIN    0
  #define SX_MAX  320
  #define SY_MIN    0
  #define SY_MAX  480

#elif RESOLUTIE == 4880
  #define ILI9341_DRIVER

  #define SX_MIN    0
  #define SX_MAX  480
  #define SY_MIN    0
  #define SY_MAX  800

#endif


#if DP_DRIVER == 1
  #include <Adafruit_GFX.h>
  #include <Adafruit_ILI9341.h>

  Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

#elif DP_DRIVER == 2

  #include <TFT_eSPI.h>
  TFT_eSPI tft = TFT_eSPI(SX_MAX, SY_MAX);

#elif DP_DRIVER == 3
  #if !defined(GFX_DEV_DEVICE)
    #include <Arduino_GFX_Library.h>
    #if defined(ESP32)
      Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI);
    #elif defined(PI_PICO)
      Arduino_DataBus *bus = new Arduino_RPiPicoSPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI);
    #endif

    // #if RESOLUTIE == 3248
    //   Arduino_GFX *tft = new Arduino_ILI9488(bus, TFT_RST, 0, false);
    // #else
    Arduino_GFX tft = Arduino_ILI9341(bus, TFT_RST, 0, false);
    // #endif
  #endif

#endif

void tft_begin();
void tft_setup();