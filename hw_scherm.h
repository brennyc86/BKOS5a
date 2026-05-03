int tft_helderheid = 75;
long scherm_timer = 5;
bool tft_actief = true;

// Logo wordt hier al aangemaakt omdat ik in het basis scherm het logo wil tonen pagina groot als soort van welkomstscherm
byte bkos_logo_200_75[] = { 80, 40, 80, 0, 70, 60, 70, 0, 63, 74, 63, 0, 57, 86, 57, 0, 52, 20, 31, 45, 52, 0, 47, 21, 35, 50, 47, 0, 43, 23, 37, 54, 43, 0, 40, 24, 39, 31, 11, 15, 40, 0, 37, 25, 41, 30, 11, 19, 37, 0, 34, 27, 42, 29, 11, 20, 37, 0, 31, 29, 43, 28, 11, 24, 34, 0, 29, 30, 44, 27, 11, 28, 31, 0, 27, 32, 14, 22, 8, 26, 11, 31, 29, 0, 24, 34, 12, 25, 8, 25, 11, 34, 27, 0, 22, 36, 10, 27, 8, 24, 11, 38, 24, 0, 20, 37, 10, 28, 8, 23, 11, 41, 22, 0, 18, 39, 9, 29, 8, 22, 11, 44, 20, 0, 17, 39, 10, 29, 8, 21, 11, 47, 18, 0, 15, 41, 9, 30, 8, 20, 11, 50, 16, 0, 14, 42, 8, 31, 8, 19, 11, 52, 15, 0, 12, 43, 9, 31, 8, 18, 11, 55, 13, 0, 11, 44, 9, 31, 8, 17, 11, 57, 12, 0, 10, 45, 9, 31, 8, 16, 11, 59, 11, 0, 9, 46, 9, 31, 8, 15, 11, 61, 10, 0, 8, 48, 8, 31, 8, 14, 11, 63, 9, 0, 7, 49, 8, 31, 8, 13, 11, 65, 8, 0, 6, 50, 9, 30, 8, 12, 11, 67, 7, 0, 5, 51, 9, 30, 8, 11, 11, 69, 6, 0, 4, 53, 9, 29, 8, 10, 11, 71, 5, 0, 3, 54, 10, 28, 8, 9, 11, 73, 4, 0, 3, 55, 10, 27, 8, 8, 11, 74, 4, 0, 2, 57, 11, 25, 8, 7, 11, 76, 3, 0, 2, 58, 12, 23, 8, 6, 11, 77, 3, 0, 1, 60, 42, 5, 11, 79, 2, 0, 1, 61, 41, 4, 13, 78, 2, 0, 1, 63, 39, 3, 15, 77, 2, 0, 0, 63, 40, 2, 17, 77, 1, 0, 0, 61, 42, 1, 18, 77, 1, 0, 0, 59, 64, 76, 1, 0, 0, 58, 55, 2, 9, 75, 1, 0, 0, 57, 55, 3, 10, 74, 1, 0, 1, 55, 13, 26, 16, 5, 9, 73, 2, 0, 1, 54, 11, 29, 15, 7, 9, 72, 2, 0, 1, 53, 11, 30, 14, 8, 10, 71, 2, 0, 2, 52, 10, 31, 13, 10, 10, 69, 3, 0, 2, 51, 11, 31, 12, 12, 9, 69, 3, 0, 3, 50, 10, 32, 11, 14, 9, 67, 4, 0, 3, 49, 11, 32, 10, 15, 10, 66, 4, 0, 4, 48, 10, 33, 9, 17, 9, 65, 5, 0, 5, 47, 10, 33, 8, 19, 9, 63, 6, 0, 6, 46, 10, 33, 8, 19, 10, 61, 7, 0, 7, 45, 10, 33, 8, 20, 9, 60, 8, 0, 8, 44, 10, 33, 8, 21, 9, 58, 9, 0, 9, 43, 10, 33, 8, 21, 10, 56, 10, 0, 10, 42, 11, 32, 8, 22, 10, 54, 11, 0, 12, 40, 11, 32, 8, 22, 10, 52, 13, 0, 13, 40, 10, 32, 8, 23, 10, 50, 14, 0, 15, 38, 11, 31, 8, 23, 11, 47, 16, 0, 16, 38, 11, 30, 8, 24, 10, 46, 17, 0, 18, 36, 12, 29, 8, 24, 11, 43, 19, 0, 20, 35, 13, 27, 8, 25, 11, 40, 21, 0, 22, 33, 15, 25, 8, 26, 10, 38, 23, 0, 25, 31, 47, 26, 11, 34, 26, 0, 27, 30, 46, 27, 11, 31, 28, 0, 29, 29, 45, 28, 10, 29, 30, 0, 32, 27, 44, 28, 11, 25, 33, 0, 35, 25, 43, 29, 11, 21, 36, 0, 38, 23, 42, 30, 10, 18, 39, 0, 41, 22, 40, 30, 11, 14, 42, 0, 45, 21, 37, 31, 11, 9, 46, 0, 50, 20, 33, 46, 51, 0, 55, 89, 56, 0, 61, 77, 62, 0, 68, 63, 69, 0, 78, 43, 79, 0};
void bkos_logo(int32_t x, int32_t y, int schaal, uint32_t kleur);
void bkos_logo(int32_t x, int32_t y, uint32_t kleur);



// 0 : Raspberry pi pico (2) + ESP8266 s01
// 1 : Raspberry pi pico (2)W
// 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
// 3 : ESP32 VROOM     (38 pin versie) 
// 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
// 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU

#if HARDWARE == 5
  #include <Arduino_GFX_Library.h>

  int TFT_HELDER_FACTOR = 2;

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


#if RESOLUTIE == 4880
  #define ILI9341_DRIVER

  #define SX_MIN    0
  #define SX_MAX  480
  #define SY_MIN    0
  #define SY_MAX  800

#endif


#if DP_DRIVER == 3
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


uint16_t kleur_groen = tft.color565(0,100,0);
uint16_t kleur_donker = kleur_groen;
uint16_t kleur_beige = tft.color565(235,235,100);
uint16_t kleur_licht = kleur_beige;


uint16_t kleur_zwart = tft.color565(0,0,0);
uint16_t kleur_wit = tft.color565(255,255,255);
uint16_t kleur_grijs = tft.color565(100,100,100);
uint16_t kleur_geel = tft.color565(200, 255, 0);
uint16_t kleur_oranje = tft.color565(255, 200, 0);
uint16_t kleur_rood = tft.color565(200, 0, 0);
uint16_t kleur_blauw = tft.color565(100, 100, 255);
uint16_t kleur_actief_rood = tft.color565(255, 0, 0);
uint16_t kleur_passief_rood = tft.color565(150, 0, 0);
uint16_t kleur_actief_groen = tft.color565(0, 255, 0);
uint16_t kleur_passief_groen = tft.color565(0, 150, 0);


uint16_t kleur_home_knop = kleur_donker;
uint16_t kleur_home_tekst = kleur_licht;


void tft_begin();
void tft_setup();
void tft_scherm_helderheid();