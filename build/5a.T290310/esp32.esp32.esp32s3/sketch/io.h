#line 1 "/a0/usr/workdir/BKOS5a/io.h"
void io();
void io_reset_alert();
void io_set_defaults();
void io_set_defaults(int standaard);
void io_schakel(int knop);
void io_boot();
int io_output_status(byte);

int io_cnt;
int io_knoppen_cnt;
int io_sensoren_cnt;

byte aparaten [30]; // Gedetecteerde aparaten
int aparaten_cnt = 0; // aantal aparaten

unsigned long io_lost = 0;

bool io_runned = false;

int io_alert_cnt;
int io_alert_app;
int io_diepcheck_cnt = 0;
int* io_diepcheck_pins;
int io_diepcheck_pins_cnt = 0;
bool io_diepcheck = true;
// BKOS5a: IO polling interval - instelbaar (milliseconden)
// Bij wijziging wordt io_now=true gezet voor directe uitvoering
#ifndef IO_INTERVAL_MS
  #define IO_INTERVAL_MS 30000UL  // Standaard: 30 seconden
#endif
long io_timer = IO_INTERVAL_MS;  // Aantal milliseconden tussen de checks (0 = niet checken)
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

String io_handleiding[6];


// /* STANDAARD WAARDEN
//  *
//  * De volgende parameters zijn enkel de standaard waarden
//  * Dit wordt gebruikt zolang de SD kaartlezer niet in gebruik is
//  * En mogelijk ook later
//  */

// *io_objecten[] = {3, 3, 3, 3, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0,
//                3, 3, 3, 0, 3, 0, 3, 3};

// *io_input[]    = {0, 0, 0, 0, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0};

// *io_namen    = {"USB       ", "230V      ", "?         ", "?         ", "?         ", "?         ", "?         ", "?         ",
//               "?         ", "?         ", "?         ", "?         ", "?         ", "?         ", "?         ", "?         ",
//               "USB       ", "230V      ", "?         ", "?         ", "?         ", "?         ", "?         ", "?         "};

// *io_gewijzigd = {0, 0, 0, 0, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0};

// *io_alert    = {0, 0, 0, 0, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0,
//                0, 0, 0, 0, 0, 0, 0, 0};

// BKOS5a: NVS state persistentie
#ifdef ESP32
#include <Preferences.h>
#define IO_WRITE_DELAY_MS 30000UL
extern bool io_state_dirty;
extern unsigned long io_state_last_write;
void io_state_opslaan();
void io_state_laden();
void io_state_check();
#endif
// Pico: gebruik LittleFS + JSON als alternatief
