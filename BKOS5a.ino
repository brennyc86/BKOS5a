#include <SPI.h>

#define JSON_CONFIG_FILE "/bkos4_config.json"
bool shouldSaveConfig = false;

char bootnaam[25] = "bootnaam";
char haven[25]    = "haven";
char eigenaar[25] = "eigenaar";
char telefoon[25] = "telefoon";


// Selectie gebruikte hardware (tzt kijken of dit automatisch kan)
#define HARDWARE 5    // 0 : Raspberry pi pico (2) + ESP8266 s01
                      // 1 : Raspberry pi pico (2)W
                      // 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
                      // 3 : ESP32 VROOM     (38 pin versie) 
                      // 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
                      // 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU

// #define scherm_ili 9341  //  9341  of  9488
#define RESOLUTIE 4880   //  2432  of  3248 4880

#define ORIENTATIE 0 // 1 staand, 0 liggend, 2 liggend (brede kant onder... werkt nog niet naar behoren)

#define use_freeRTOS 1  // 1 voor het gebruik fan freeRTOS

const unsigned long ota_git_interval = 5 * 60 * 1000;  // 5 minuten in milliseconds
unsigned long ota_git_check = 0;
const unsigned long ota_wifi_interval = 3  * 1000;  // 5 seconde in milliseconds
unsigned long ota_wifi_check = 0;

int SCRIPT_RESOLUTIE = 2432;
volatile bool io_now = false;  // BKOS5a: volatile voor cross-core zichtbaarheid
bool updaten = false;

#include "hardware.h"
#include "diverse.h"
#include "BKOS.h"

// void ioLoop();
// void guiLoop();

void setup() {
  Serial.begin(9600);

  tft_setup();

  tft.fillScreen(kleur_zwart);
  tft.println("WiFiManager starten...");
  wifimanager();
  
  tft.fillScreen(kleur_zwart);
  tft.setCursor(0, 10);
  tft.setTextColor(kleur_wit);
  tft.println("boot...");
 
  // Note: The Adafruit librarys is not setting the backlight on, so we need to do that in code', flash size: 16mb, PSRAM: OPI PSRAM
  BKOS_boot();
  tft.println("boot done");
  
  digitalWrite(TFT_BL, HIGH);

  scherm_touched = millis();


#if use_freeRTOS == 1
  tft.println("");
  tft.println("start ioTask");
  // delay(500);
  xTaskCreatePinnedToCore(ioTask,
    "IO",
    2048,
    NULL,
    1,
    NULL,
    1);

  tft.println("startwifiTask");
  xTaskCreatePinnedToCore(wifiTask,
    "WIFI",
    20480,
    NULL,
    1,
    NULL,
    1);
  
  #else
    ota_setup(true);

  #endif
  
  tft.println("done");
  tft.println("exit setup");
  

}


void wifiLoop() {
  if (millis() > ota_git_check + ota_git_interval) {
    ota_wifi_update();
    // ota_git_update();
    ota_git_check = millis();
  } else if (millis() > ota_wifi_check + ota_wifi_interval){
    ota_wifi_update();
    ota_wifi_check = millis();
  }
}


void ioLoop(){
  // BKOS5a: io_now = directe actie (bijv. na schakelaar), altijd uitvoeren
  // Periodieke check op basis van io_timer (standaard 30s)
  if (io_now) {
    io_now = false;
    io();
    delay(50);
    io();
    // BKOS5a fix: io_gecheckt NIET resetten na directe actie
    // Periodieke timer loopt onafhankelijk door
    // Meerdere schakelacties snel achter elkaar: altijd direct uitgevoerd via io_now
  } else if ((io_gecheckt + io_timer) < millis()) {
    io();
    delay(50);
    io();
    io_gecheckt = millis();
  }
  #ifdef ESP32
  io_state_check(); // BKOS5a: NVS delayed write
  #endif
}

void guiLoop(){
  
  if (!updaten) {
    if (scherm_actief) {
      ts_begin();
      if (ts_touched()) {
        // BKOS5a: Touch debounce - voorkom dubbele registraties
        unsigned long nu = millis();
        if (nu - laatste_touch_tijd >= TOUCH_DEBOUNCE_MS) {
          laatste_touch_tijd = nu;
          scherm_touched = nu;
          actieve_touch = true;
          ts_x = touch_x();
          ts_y = touch_y();
        } else {
          actieve_touch = false;
        }
        // fillCircle(ts_x, ts_y, 10, kleur_wit);
      } else {
        actieve_touch = false;
      }

      unsigned long verstreken = millis() - scherm_touched;
      unsigned long timeout_ms = (unsigned long)scherm_timer * 1000UL;
      if ((millis() > scherm_touched + timeout_ms) || (millis() < scherm_touched)) {
        scherm_actief = false;
        scherm_waarschuwing_actief = false;
        digitalWrite(TFT_BL, LOW);
      } else {
        // BKOS5a: Schermtimer waarschuwing - rode rand 10s voor timeout
        if (timeout_ms > 5000UL && verstreken >= timeout_ms - 5000UL) {  // BKOS5a: waarschuwing 5s voor timeout
          if (!scherm_waarschuwing_actief) {
            scherm_waarschuwing_actief = true;
            scherm_timeout_waarschuwing();
          }
        } else {
          if (scherm_waarschuwing_actief) {
            scherm_waarschuwing_actief = false;
            scherm_bouwen = true;  // hertekenenen
          }
        }
        app_uitvoeren();
        if (millis() > klok_getekend + 5000) {
          header_plaatsen();
        }
      }

    } else {
      ts_begin();
      if (ts_touched()) {
        ts_begin();
        while (ts_touched()) {
          delay(50);
          ts_begin();
        }
        delay(250);
        digitalWrite(TFT_BL, HIGH);
        scherm_actief = true;
        delay(250);
        ts_begin();
        scherm_touched = millis();
        actieve_touch = false;
        // BKOS5a: altijd hertekenen na wake-up (rode rand weg + scherm fris)
        scherm_waarschuwing_actief = false;
        scherm_bouwen = true;  // altijd hertekenen na wake-up
      }
    }
  }
}


#if use_freeRTOS == 1

void ioTask(void* parameters){
  
  for (;;) {
    ioLoop();
    delay(50);
  }
}

void wifiTask(void* parameters){
  ota_setup(false);
  for (;;) {
    wifiLoop();
    delay(50);
  }
}

void guiTask(void* parameters){
  for (;;){
    guiLoop();
    delay(50);
  }
}

void loop() {
  guiLoop();
  // if (io_now) {
  //   if (io_actief){
  //     tft.fillCircle(15, 15, 12, tft.color565(0,0,255));
  //   } else {
  //     tft.fillCircle(15, 15, 12, tft.color565(255,0,0));
  //   }
  // } else if (io_actief) {
  //   tft.fillCircle(15, 15, 12, tft.color565(255, 0,255));
  // } else {
  //   tft.fillCircle(15, 15, 12, tft.color565(0, 255,0));
  // }
  delay(50);
}



#else
void loop(void) {
  ioLoop();
  guiLoop();
  wifiLoop();
  delay(50);
}
#endif// Last GitHub push: $(date +"%Y-%m-%d %H:%M:%S")
