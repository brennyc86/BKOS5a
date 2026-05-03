// Selectie gebruikte hardware (tzt kijken of dit automatisch kan)
#define HARDWARE 5    // 0 : Raspberry pi pico (2) + ESP8266 s01
                      // 1 : Raspberry pi pico (2)W
                      // 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
                      // 3 : ESP32 VROOM     (38 pin versie) 
                      // 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
                      // 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU

#include "parameters.h"
#include "hardware.h"


void setup() {
  hw_setup();
  scherm_touched = millis();
}

void loop() {
  hw_loop();
  if (actieve_touch){
    tft_helderheid = tft_helderheid + 10;
    if (tft_helderheid > 100) {
      tft_helderheid = 5;
    }
    tft_scherm_helderheid();
    tft.fillRect(370, 350, 200, 100, kleur_zwart);
    tft.setCursor(375, 355);
    tft.print(tft_helderheid);
    tft.print("%   ");
    tft.print(tft_helderheid * TFT_HELDER_FACTOR);
    delay(50);
    while (actieve_touch) {
      ts_touched();
      delay(50);
    }
  }
  delay(50);
}