#line 1 "/a0/usr/workdir/BKOS5a/hardware.h"
#if HARDWARE == 0
  #define MCU_TYPE "RP PI PICO (2)"

  #define PI_PICO

  #if RESOLUTIE == 2432
    #define DP_DRIVER 1  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX (not niet helemaal geschikt voor deze configuratie)
  #else
    #define DP_DRIVER 2  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX (not niet helemaal geschikt voor deze configuratie)
  #endif
  #define TS_DRIVER 1  // Touch driver    1 = XPT2046, 2 = URTouch
  #define IO_METHODE 1 // IO aansluiting  1 = direct,  2 = via module


#elif HARDWARE == 1
  #define MCU_TYPE "RP PI PICO (2)W"

  #define PI_PICO
  #define PI_PICO_W

  #if RESOLUTIE == 2432
    #define DP_DRIVER 1  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX (not niet helemaal geschikt voor deze configuratie)
  #else
    #define DP_DRIVER 2  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX (not niet helemaal geschikt voor deze configuratie)
  #endif
  #define TS_DRIVER 1  // Touch driver    1 = XPT2046, 2 = URTouch
  #define IO_METHODE 1 // IO aansluiting  1 = direct,  2 = via module
  

#elif HARDWARE == 2
  #define CYD
  #define MCU_TYPE "ESP32-2432* (CYD)"

  #define DP_DRIVER 2  // Display driver  1 = ILI9341, 2 = TFT_eSPI
  #define TS_DRIVER 1  // Touch driver    1 = XPT2046, 2 = URTouch
  #define IO_METHODE 2 // IO aansluiting  1 = direct,  2 = via module


#elif HARDWARE == 3
  #define MCU_TYPE "ESP32"
  
  #define ESP32

  // LETOP, op deze MCU werkt de touch niet goed samen met de ILI9341 display driver.
  // De TST_eSPI driver werkt sowieso veel sneller en dus fijner, maar voor deze driver moet je wel in de
  // User_Setup.h verwijzingen zetten naar de juiste pinnen. Dit kan een uitdaging zijn.

  #define DP_DRIVER 2  // Display driver  1 = ILI9341, 2 = TFT_eSPI
  #define TS_DRIVER 1  // Touch driver    1 = XPT2046, 2 = URTouch
  #define IO_METHODE 1 // IO aansluiting  1 = direct,  2 = via module


#elif HARDWARE == 4
  #define MCU_TYPE "ESP32-8048S043C"
  #define ESP32

  #define DP_DRIVER 3  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX
  #define TS_DRIVER 3  // Touch driver    1 = XPT2046, 2 = URTouch, 3 = GT911
  #define IO_METHODE 2 // IO aansluiting  1 = direct,  2 = via module

#elif HARDWARE == 5
  #define MCU_TYPE "ESP32-8048S070C"
  #define ESP32

  #define DP_DRIVER 3  // Display driver  1 = ILI9341, 2 = TFT_eSPI, 3 = GFX
  #define TS_DRIVER 3  // Touch driver    1 = XPT2046, 2 = URTouch, 3 = GT911
  #define IO_METHODE 2 // IO aansluiting  1 = direct,  2 = via module
  
#endif

#if RESOLUTIE == 4880
  #if ORIENTATIE == 1
    #define ROTATIE 3
  #elif ORIENTATIE == 0
    #define ROTATIE 0
  #elif ORIENTATIE == 2
    #define ROTATIE 2
  #elif ORIENTATIE == 3
    #define ROTATIE 3
  #endif
#elif HARDWARE == 2
  #define ROTATIE 1
#else
  #define ROTATIE 0
#endif

// #define ROTATIE 0

#include "hw_scherm.h"
#include "hw_touch.h"
#include "hw_sd.h"
#include "hw_io.h"

