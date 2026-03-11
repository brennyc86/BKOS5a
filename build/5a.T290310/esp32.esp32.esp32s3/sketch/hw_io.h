#line 1 "/a0/usr/workdir/BKOS5a/hw_io.h"
// Selectie gebruikte hardware (tzt kijken of dit automatisch kan)
                      // 0 : Raspberry pi pico (2) + ESP8266 s01
                      // 1 : Raspberry pi pico (2)W
                      // 2 : ESP32-2432s028r (CYD, Cheap Yellow Display)
                      // 3 : ESP32 VROOM     (38 pin versie) 
                      // 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
                      // 5 : ESP32-8048S070C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU



#if HARDWARE <= 1
  #define HC_PCK 2
  #define HC_SCK 1
  #define HC_IN  0
  #define HC_UIT 3
  #define HC_ID  4

#elif HARDWARE == 2


#elif HARDWARE == 3
  #define HC_PCK 2
  #define HC_SCK 15
  #define HC_IN  35
  #define HC_UIT 4
  #define HC_ID  34

#elif HARDWARE == 4


#elif HARDWARE == 5


#endif
