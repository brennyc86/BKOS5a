
void hw_setup();

// 0 : Raspberry pi pico (2) + ESP8266 s01
// 1 : Raspberry pi pico (2)W
// 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
// 3 : ESP32 VROOM     (38 pin versie) 
// 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
// 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU

#if HARDWARE == 5
  #define RESOLUTIE 4880
  #define ORIENTATIE 0
  #define ROTATIE 0
  #define use_freeRTOS 1

  #define MCU_TYPE "ESP32-8048S070C"
  #define ESP32

  #define DP_DRIVER 3  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX
  #define TS_DRIVER 3  // Touch driver    1 = XPT2046, 2 = URTouch, 3 = GT911
  #define IO_METHODE 2 // IO aansluiting  1 = direct,  2 = via module

#else
  #define RESOLUTIE 2432
  #define ORIENTATIE 0
  #define use_freeRTOS 0

#endif

#include "hw_scherm.h"
#include "hw_touch.h"
#include "hw_io.h"
#include "wifi.h"
#include "ota.h"