#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

int io_cnt;
int io_knoppen_cnt;
int io_sensoren_cnt;

bool io_runned = false;


byte aparaten [30]; // Gedetecteerde aparaten
int aparaten_cnt; // aantal aparaten

unsigned long io_lost = 0;

int io_alert_cnt;
int io_alert_app;
int io_diepcheck_cnt = 0;
int* io_diepcheck_pins;
int io_diepcheck_pins_cnt = 0;
bool io_diepcheck = true;
long io_timer = 5000;  // Aantal miliseconde tussen de checks (0 = niet checken)
long io_gecheckt;   // Moment waarop de laatste check heeft plaatsgevonden.
bool io_wijziging = false;  // Of er een wijziging heeft plaatsgevonden
bool io_actief = false;

byte io_detectie[21];
byte* io_objecten;   // Wat zit er aangesloten op de poorten 0: niks, 1: knop, 2: sensor
byte* io_object_ruimte; // Overzicht met welke ruimte hoort bij het object
byte* io_output;  // Wat is de gewenste output 0: uit, 1: aan, 2: inv uit (hoog), 3: inv aan (laag), 4: geblokeerd (laag), 5: inv geblokeerd (hoog)
bool* io_input;   // Gemete status 0: laag, 1: hoog
bool* io_gewijzigd;   // Gemete status 0: laag, 1: hoog
byte* io_alert;   // Wanneer moet er een alert uit? 0: bij passief worden, 1: bij actief worden, 2: bij alle verandering, 3: nooit
char** io_namen; // Namen van de aangesloten objecten
bool* io_open_alert;
byte**** io_events; // het automatisch mee in en uitschakelen van aparaten met sensoren {{{{}, {}}, {{}, {}}}}
                   // [?](sensor nummer)[0/1]wordt passief/actief  [0/1]maakt passief/actief [10] lijst van max 10 mee te schakelen objecten standaard = -1


int* io_knoppen;  // Mapping voor de knoppen
int* io_sensoren; // Mapping voor de sensoren

int io_ruimtes_cnt;        // Aantal gedefinieerde ruimten
String* io_ruimtes_namen;  // Namen van de ruimten
byte*** io_ruimtes;        // Coordinaten van de ruimten {{cnt}, {x,y}, {x,y}, {x,y}, {x,y}}

// byte* io_objecten;


void hw_io_eeprom();

// 0 : Raspberry pi pico (2) + ESP8266 s01
// 1 : Raspberry pi pico (2)W
// 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
// 3 : ESP32 VROOM     (38 pin versie) 
// 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
// 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU


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


#include "io.h"