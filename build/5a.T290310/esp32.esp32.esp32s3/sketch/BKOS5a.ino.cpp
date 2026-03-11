#include <Arduino.h>
#line 1 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
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

#line 43 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void setup();
#line 99 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void wifiLoop();
#line 111 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void ioLoop();
#line 133 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void guiLoop();
#line 206 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void ioTask(void* parameters);
#line 214 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void wifiTask(void* parameters);
#line 222 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void guiTask(void* parameters);
#line 229 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
void loop();
#line 1 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur(String actie);
#line 82 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void bouw_exterieur();
#line 161 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur_teken_boot(int32_t x, int32_t y);
#line 243 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur_symbolen_verlichting(int32_t x, int32_t y);
#line 351 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void run_exterieur();
#line 381 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieurscherm_schakel(int knop);
#line 489 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur_verlichting_uit();
#line 501 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur_zeilverlichting();
#line 540 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur_motorverlichting();
#line 590 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur_ankerverlichting();
#line 629 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void interieur_verlichting();
#line 633 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void interieur_verlichting(bool schakelen);
#line 696 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
String charstrip(char invoer[]);
#line 2 "/a0/usr/workdir/BKOS5a/BKOS.ino"
void BKOS_boot();
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
void thuisscherm(String actie);
#line 24 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
void homescherm_knop(int knop_nummer);
#line 39 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
void alle_homescherm_knoppen_plaatsen();
#line 45 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
void bouw_thuisscherm();
#line 57 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
int klik_thuis(int x, int y);
#line 66 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
void run_thuisscherm();
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_SCHAKEL.ino"
void schakelscherm(String actie);
#line 37 "/a0/usr/workdir/BKOS5a/BKOS_SCHAKEL.ino"
void bouw_schakelscherm();
#line 81 "/a0/usr/workdir/BKOS5a/BKOS_SCHAKEL.ino"
void schakelscherm_indicator_bolletjes();
#line 91 "/a0/usr/workdir/BKOS5a/BKOS_SCHAKEL.ino"
void run_schakelscherm();
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void header_plaatsen(String headertitel);
#line 6 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void header_plaatsen();
#line 24 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void klok_update();
#line 28 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void klok_update(bool force);
#line 55 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void achtergrond(uint16_t kleur);
#line 74 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void overligger(uint16_t kleur, int hoogte, int dicht);
#line 91 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void overligger(uint16_t kleur, int hoogte);
#line 105 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void achtergrond(uint16_t kleur, bool plaats_home_knop);
#line 114 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void achtergrond(uint16_t kleur, bool plaats_home_knop, uint16_t home_knop_kleur);
#line 123 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void header_alert();
#line 132 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void header_alert(int icon_tekst_grootte);
#line 154 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void bouw_alert();
#line 162 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void bouw_alert(int icon_tekst_grootte);
#line 219 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void run_alert();
#line 261 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void klik_header(int x, int y);
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_logo.ino"
void bkos_logo(int32_t x, int32_t y, uint32_t kleur);
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_update.ino"
void update_center(String actie);
#line 21 "/a0/usr/workdir/BKOS5a/BKOS_update.ino"
void bouw_update();
#line 109 "/a0/usr/workdir/BKOS5a/BKOS_update.ino"
String fetchVersion4();
#line 122 "/a0/usr/workdir/BKOS5a/BKOS_update.ino"
void run_update();
#line 2 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void knop_feedback(int knop_nummer);
#line 17 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void knop_plaatsen(int knop_nummer);
#line 21 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void knop_plaatsen(int knop_nummer, bool tekst);
#line 25 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void knop_plaatsen(int knop_nummer, bool tekst, uint16_t basis_kleur);
#line 170 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void alle_knoppen_plaatsen();
#line 173 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void alle_knoppen_plaatsen(bool tekst);
#line 179 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
int klik(int x, int y);
#line 189 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void homeknop_plaatsen();
#line 196 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void halve_homeknop_plaatsen();
#line 203 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void halve_2eknop_plaatsen(String tekst);
#line 211 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
void homeknop_plaatsen(bool invert);
#line 1 "/a0/usr/workdir/BKOS5a/app_configmodus.ino"
void configmodus(String actie);
#line 27 "/a0/usr/workdir/BKOS5a/app_configmodus.ino"
void bouw_configmodus();
#line 189 "/a0/usr/workdir/BKOS5a/app_configmodus.ino"
void run_configmodus();
#line 1 "/a0/usr/workdir/BKOS5a/apps.ino"
void app_uitvoeren();
#line 1 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon(int32_t x, int32_t y, int breedte, uint32_t kleur, byte icon_to_draw[250], int num_instructions);
#line 34 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon10x10(int x, int y, String icon[10], uint16_t kleur, int factor);
#line 48 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon10x10(int x, int y, String icon[10], uint16_t kleur);
#line 59 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y);
#line 63 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y, int grootte);
#line 67 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon10x10(int x, int y, String icon[10], bool status);
#line 71 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon10x10(int x, int y, String icon[10], bool status, int factor);
#line 81 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon10x10(int x, int y, String icon[10], uint16_t kleur, bool status);
#line 88 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y, bool check);
#line 92 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y, bool check, int grootte);
#line 110 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y, uint16_t kleur);
#line 115 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y, uint16_t kleur, bool check);
#line 119 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIconWifi10x10(int x, int y, uint16_t kleur, bool check, int grootte);
#line 3 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void scherm_timeout_waarschuwing();
#line 12 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void beeld_melding(String hoofdmelding);
#line 16 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void beeld_melding(String hoofdmelding, String submelding);
#line 20 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void beeld_melding(String hoofdmelding, String submelding, String titel);
#line 24 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10]);
#line 28 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10], bool sec_icon_bool);
#line 55 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur);
#line 58 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal);
#line 61 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur);
#line 64 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal);
#line 67 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void fillRect(int x, int y, int w, int h, uint16_t kleur);
#line 70 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void fillRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal);
#line 73 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void drawRect(int x, int y, int w, int h, uint16_t kleur);
#line 76 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void drawRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal);
#line 79 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void setCursor(int x, int y);
#line 82 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void setCursor(int x, int y, uint16_t schaal);
#line 85 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void fillCircle(int x, int y, int r, uint16_t kleur);
#line 88 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void fillCircle(int x, int y, int r, uint16_t kleur, uint16_t schaal);
#line 91 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur);
#line 94 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur, uint16_t schaal);
#line 100 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, String optie1);
#line 103 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2);
#line 106 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3);
#line 109 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4);
#line 112 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4, String optie5);
#line 115 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, byte opties_cnt, String optie1, String optie2, String optie3, String optie4, String optie5);
#line 120 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
byte beeld_vraag(String hoofdmelding, String submelding, String array[], byte opties_cnt);
#line 190 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void gewaasd_scherm();
#line 202 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte, uint16_t tekst_kleur, bool schalen);
#line 226 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_x(int16_t invoer);
#line 230 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_x(int16_t invoer, int16_t code_resolutie);
#line 237 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie);
#line 241 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max);
#line 245 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t resolutie_x(int16_t resolutie, bool eind);
#line 266 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int getCursorX();
#line 270 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int getCursorX(int16_t code_resolutie);
#line 274 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int getCursorX(int16_t code_resolutie, int16_t scherm_resolutie);
#line 279 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_y(int16_t invoer);
#line 283 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_y(int16_t invoer, int16_t code_resolutie);
#line 290 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie);
#line 294 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max);
#line 298 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int16_t resolutie_y(int16_t resolutie, bool eind);
#line 319 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int getCursorY();
#line 323 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int getCursorY(int16_t code_resolutie);
#line 327 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
int getCursorY(int16_t code_resolutie, int16_t scherm_resolutie);
#line 333 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"
void run_scherm_pauze();
#line 1 "/a0/usr/workdir/BKOS5a/div_status.ino"
void wifi_check();
#line 1 "/a0/usr/workdir/BKOS5a/div_teken.ino"
void tekenItem(int32_t x, int32_t y, uint32_t kleur, int item_to_draw[250][2], int num_instructions);
#line 23 "/a0/usr/workdir/BKOS5a/div_teken.ino"
void tekenCircels(int32_t x, int32_t y, uint32_t kleur, int circle_to_draw[250][3], int num_instructions);
#line 29 "/a0/usr/workdir/BKOS5a/div_teken.ino"
void tekenTeksten(int32_t x, int32_t y, uint32_t kleur, int tekst_positie[20][3], String teksten[20], int num_instructions);
#line 3 "/a0/usr/workdir/BKOS5a/div_tijd.ino"
void internet_tijd();
#line 8 "/a0/usr/workdir/BKOS5a/div_tijd.ino"
String tijd();
#line 12 "/a0/usr/workdir/BKOS5a/div_tijd.ino"
String tijd(String wat);
#line 1 "/a0/usr/workdir/BKOS5a/hw_scherm.ino"
void tft_begin();
#line 5 "/a0/usr/workdir/BKOS5a/hw_scherm.ino"
void tft_setup();
#line 21 "/a0/usr/workdir/BKOS5a/hw_sd.ino"
bool sd_begin();
#line 24 "/a0/usr/workdir/BKOS5a/hw_sd.ino"
void sd_setup();
#line 29 "/a0/usr/workdir/BKOS5a/hw_sd.ino"
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
#line 60 "/a0/usr/workdir/BKOS5a/hw_sd.ino"
void readFile(fs::FS &fs, const char * path);
#line 76 "/a0/usr/workdir/BKOS5a/hw_sd.ino"
void sd_lees();
#line 30 "/a0/usr/workdir/BKOS5a/hw_touch.ino"
void ts_begin();
#line 33 "/a0/usr/workdir/BKOS5a/hw_touch.ino"
void ts_setup();
#line 49 "/a0/usr/workdir/BKOS5a/hw_touch.ino"
bool ts_touched();
#line 88 "/a0/usr/workdir/BKOS5a/hw_touch.ino"
int touch_x();
#line 92 "/a0/usr/workdir/BKOS5a/hw_touch.ino"
int touch_y();
#line 1 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void instellingen(String actie);
#line 29 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void instellingen_menu(String actie);
#line 50 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void bouw_instellingen();
#line 56 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void bouw_instellingen(bool overlay);
#line 75 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void insteling_knop_plaatsen(int model, int knop_nummer);
#line 107 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void alle_instelling_knoppen_plaatsen();
#line 122 "/a0/usr/workdir/BKOS5a/instellingen.ino"
int klik_instelling(int x, int y);
#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_cnt.ino"
void bouw_instellingen_io_cnt();
#line 5 "/a0/usr/workdir/BKOS5a/instellingen_io_cnt.ino"
void instellingen_io_cnt_aanpassen_naar(int cnt_nieuw);
#line 23 "/a0/usr/workdir/BKOS5a/instellingen_io_cnt.ino"
void run_instellingen_io_cnt();
#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
void naar_instellingen_io_poort(int poort);
#line 29 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
void io_poorten_tijdelijk_opslaan();
#line 55 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
String io_poorten_io_events_instellingen_toevoegen();
#line 83 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
String io_poorten_io_ruimte_instellingen_toevoegen();
#line 90 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
void bouw_instellingen_io_poort();
#line 115 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
void bouw_instellingen_io_poort_knoppen_plaatsen();
#line 167 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
void run_instellingen_io_poort();
#line 1 "/a0/usr/workdir/BKOS5a/io.ino"
void io_boot();
#line 39 "/a0/usr/workdir/BKOS5a/io.ino"
String io_code_naar_naam(byte code);
#line 81 "/a0/usr/workdir/BKOS5a/io.ino"
void io_setup();
#line 85 "/a0/usr/workdir/BKOS5a/io.ino"
void io_detect();
#line 220 "/a0/usr/workdir/BKOS5a/io.ino"
void io();
#line 334 "/a0/usr/workdir/BKOS5a/io.ino"
void io_reset_alert();
#line 450 "/a0/usr/workdir/BKOS5a/io.ino"
void io_schakel(int knop);
#line 475 "/a0/usr/workdir/BKOS5a/io.ino"
int io_output_status(byte output);
#line 495 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_defaults();
#line 504 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_defaults(int standaard);
#line 543 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_1();
#line 641 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_2();
#line 739 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_3();
#line 884 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_4();
#line 982 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_5();
#line 1127 "/a0/usr/workdir/BKOS5a/io.ino"
void io_set_6();
#line 1193 "/a0/usr/workdir/BKOS5a/io.ino"
void io_state_opslaan();
#line 1202 "/a0/usr/workdir/BKOS5a/io.ino"
void io_state_laden();
#line 1211 "/a0/usr/workdir/BKOS5a/io.ino"
void io_state_check();
#line 1 "/a0/usr/workdir/BKOS5a/io_instellingen.ino"
void naar_io_instellingen();
#line 6 "/a0/usr/workdir/BKOS5a/io_instellingen.ino"
void start_io_instellingen();
#line 14 "/a0/usr/workdir/BKOS5a/io_instellingen.ino"
void bouw_io_instellingen();
#line 44 "/a0/usr/workdir/BKOS5a/io_instellingen.ino"
void bouw_io_instellingen_knoppen();
#line 82 "/a0/usr/workdir/BKOS5a/io_instellingen.ino"
void run_io_instellingen();
#line 8 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
void bouw_nummeriek();
#line 11 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
void bouw_nummeriek(String naam);
#line 45 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
void nummeriek_cijfer_toevoegen(int invoer);
#line 52 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
void nummeriek_cijfer_verwijderen();
#line 64 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
void nummeriek_enter_esc();
#line 69 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
void run_nummeriek();
#line 1 "/a0/usr/workdir/BKOS5a/ota.ino"
void ota_setup();
#line 5 "/a0/usr/workdir/BKOS5a/ota.ino"
void ota_setup(bool displayed);
#line 12 "/a0/usr/workdir/BKOS5a/ota.ino"
void connectToWiFi();
#line 16 "/a0/usr/workdir/BKOS5a/ota.ino"
void connectToWiFi(bool displayed);
#line 40 "/a0/usr/workdir/BKOS5a/ota.ino"
void disconnectWiFi();
#line 45 "/a0/usr/workdir/BKOS5a/ota.ino"
void checkWiFi();
#line 8 "/a0/usr/workdir/BKOS5a/ota_git.ino"
void ota_git_update();
#line 47 "/a0/usr/workdir/BKOS5a/ota_git.ino"
bool checkForFirmwareUpdate();
#line 79 "/a0/usr/workdir/BKOS5a/ota_git.ino"
String fetchAlowedVersions();
#line 102 "/a0/usr/workdir/BKOS5a/ota_git.ino"
String fetchLatestVersion();
#line 125 "/a0/usr/workdir/BKOS5a/ota_git.ino"
String fetchVersion5();
#line 139 "/a0/usr/workdir/BKOS5a/ota_git.ino"
void downloadAndApplyFirmware();
#line 149 "/a0/usr/workdir/BKOS5a/ota_git.ino"
void downloadAndApplyFirmware(byte BKOS);
#line 184 "/a0/usr/workdir/BKOS5a/ota_git.ino"
bool startOTAUpdate(WiFiClient* client, int contentLength);
#line 249 "/a0/usr/workdir/BKOS5a/ota_git.ino"
void draw_update_progress(int progress);
#line 1 "/a0/usr/workdir/BKOS5a/ota_wifi.ino"
void ota_wifi_update();
#line 5 "/a0/usr/workdir/BKOS5a/ota_wifi.ino"
void ota_wifi_setup();
#line 9 "/a0/usr/workdir/BKOS5a/ota_wifi.ino"
void ota_wifi_setup(bool displayed);
#line 1 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void wifimanager_app(String actie);
#line 21 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void wifimanager();
#line 25 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void wifimanager(bool forceConfig);
#line 140 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void saveConfigFile();
#line 161 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
bool loadConfigFile();
#line 183 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void saveConfigCallback();
#line 187 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void configModeCallback(WiFiManager *myWiFiManager);
#line 43 "/a0/usr/workdir/BKOS5a/BKOS5a.ino"
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
#endif
#line 1 "/a0/usr/workdir/BKOS5a/APP_exterieur.ino"
void exterieur(String actie) {
  if (actie == "bouw") {
    bouw_exterieur();
  } else if (actie == "run") {
    run_exterieur();
  } else if (actie == "naam") {
    item_naam = ("Exterieur");
  } else if (actie == "print") {
    exterieur("naam");
    tft.print(item_naam);
  } else if (actie == "icon_groot") {
    int x = getCursorX();
    int y = getCursorY();

    uint16_t kl_lucht  = tft.color565(100, 160, 210);
    uint16_t kl_water  = tft.color565(20,  65,  120);

    // Achtergrond: lucht + water
    int wl = y + 52;
    fillRect(x+3, y+3, 98, 60, kl_lucht);
    fillRect(x+3, wl,  98, 43, kl_water);
    drawLine(x+3, wl,   x+100, wl,   tft.color565(160, 210, 240));
    drawLine(x+3, wl+1, x+100, wl+1, tft.color565(100, 170, 220));

    // Mini boot - schaal ~0.55x van origineel (120x165 -> 66x90)
    // Coordinaten: orig_x*0.55 + x+18, orig_y*0.35 + y+8
    uint16_t km = tft.color565(185, 185, 185);
    uint16_t kz = tft.color565(155, 60,  35);
    uint16_t kr = tft.color565(30,  80,  40);
    uint16_t ko = tft.color565(235, 235, 235);
    int bx = x + 40;  // boot offset x
    int by = y + 120;   // boot offset y
    // Schaal: sx = bx + int(ox*0.55), sy = by + int(oy*0.35)
    // Mast (x=65-69 -> ~36, y=0-133)
    tft.drawLine(bx+36, by+0,  bx+36, by+47, km);
    tft.drawLine(bx+37, by+0,  bx+37, by+47, km);
    // Verstaging
    tft.drawLine(bx+36, by+1, bx+0,  by+52, km);  // voor
    tft.drawLine(bx+36, by+1, bx+66, by+49, km);  // achter
    // Giek (y=118-120 -> ~41-42)
    tft.drawLine(bx+11, by+41, bx+36, by+41, km);
    // Grootzeil (roodbruin driehoek)
    tft.drawLine(bx+11, by+41, bx+36, by+1,  kz);
    // Genua
    tft.drawLine(bx+36, by+47, bx+64, by+48, kz);
    tft.drawLine(bx+36, by+47, bx+29, by+45, kz);
    // Romp
    tft.drawLine(bx+1,  by+52, bx+55, by+58, kr);
    tft.drawLine(bx+55, by+58, bx+66, by+49, kr);
    tft.drawLine(bx+1,  by+52, bx+0,  by+52, kr);
    // Opbouw (wit blokje)
    tft.drawLine(bx+0,  by+52, bx+22, by+49, ko);
    tft.drawLine(bx+22, by+49, bx+22, by+44, ko);
    tft.drawLine(bx+22, by+44, bx+30, by+44, ko);
    tft.drawLine(bx+30, by+44, bx+47, by+47, ko);

    // Knoppen op 1 rij (Haven, Zeilen, Motor, Anker)
    const char* knop_labels[] = {"Hav", "Zei", "Mot", "Ank"};
    int kx_start = x + 4;
    int ky = y + 68;
    int kw = 22;
    int kh = 14;
    int kgap = 2;
    for (int k = 0; k < 4; k++) {
      int kx = kx_start + k * (kw + kgap);
      uint16_t kb_kleur = (exterieurscherm_status[k] == 1) ?
        tft.color565(0, 180, 70) : tft.color565(40, 40, 60);
      fillRect(kx, ky, kw, kh, kb_kleur);
      drawRect(kx, ky, kw, kh, tft.color565(90, 90, 110));
      tft.setTextColor(kleur_wit);
      tft.setTextSize(1);
      setCursor(kx + 2, ky + 3);
      tft.print(knop_labels[k]);
    }

    tft.setTextColor(kleur_donker);
    tft.setTextSize(1.5);
    setCursor(x+5, y+105);
  }
}

void bouw_exterieur() {

  // Kleuren zee/lucht sfeer
  uint16_t kl_lucht       = tft.color565(120, 180, 220);  // lichtblauw
  uint16_t kl_water       = tft.color565(18,  60,  115);  // diep zeeblauw
  uint16_t kl_golf_crest  = tft.color565(160, 215, 245);  // schuimkop
  uint16_t kl_golf_dal    = tft.color565(15,  55,  105);  // golfbal
  uint16_t kl_header      = tft.color565(10,  25,  65);   // donker header

  // Waterlijn in schermcoordinaten
  int wl_y = scherm_y(ext_y + 165); // = scherm_y(210) ≈ 315px

  // --- LUCHT (boven waterlijn) ---
  tft.fillRect(0, 0, tft.width(), wl_y, kl_lucht);

  // Lichte verloop lucht: iets donkerder aan horizon
  for (int i = 0; i < 30; i++) {
    uint8_t fade = (uint8_t)(i * 2);
    uint16_t kl_fade = tft.color565(120-fade/4, 180-fade/4, 220-fade/8);
    tft.drawFastHLine(0, wl_y - 30 + i, tft.width(), kl_fade);
  }

  // --- WATER (onder waterlijn) ---
  tft.fillRect(0, wl_y, tft.width(), tft.height()-wl_y, kl_water);

  // Golvende waterlijn (sinus)
  for (int gx = 0; gx < tft.width(); gx++) {
    int gy1 = wl_y + (int)(4.0 * sin(gx * 0.04));
    int gy2 = wl_y + (int)(3.0 * sin(gx * 0.04 + 1.2)) + 5;
    int gy3 = wl_y + (int)(2.5 * sin(gx * 0.06 + 0.5)) + 10;
    tft.drawPixel(gx, gy1,   kl_golf_crest);
    tft.drawPixel(gx, gy1+1, tft.color565(100, 170, 215));
    tft.drawPixel(gx, gy2,   tft.color565(60, 130, 185));
    tft.drawPixel(gx, gy3,   tft.color565(40, 100, 160));
  }

  // Kleine schuimvlekjes op de golven
  for (int gx = 20; gx < tft.width()-10; gx += 45) {
    int gy = wl_y + (int)(4.0 * sin(gx * 0.04)) - 1;
    tft.fillRect(gx, gy, 8, 2, tft.color565(220, 240, 255));
  }

  // --- HEADER BALK ---
  kleur_header_balk = tft.color565(0,100,200); // blauw
  drawHeader(0, 0, TFT_WIDTH, 30, kleur_header_balk, kleur_header_tekst, true); // inclusief klok/symbolen, voor title
  // --- KNOPPEN SETUP ---
  aantal_knoppen = exterieur_knoppen_cnt;

  delete[]knoppen_positie;
  delete[]knoppen_teken_positie;
  delete[]knoppen_tekst;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst_kleur;
  delete[]knoppen_status;

  knoppen_positie      = new int*[aantal_knoppen];
  knoppen_teken_positie= new int*[aantal_knoppen];
  knoppen_tekst        = new char*[aantal_knoppen];
  knoppen_basiskleur   = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur  = new uint16_t*[aantal_knoppen];
  knoppen_status       = new byte[aantal_knoppen];

  for (int i = 0; i < aantal_knoppen; i++) {
    knoppen_positie[i]       = exterieurscherm_knoppen_positie[i];
    knoppen_teken_positie[i] = exterieurscherm_knoppen_positie[i];
    knoppen_tekst[i]         = exterieur_knoppen_namen[i];
    knoppen_status[i]        = exterieurscherm_status[i];
    knoppen_basiskleur[i]    = exterieruscherm_knoppen_kleur;
    knoppen_tekst_kleur[i]   = schakelscherm_knoppen_tekst_kleur;
  }

  alle_knoppen_plaatsen();

  // --- BOOT TEKENEN ---
  exterieur_teken_boot(ext_x, ext_y);
  exterieur_symbolen_verlichting(ext_x, ext_y);
  interieur_verlichting();
}

void exterieur_teken_boot(int32_t x, int32_t y) {
// Romp groen
tft.fillRect(x+20, y+180, 160, 20, tft.color565(30,80,40)); // Onderrand
tft.fillTriangle(x+100, y+100, x+140, y+150, x+60, y+150, tft.color565(30,80,40)); // Knikje
  // Kleuren
  uint16_t kl_romp   = tft.color565(30,  80,  40);   // donkergroen romp
  uint16_t kl_opbouw = tft.color565(235, 235, 235);  // wit opbouw
  uint16_t kl_zeil   = tft.color565(255, 255, 255);  // wit zeilen
  uint16_t kl_mast   = tft.color565(185, 185, 185);  // grijs mast/verst./giek
  uint16_t kl_ramen  = tft.color565(140, 195, 235);  // lichtblauw ramen

  // TEKEN ORDE: onder- naar bovenlaag (vlakken vullen waar mogelijk)

  // ZEILEN wit - lijnen tekenen (geen vulling, zeilen zijn doorschijnend)
  static int boot_zeilen[][2] = {
    {20, 118}, {20, 118}, {65, 4}, {65, 4},  // grootzeil rand
    {117, 137}, {117, 137}, {89, 137}, {89, 137},  // genua deel 1
    {52, 129}, {52, 129}, {53, 120}, {53, 120}  // genua deel 2
  };
  tekenItem(x, y, kl_zeil, boot_zeilen, sizeof(boot_zeilen)/sizeof(int)/2);
  // Grootzeil vulling
  tft.fillTriangle(x + 20, y + 118, x + 65, y + 4, x + 65, y + 118, kl_zeil); // grootzeil wit
  // Genua vulling
  tft.fillTriangle(x + 89, y + 137, x + 117, y + 137, x + 53, y + 120, kl_zeil); // genua wit approx

  // ROMP donkergroen - lijnen + scanline vulling
  static int boot_romp[][2] = {
    {0, 150}, {0, 150}, {2, 165}, {100, 165}, {120, 140}, {120, 140},  // onderrand
    {70, 150}, {70, 150}, {105, 147}, {105, 147},  // westerly knikje
    {40, 140}, {40, 140}, {49, 137}, {49, 146}, {49, 146},  // kuiprand bovenzijde
    {25, 143}, {25, 143}, {25, 148}, {25, 148}  // kuiprand onderzijde
  };
  tekenItem(x, y, kl_romp, boot_romp, sizeof(boot_romp)/sizeof(int)/2);
  // Romp vulling met fillRect
  tft.fillRect(x + 2, y + 151, 98, 14, kl_romp); // onderrand/kuip donker
  // Romp vulling met scanlines (geen horizontale balken)
//   for (int ry = 151; ry < 165; ry++) {
//     tft.drawLine(scherm_x(x + 2), scherm_y(y + ry), scherm_x(x + 100), scherm_y(y + ry), kl_romp);
//   }
//   tekenCircels(x, y, kl_romp, circels_boot, sizeof(circels_boot)/sizeof(int)/3);
// 
//   // MAST/VERSTAGING/GIEK grijs - lijnen
  static int boot_mast[][2] = {
    {0, 150}, {0, 150}, {63, 0}, {71, 0}, {120, 141}, {120, 141},  // verstaging
    {20, 120}, {20, 120}, {65, 120}, {65, 119}, {20, 119}, {20, 118}, {65, 118}, {65, 118},  // giek
    {69, 133}, {69, 133}, {69, 0}, {68, 0}, {68, 133}, {67, 133}, {67, 0}, {66, 0}, {66, 133},  // mast lijnen 1-2
    {65, 133}, {65, 0}, {65, 0}  // mast lijn 3
  };
  tekenItem(x, y, kl_mast, boot_mast, sizeof(boot_mast)/sizeof(int)/2);

  // OPBOUW wit - lijnen + scanline vulling
  static int boot_opbouw[][2] = {
    {0, 150}, {2, 146}, {40, 140}, {40, 125}, {49, 125}, {54, 133}, {70, 133}, {72, 135}, {85, 135}, {92, 142}, {92, 142},  // opbouw contour
    {54, 133}, {54, 133}, {44, 133}, {44, 137}, {44, 137}  // kajuit deur
  };
  tekenItem(x, y, kl_opbouw, boot_opbouw, sizeof(boot_opbouw)/sizeof(int)/2);
  // Opbouw vulling met fillRect
  tft.fillRect(x + 2, y + 126, 38, 15, kl_opbouw); // kajuit wit approx
  // Opbouw vulling met scanlines (rompvorm volgen)
//   for (int oy = 126; oy < 140; oy++) {
//     int lx = oy - 126 + 2;  // volg romp curve
//     int rx = 40 - (oy - 126)/3;
//     tft.drawLine(scherm_x(x + lx), scherm_y(y + oy), scherm_x(x + rx), scherm_y(y + oy), kl_opbouw);
//   }

  // RAMEN lichtblauw - lijnen
  static int boot_ramen[][2] = {
    {51, 142}, {51, 142}, {58, 142}, {58, 135}, {53, 135}, {51, 142}, {51, 142},  // raam 1
    {61, 142}, {61, 142}, {69, 142}, {67, 135}, {61, 135}, {61, 142}, {61, 142},  // raam 2
    {42, 131}, {42, 131}, {51, 131}, {47, 127}, {42, 127}, {42, 131}, {42, 131}  // raam 3
  };
  tekenItem(x, y, kl_ramen, boot_ramen, sizeof(boot_ramen)/sizeof(int)/2);
  // Ramen vullingen lichtblauw
  tft.fillRect(x + 51, y + 135, 7, 7, kl_ramen); // raam 1
  tft.fillRect(x + 61, y + 135, 6, 7, kl_ramen); // raam 2
  tft.fillRect(x + 42, y + 127, 9, 4, kl_ramen); // raam 3

  // Roepnaam
  tekenTeksten(x, y, tft.color565(200, 200, 200), teken_tekst_positie_boot, teken_tekst_boot,
               sizeof(teken_tekst_positie_boot)/sizeof(int)/3);
}

void exterieur_symbolen_verlichting(int32_t x, int32_t y){

  int32_t ix;
  int32_t iy;
  bool gevonden;
  // hek
  ix = 0;
  iy = 150;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'h') && (io_namen[io_knoppen[i]][5] == 'e') && (io_namen[io_knoppen[i]][6] == 'k')) {
      if (io_input[io_knoppen[i]] == 1) {
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
        } else {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
      } else {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
      }
    }
  }

  // top
  ix = 67;
  iy = 0;
  gevonden = false;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == '3') && (io_namen[io_knoppen[i]][5] == 'k') && (io_namen[io_knoppen[i]][6] == 'l')) {
      if (io_input[io_knoppen[i]] == 1) {
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
          gevonden = true;
        } else {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
        gevonden = true;
      }
    }
  }
  for (int i = 0; i < io_knoppen_cnt; i++){
    if (!gevonden) {
      if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'a') && (io_namen[io_knoppen[i]][5] == 'n') && (io_namen[io_knoppen[i]][6] == 'k') && (io_namen[io_knoppen[i]][7] == 'e') && (io_namen[io_knoppen[i]][8] == 'r')){
        if (io_input[io_knoppen[i]] == 1){
          if (io_output[io_knoppen[i]] == 1) {
            drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
          } else {
            drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
          }
        } else if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
        } else {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
        }
      }
    }
  }

  // stoom
  ix = 75;
  iy = 75;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 's') && (io_namen[io_knoppen[i]][5] == 't') && (io_namen[io_knoppen[i]][6] == 'o') && (io_namen[io_knoppen[i]][7] == 'o') && (io_namen[io_knoppen[i]][8] == 'm')) {
      if (io_input[io_knoppen[i]] == 1){
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
        } else {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
      } else {
      drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
      }
    }
  }

  // boordlichten
  ix = 80;
  iy = 143;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'n') && (io_namen[io_knoppen[i]][5] == 'a') && (io_namen[io_knoppen[i]][6] == 'v') && (io_namen[io_knoppen[i]][7] == 'i')) {
      if (io_input[io_knoppen[i]] == 1){
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-23 , 30, kleur_actief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
        } else {
          drawIcon(x+ix - 15, y+iy-23 , 30, kleur_oranje, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-23 , 30, kleur_passief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
      } else {
      drawIcon(x+ix - 15, y+iy-23 , 30, kleur_zwart, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
      drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
      }
    }
  }
}

void run_exterieur() {
  int druk;
  if (actieve_touch) {
    druk = klik(ts_x, ts_y);

    if (druk > -1){
      exterieurscherm_schakel(druk);
      knop_plaatsen(druk);
      io_wijziging = true;
    }

    while (ts_touched()) {
      ts_begin();
    }

    if (ts_y >= home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (ts_y < 30) {
      klik_header(ts_x, ts_y);
    }
  }
  if (io_runned) {
    io_runned = false;
    exterieur_symbolen_verlichting(ext_x, ext_y);
  }
  
  ts_begin(); 
}

void exterieurscherm_schakel(int knop) {
  if (knop > 3) {
    if (charstrip(exterieur_knoppen_namen[knop]) == "**I_licht") {
      interieur_verlichting(true);
      exterieurscherm_status[knop] = interieur_licht_variant;
      knoppen_status[knop] = interieur_licht_variant;
      // knop_plaatsen(knop);
    } else {
      if (knoppen_status[knop] == 0) {
        // uit wordt aan
        knoppen_status[knop] = 1;
        exterieurscherm_status[knop] = 1;
        for (int i = 0; i < io_knoppen_cnt; i++){
          int j = 0;
          bool fl = false;
          bool stp = false;
          while (!stp){
            if (io_namen[io_knoppen[i]][j] == knoppen_tekst[knop][j]) {
              stp = false;
            } else {
              fl = true;
              stp = true;
            }
            if (j >= 9){
              stp = true;
            }
            j++;
          }
          if (!fl) {
            if ((io_output[io_knoppen[i]] == 0) || (io_output[io_knoppen[i]] == 1)){
              io_output[io_knoppen[i]] = 1;
            } else if ((io_output[io_knoppen[i]] == 2) || (io_output[io_knoppen[i]] == 3)){
              io_output[io_knoppen[i]] = 3;
            }
          }
        }
      } else{
        // aan wordt uit
        knoppen_status[knop] = 0;
        exterieurscherm_status[knop] = 0;
        for (int i = 0; i < io_knoppen_cnt; i++){
          int j = 0;
          bool fl = false;
          bool stp = false;
          while (!stp){
            if (io_namen[io_knoppen[i]][j] == knoppen_tekst[knop][j]) {
              stp = false;
            } else {
              fl = true;
              stp = true;
            }
            if (j >= 9){
              stp = true;
            }
            j++;
          }
          if (!fl) {
            if ((io_output[io_knoppen[i]] == 0) || (io_output[io_knoppen[i]] == 1)){
              io_output[io_knoppen[i]] = 0;
            } else if ((io_output[io_knoppen[i]] == 2) || (io_output[io_knoppen[i]] == 3)){
              io_output[io_knoppen[i]] = 2;
            }
          }
        }
      }
    }
    // knop_plaatsen(knop);
  } else {
    for (int i = 0; i < 4; i++) {
      if (i == knop) {
        if (knoppen_status[i] == 1) {
          exterieur_licht_variant++;
        } else {
          knoppen_status[i] = 1;
          exterieurscherm_status[i] = 1;
          exterieur_licht_variant = 0;
        }
      } else {
        knoppen_status[i] = 0;
        exterieurscherm_status[i] = 0;
      }
      knop_plaatsen(i);
    }
    if (knop == 0) {
      exterieur_verlichting_uit();
      interieur_verlichting();
    } else if (knop == 1) {
      exterieur_zeilverlichting();
      interieur_verlichting();
    } else if (knop == 2) {
      exterieur_motorverlichting();
      interieur_verlichting();
    } else if (knop == 3) {
      exterieur_ankerverlichting();
      interieur_verlichting();
    }
    
  }
  knop_plaatsen(knop);
  exterieur_symbolen_verlichting(ext_x, ext_y);
  io_now = true;
  // io();
  // exterieur_symbolen_verlichting(ext_x, ext_y);
  // delay(50);
  // io();
  // exterieur_symbolen_verlichting(ext_x, ext_y);
}

void exterieur_verlichting_uit(){
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
        io_output[io_knoppen[knop]] = 0;
      } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
        io_output[io_knoppen[knop]] = 2;
      }
    }
  }
}

void exterieur_zeilverlichting(){
  bool aan_uit;
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      aan_uit = false;
      if (exterieur_licht_variant > 1) {
        exterieur_licht_variant = 0;
      }
      if (exterieur_licht_variant == 0) {
        // Enkel de 3 kleurenlamp aan ( **L_3kl)
        if ((io_namen[io_knoppen[knop]][4] == '3') && (io_namen[io_knoppen[knop]][5] == 'k') && (io_namen[io_knoppen[knop]][6] == 'l')){
          aan_uit = true;
        }
      } else if (exterieur_licht_variant == 1) {
        // navigatie verlichting en heklicht aan (**L_navi  & **L_hek)
        if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
          aan_uit = true;
        }
      } 
      
      if (aan_uit) {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 1;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 3;
        }
      } else {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 0;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 2;
        }
      }
    }
  }
}

void exterieur_motorverlichting(){
  bool aan_uit;
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      aan_uit = false;
      if (exterieur_licht_variant > 2) {
        exterieur_licht_variant = 0;
      }
      if (exterieur_licht_variant == 0) {
        // ankerlicht en navigatie verlichting (**L_anker en **L_navi)
        if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'a') && (io_namen[io_knoppen[knop]][5] == 'n') && (io_namen[io_knoppen[knop]][6] == 'k') && (io_namen[io_knoppen[knop]][7] == 'e') && (io_namen[io_knoppen[knop]][8] == 'r')){
          aan_uit = true;
        }
      } else if (exterieur_licht_variant == 1) {
        // navigatie verlichting, stoomlicht en heklicht aan (**L_navi, **L_stoom  & **L_hek)
        if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
          aan_uit = true;
        }
      }  else if (exterieur_licht_variant == 2) {
        // 3 kleurenlicht en stoomlicht aan (**L_3kl & **L_stoom)
        if ((io_namen[io_knoppen[knop]][4] == '3') && (io_namen[io_knoppen[knop]][5] == 'k') && (io_namen[io_knoppen[knop]][6] == 'l')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
          aan_uit = true;
        }
      } 
      
      if (aan_uit) {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 1;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 3;
        }
      } else {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 0;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 2;
        }
      }
    }
  }
}

void exterieur_ankerverlichting(){
  bool aan_uit;
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      aan_uit = false;
      if (exterieur_licht_variant > 1) {
        exterieur_licht_variant = 0;
      }
      if (exterieur_licht_variant == 0) {
        // Enkel het ankerlicht aan ( **L_anker)
        if ((io_namen[io_knoppen[knop]][4] == 'a') && (io_namen[io_knoppen[knop]][5] == 'n') && (io_namen[io_knoppen[knop]][6] == 'k') && (io_namen[io_knoppen[knop]][7] == 'e') && (io_namen[io_knoppen[knop]][8] == 'r')){
          aan_uit = true;
        }
      } else if (exterieur_licht_variant == 1) {
        // stoomlicht en heklicht aan (**L_stoom  & **L_hek)
        if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
          aan_uit = true;
        }
      }   
      
      if (aan_uit) {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 1;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 3;
        }
      } else {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 0;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 2;
        }
      }
    }
  }
}

void interieur_verlichting() {
  interieur_verlichting(false);
}

void interieur_verlichting(bool schakelen) {
  if (schakelen) {
    interieur_licht_variant ++;
  }
  if (interieur_licht_variant >= 4) {
    interieur_licht_variant = 0;
  }
  // tft.fillRect(10, 30, 100, 30, kleur_zwart);
  // tft.setTextColor(kleur_wit);
  // tft.setCursor(10, 31);
  // tft.print(interieur_licht_variant);

  int wit = 0;
  int rood = 0;

  if (interieur_licht_variant == 0) {
    wit = 0;
    rood = 0;
  } else if (interieur_licht_variant == 1) {
    wit = 1;
    rood = 0;
  } else if (interieur_licht_variant == 2) {
    wit = 0;
    rood = 1;
  } else if (interieur_licht_variant == 3) {
    for (int i = 0; i < sizeof(exterieurscherm_status); i++) {
      if (exterieurscherm_status[i] == 1) {
        if (charstrip(exterieur_knoppen_namen[i]) == "**haven") {
          // tft.print("haven");
          wit = 1;
          rood = 0;
        } else if (charstrip(exterieur_knoppen_namen[i]) == "**zeilen") {
          // tft.print("zeilen");
          wit = 0;
          rood = 1;
        } else if (charstrip(exterieur_knoppen_namen[i]) == "**motor") {
          // tft.print("motor");
          wit = 0;
          rood = 1;
        } else if (charstrip(exterieur_knoppen_namen[i]) == "**anker") {
          // tft.print("anker");
          wit = 1;
          rood = 0;
        // } else {
        //   tft.print(i);
        //   tft.print("[");
        //   tft.print(charstrip(exterieur_knoppen_namen[i]));
        //   tft.print("]");
        }
      }
    }
  }

  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if (io_namen[io_knoppen[knop]] == "**IL_wit "){
      io_output[io_knoppen[knop]] = wit;
    } else if (io_namen[io_knoppen[knop]] == "**IL_rood"){
      io_output[io_knoppen[knop]] = rood;
    }
  }
}


String charstrip(char invoer[]) {
  String uitvoer = "";
  for (int i = 0; i < 10; i++){
    if (invoer[i] != ' '){
      uitvoer += invoer[i];
    }
  }
  uitvoer.trim();
  return uitvoer;
}
#line 1 "/a0/usr/workdir/BKOS5a/BKOS.ino"
// #if ORIENTATIE == 1
void BKOS_boot(){
  digitalWrite(TFT_BL, HIGH);
  // delay(1000);
  bkos_logo(scherm_x(120) - 100, 10, kleur_donker);
  tft.setCursor(scherm_x(120) - 95, 95);
  tft.setTextSize(8);
  tft.setTextColor(kleur_licht);
  tft.println("BKos");
  tft.setTextSize(2);
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println("Boordcomputer OS");
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println("      door");
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println(" Brendan Koster");
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.print("  versie ");
  tft.println(BKOS_VERSIE);
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println(MCU_TYPE);
  tft.setTextSize(1);
  tft.setTextColor(tft.color565(0, 0, 0));
  tft.setTextColor(tft.color565(255, 255, 255));
  // BKOS5a: QR-code configureerbaar via QR_TONEN in BKOS.h
//   #if QR_TONEN  // [BKOS5a disabled]
  if (ORIENTATIE == 1) {
//     qr_bytes(137, 220, qr_brendanintechYT, 3, kleur_donker, kleur_licht);  // [BKOS5a disabled]
  } else if (ORIENTATIE == 0) {
//     qr_bytes(150, 10, qr_brendanintechYT, scherm_x(3), kleur_donker, kleur_licht);  // [BKOS5a disabled]
  }
  // #endif  // [BKOS5a: QR_TONEN block end]

  tft.setTextColor(tft.color565(0, 0, 110));
  char c = ' ';
  while (Serial.available()) {
    c = Serial.read();
    // tft.print(c);
    if (!Serial.available()){
      delay(20);
    }
  }
  // tft.println("");
  if (RESOLUTIE == 4880) {
    tft.setCursor(0, 0);
  }
  tft.setTextColor(tft.color565(255, 255, 255));

  Serial.flush();
  Serial.print("AT S?");
  delay(50);
  BKOSS = "";
  unsigned int start = millis();
  while (!Serial.available() && millis() < start + 2500) {
    delay(50);
  }
  if (Serial.available()) {
    BKOSS = Serial.readStringUntil('\n');
  }
  
  Serial.print("AT V?");
  delay(50);
  BKOSSV = "";
  start = millis();
  while (!Serial.available() && millis() < start + 2500) {
    delay(50);
  }
  if (Serial.available()) {
    BKOSSV = Serial.readStringUntil('\n');
  }
  

  tft.print("  BKOSS  ");
  tft.setTextColor(tft.color565(0, 110, 0));
  tft.print(BKOSS);
  tft.setTextColor(kleur_wit);
  tft.print(" v ");
  tft.setTextColor(tft.color565(0, 110, 0));
  tft.print(BKOSSV);
  
  if (BKOSS == "" || BKOSSV == "") {
    tft.setTextColor(tft.color565(255, 0, 0));
    tft.println("Helaas, ontvangst BKOSS module ging niet helemaal goed");
    tft.println("          Dit is normaal wanneer je met UWB bent aangesloten");
    tft.println("          Aangesloten via UTP? dan kan het zijn dat de interne kabel niet goed zit.");
  } else {
    tft.print('\n');
  }
  tft.setTextColor(tft.color565(255, 255, 255));

  io_boot();
  // qr(120, 200, "brendanintech", 2)//, kleur_donker, kleur_licht);
  // qr(120, 200, "http://www.youtube.com/@brendanintech", 2, kleur_donker, kleur_licht);
  tft.println("");
  // io_detect();
  // sd_lees();
  // delay(1000); // Kleine vertraging waarin het opstartschemr blijft staan, wel zo mooi
  ts_setup();
}
// #elif ORIENTATIE == 0
//   void BKOS_boot(){
//     digitalWrite(TFT_BL, HIGH);
//     delay(1000);
//     bkos_logo(20, 10, kleur_donker);
//     tft.setCursor(25, 95);
//     tft.setTextSize(8);
//     tft.setTextColor(kleur_licht);
//     tft.println("BKos");
//     tft.setTextSize(2);
//     tft.println("  Boordcomputer OS");
//     tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);
//     tft.println("        door");
//     tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);
//     tft.println("   Brendan Koster");
//     tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);
//     tft.print("  versie ");
//     tft.println(BKOS_VERSIE);
//     tft.setCursor(tft.getCursorX()+25, tft.getCursorY()+5);
//     tft.println(MCU_TYPE);
//     // tft.setTextSize(1);
//     // tft.setTextColor(tft.color565(0, 0, 0));
//     tft.setTextColor(tft.color565(255, 255, 255));
//     qr_bytes(150, 10, qr_brendanintechYT, scherm_x(3), kleur_donker, kleur_licht);
//     // qr(120, 200, "brendanintech", 2)//, kleur_donker, kleur_licht);
//     // qr(120, 200, "http://www.youtube.com/@brendanintech", 2, kleur_donker, kleur_licht);
//     tft.println("");
//     io_detect();
//     delay(5000);
//   }
// #endif
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_HOME.ino"
void thuisscherm(String actie) {
  if (actie == "bouw") {
    bouw_thuisscherm();
  } else if (actie == "run") {
    run_thuisscherm();
  } else if (actie == "naam") {
    item_naam = "Thuisscherm";
  } else if (actie == "print") {
    thuisscherm("naam");
    tft.print(item_naam);
  } else if (actie == "icon_klein") {
    int x = getCursorX();
    int y = getCursorY();
    fillRect(x+2, y+2, 61, 45, kleur_zwart);
    drawRect(x+20, y+20, 20, 20, kleur_wit);
    drawLine(x+30, y+10, x+20, y+20, kleur_wit);
    drawLine(x+30, y+10, x+40, y+20, kleur_wit);

    setCursor(x+3, y+52);
  }
}


void homescherm_knop(int knop_nummer){
  fillRoundRect(keuzescherm_knoppen[knop_nummer][0], keuzescherm_knoppen[knop_nummer][1], keuzescherm_knoppen[knop_nummer][2], keuzescherm_knoppen[knop_nummer][3], 5, kleur_licht);
  tft.setTextSize(1.5);
  tft.setTextColor(kleur_donker);
  setCursor(keuzescherm_knoppen[knop_nummer][0], keuzescherm_knoppen[knop_nummer][1]);

  if (knop_nummer <= 1) {
    apps[knop_nummer+1]("icon_groot");
  } else {
    apps[knop_nummer+1]("icon_klein");
  }
  apps[knop_nummer+1]("print");
}


void alle_homescherm_knoppen_plaatsen(){
  for (byte i = 0; i < app_cnt-1; i++) {
    homescherm_knop(i);
  }
}

void bouw_thuisscherm() {

  fillRect(0, header_vlak[3], 240, 360-header_vlak[3], kleur_donker);
  homeknop_actief = false;
  
//   qr_bytes(169, 249, qr_brendanintechYT, 2, kleur_zwart, kleur_wit);  // [BKOS5a disabled]

  alle_homescherm_knoppen_plaatsen();

}


int klik_thuis(int x, int y) {
  for (byte i = 0; i < keuzescherm_knoppen_cnt; i++) {
    if ((x >= keuzescherm_knoppen[i][0]) && (x <= keuzescherm_knoppen[i][0]+keuzescherm_knoppen[i][2]) && (y >= keuzescherm_knoppen[i][1]) && (y <= keuzescherm_knoppen[i][1]+keuzescherm_knoppen[i][3])) {
      return i;
    }
  }
  return -1;
}

void run_thuisscherm() {
  int druk;
  if (actieve_touch) {

    druk = klik_thuis(ts_x, ts_y);

    if ((druk > -1) & (druk < app_cnt)) {
      actieve_app = druk + 1;
      scherm_bouwen = true;
    }
  }
}
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_SCHAKEL.ino"
void schakelscherm(String actie) {
  if (actie == "bouw") {
    // Serial.println("Bouw");
    bouw_schakelscherm();
  } else if (actie == "run") {
    run_schakelscherm();
  } else if (actie == "naam") {
    item_naam = ("Paneel");
  } else if (actie == "print") {
    schakelscherm("naam");
    tft.print(item_naam);
  } else if (actie == "icon_groot") {
    int x = getCursorX();
    int y = getCursorY();
    fillRect(x+3, y+3, 98, 95, kleur_zwart);      // Vlak waarbinnen een (groot) icon getekent mag worden 
    drawLine(x+20, y+14, x+20, y+80, kleur_wit);
    drawLine(x+20, y+47, x+25, y+19, kleur_wit);
    drawLine(x+20, y+14, x+60, y+14, kleur_wit);
    drawLine(x+25, y+19, x+65, y+19, kleur_wit);
    drawLine(x+20, y+14, x+25, y+19, kleur_wit);
    drawLine(x+60, y+14, x+65, y+19, kleur_wit);
    drawLine(x+20, y+80, x+60, y+80, kleur_wit);
    drawLine(x+60, y+80, x+60, y+47, kleur_wit);
    drawLine(x+60, y+47, x+65, y+19, kleur_wit);
    drawLine(x+20, y+47, x+60, y+47, tft.color565(100, 100, 100));
    drawLine(x+42, y+42, x+45, y+25, kleur_wit);
    setCursor(x+35, y+53);
    tft.setTextSize(2);
    tft.setTextColor(kleur_wit);
    tft.print(0);
    tft.setTextColor(kleur_donker);
    tft.setTextSize(1.5);
    setCursor(x+5, y+105);
  }
}

void bouw_schakelscherm() {

  // header_plaatsen("Schakelscherm");
  achtergrond(kleur_licht);

  if (io_knoppen_cnt < 10) {
    aantal_knoppen = io_knoppen_cnt;
  } else {
    aantal_knoppen = 10;
  }
  
  delete[]knoppen_positie;
  delete[]knoppen_teken_positie;
  delete[]knoppen_tekst;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst_kleur;
  delete[]knoppen_status;

  
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = new byte[aantal_knoppen];

  
  for (int i  = 0 ; i < aantal_knoppen ; i++) {
    knoppen_positie[i] = schakelscherm_knoppen_positie[i];
    knoppen_teken_positie[i] = schakelscherm_knoppen_positie[i];
    knoppen_tekst[i] = io_namen[io_knoppen[i]];
    knoppen_status[i] = io_output_status(io_output[io_knoppen[i]]);
    knoppen_basiskleur[i] = schakelscherm_knoppen_kleur;
    knoppen_tekst_kleur[i] = schakelscherm_knoppen_tekst_kleur;
    
  }
  
  alle_knoppen_plaatsen();
  schakelscherm_indicator_bolletjes();

  

}

void schakelscherm_indicator_bolletjes() {
  for (int i = 0; i < aantal_knoppen; i++) {
    if (io_input[io_knoppen[i]]) {
      fillCircle(schakelscherm_bolletjes[i][0], schakelscherm_bolletjes[i][1], 10, kleur_wit);
    } else {
      fillCircle(schakelscherm_bolletjes[i][0], schakelscherm_bolletjes[i][1], 10, kleur_zwart);
    }
  }
}

void run_schakelscherm() {
  schakelscherm_indicator_bolletjes();
  // ts_begin();
  int druk;

  if (actieve_touch) {
      
    druk = klik(ts_x, ts_y);
    if (druk >= 0) { knop_feedback(druk); } // BKOS5a: visuele touch feedback

    if (druk > -1){
      io_schakel(druk);
      io_wijziging = true;
    }

    if (ts_y >= home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (ts_y < 30) {
      klik_header(ts_x, ts_y);
    }

    
  }
  // ts_begin(); 
}
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_header.ino"
void header_plaatsen(String headertitel) {
  header_titel = headertitel;
  header_plaatsen();
}

void header_plaatsen() {
  // Direct de header balk vullen met de gewenste kleur
  fillRect(header_vlak[0], header_vlak[1], header_vlak[2], header_vlak[3], kleur_header_balk);
  int icon_tekst_grootte = scherm_x(1);
  if (RESOLUTIE == 4880) {
    icon_tekst_grootte = 2;
  }
  // Titel links uitgelijnd (na alert ruimte ~25-30px)
  tft.setTextColor(kleur_header_tekst);
  tft.setTextSize(icon_tekst_grootte);
  setCursor(30, 7);
  tft.print(header_titel);

  // Direct klok en iconen tekenen (geen delay)
  klok_update(true);
  header_alert(icon_tekst_grootte);
}

void klok_update() {
  klok_update(false);
}

void klok_update(bool force) {
  String huidige_tijd = tijd();
  if ((laatste_tijd != huidige_tijd) || (force)) {
    laatste_tijd = huidige_tijd;
    // Klok achtergrond vullen met header kleur
    fillRect(160, 0, 45, header_vlak[3], kleur_header_balk);
    setCursor(190, 7);

    int icon_tekst_grootte = scherm_x(1);
    if (RESOLUTIE == 4880){
      icon_tekst_grootte = 2;
    }

    checkWiFi();

    tft.setTextSize(icon_tekst_grootte);
    tft.setTextColor(kleur_header_tekst);
    tft.print(huidige_tijd);
    h_klok_millis = millis();
    drawIcon10x10(225, 5, icon_instellingen, kleur_header_tekst, icon_tekst_grootte);
    drawIconWifi10x10(175, 5, icon_tekst_grootte);
    drawIcon10x10(160, 5, icon_sd, sd_connect, icon_tekst_grootte);
    klok_getekend = millis();
  }
}


void achtergrond(uint16_t kleur) {
  if ((homeknop_actief) || (homeknop_half_actief)) {
    fillRect(0, header_vlak[3], 240, home_knop[1] - header_vlak[3], kleur);
    if (homeknop_half_actief) {
      fillRect(110, home_knop[1], 240 - 110, header_vlak[3], kleur);
    }
  } else {
    fillRect(0, header_vlak[3], 240, 320 - header_vlak[3], kleur);
  }
  
  if (kleur == kleur_donker) {
    kleur_home_knop = kleur_licht;
    kleur_home_tekst = kleur_donker;
  } else {
    kleur_home_knop = kleur_donker;
    kleur_home_tekst = kleur_licht;
  }
}

void overligger(uint16_t kleur, int hoogte, int dicht) {
  
  fillRect(0, header_vlak[3], 240, dicht - header_vlak[3], kleur);
  
  int i = dicht + 1;
  while (i <= hoogte) {
    drawLine(0, i, 240, i, kleur);
    i ++;
    drawLine(0, i, 240, i, kleur);
    i += 2;
  }
  while (i < 360) {
    drawLine(0, i, 240, i, kleur);
    i += 2;
  }
}

void overligger(uint16_t kleur, int hoogte) {
  int i = header_vlak[3] + 1;
  while (i <= hoogte) {
    drawLine(0, i, 240, i, kleur);
    i ++;
    drawLine(0, i, 240, i, kleur);
    i += 2;
  }
  while (i < 360) {
    drawLine(0, i, 240, i, kleur);
    i += 2;
  }
}

void achtergrond(uint16_t kleur, bool plaats_home_knop) {
  achtergrond(kleur);
  if (plaats_home_knop) {
    homeknop_plaatsen();
  } else {
    fillRect(0, home_knop[1], 240, 360-home_knop[1], kleur);
  }
}

void achtergrond(uint16_t kleur, bool plaats_home_knop, uint16_t home_knop_kleur) {
  achtergrond(kleur);
  if (plaats_home_knop) {
    homeknop_plaatsen(home_knop_kleur);
  } else {
    fillRect(0, home_knop[1], 240, 360-home_knop[1], kleur);
  }
}

void header_alert() {
  int icon_tekst_grootte = scherm_x(1);
  if (RESOLUTIE == 4880){
    icon_tekst_grootte = 2;
  } 
  header_alert(icon_tekst_grootte);
}


void header_alert(int icon_tekst_grootte) {
  if (io_alert_cnt > 0) {
    if (io_alert_app > 0) {
      drawIcon10x10(5, 5, icon_bel, kleur_actief_rood, icon_tekst_grootte);
    } else if (!io_diepcheck) {
      drawIcon10x10(5, 5, icon_bel, tft.color565(255, 255, 0), icon_tekst_grootte);
    } else {
      drawIcon10x10(5, 5, icon_bel, kleur_oranje, icon_tekst_grootte);
    }
    setCursor(header_vlak[0] + 3, header_vlak[1] + 5);
    tft.setTextSize(2);
    tft.print("!");
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_wit);
    tft.print(io_alert_cnt);
  } else if (io_diepcheck_pins_cnt == 0) {
    drawIcon10x10(5, 5, icon_bel, tft.color565(255, 0, 255), icon_tekst_grootte);
  } else {
    fillRect(header_vlak[0], header_vlak[1], 20, header_vlak[3], kleur_header_balk);
  }
}

void bouw_alert() {
  int icon_tekst_grootte = scherm_x(1);
  if (RESOLUTIE == 4880){
    icon_tekst_grootte = 2;
  } 
  bouw_alert(icon_tekst_grootte);
}

void bouw_alert(int icon_tekst_grootte) {
  fillRect(0, 25, 240, 100, tft.color565(50, 50, 50));
  int h = 30;
  tft.setTextColor(kleur_wit);
  tft.setTextSize(1);
  
  if (!wifi__aangesloten) {
    drawIcon10x10(10, h-1, icon_wifi, tft.color565(100, 100, 100), icon_tekst_grootte);
    drawIcon10x10(10, h-1, icon_kruis, tft.color565(255, 0, 0), icon_tekst_grootte);
    setCursor(24, h);
    tft.print("WiFi adapter niet aangesloten!");
    h += 15;
  }
  if (io_diepcheck_pins_cnt == 0) {
    drawIcon10x10(10, h-1, icon_bel, tft.color565(255, 0, 255), icon_tekst_grootte);
    setCursor(25, h);
    tft.print("Geen diepcheck pins toegewezen !");
    h += 15;
  }
  if (!io_diepcheck) {
    drawIcon10x10(10, h-1, icon_bel, tft.color565(0, 255, 255), icon_tekst_grootte);
    setCursor(25, h);
    tft.print("Diepcheck gefaald !");
    h += 15;
  }
  if (io_alert_cnt > 0) {
    drawIcon10x10(10, h-1, icon_bel, kleur_oranje, icon_tekst_grootte);
    setCursor(25, h);
    tft.print(io_alert_cnt);
    tft.print(" open alert");
    if (io_alert_cnt > 1) {
      tft.print("s");
    }
    h += 15;
  }
  if  (io_alert_app > 0){
    drawIcon10x10(10, h-1, icon_bel, tft.color565(255, 0, 0), icon_tekst_grootte);
    setCursor(25, h);
    tft.print(io_alert_app);
    tft.print(" onverzonden triger");
    if (io_alert_app > 1) {
      tft.print("s");
    }
    tft.print("!");
    h += 15;
  }

  h = 75;
  setCursor(10, h);
  tft.print("status resetten?");

  fillRoundRect(40, 90, 75, 30, 5, kleur_passief_rood);
  fillRoundRect(125, 90, 75, 30, 5, kleur_passief_groen);
  center_tekst(77, 95, "NEE", 2, kleur_wit);
  center_tekst(163, 95, "JA", 2, kleur_wit);
}

void run_alert() {
  delay(50);
  ts_begin();
  while (ts_touched()) {
    delay(50);
    ts_begin();
  }

  bool verder = false;

  while (!verder) {
    io();
    delay(50);
    ts_begin();
    if (ts_touched()) {
      int x = touch_x();
      int y = touch_y();

      delay(50);
      ts_begin();
      while (ts_touched()) {
        delay(50);
        ts_begin();
      }

      if ((x > 125)&(x < 200) & (y > 80)&(y < 130)) {
        // ja
        io_reset_alert();
        scherm_bouwen = true;
        verder = true;
      
      } else if ((x > 40)&(x < 115) & (y > 80)&(y < 130)) {
        // nee
        scherm_bouwen = true;
        verder = true;
      }

    }
    
  }
}

void klik_header(int x, int y) {
  if (x > 150) {
    actieve_app = 6;
    scherm_bouwen = true;
  } else if (x < 100) {
    bouw_alert();
    run_alert();
  }
}

#line 1 "/a0/usr/workdir/BKOS5a/BKOS_logo.ino"
void bkos_logo(int32_t x, int32_t y, uint32_t kleur) {
  int k = 0;
  int r = 0;
  bool teken = false;
  for (int i = 0; i < sizeof(bkos_logo_200_75)/sizeof(byte); i ++) {
    if (teken) {
      if (bkos_logo_200_75[i] > 0) {
        tft.drawLine(x+k, y+r, x+k+bkos_logo_200_75[i], y+r, kleur);
      }
      teken = false;
    } else {
      teken = true;
    }
    k += bkos_logo_200_75[i];
    if (k >= 200) {
      r += 1;
      k = 0;
    }
  }

}
#line 1 "/a0/usr/workdir/BKOS5a/BKOS_update.ino"
void update_center(String actie) {
  if (actie == "bouw") {
    bouw_update();
  } else if (actie == "run") {
    run_update();
  } else if (actie == "naam") {
    item_naam = "Updates";
  } else if (actie == "print") {
    update_center("naam");
    tft.print(item_naam);
  } else if (actie == "icon_klein") {
    int x = getCursorX() + 20;
    int y = getCursorY() + 25;
    setCursor(getCursorX(), getCursorY()+4);
    drawIcon(x, y, 30, kleur_zwart, icon_30_S_update, sizeof(icon_30_S_update)/sizeof(byte));
    tft.setTextColor(kleur_rood);
  }
}


void bouw_update() {

  delete[]knoppen_teken_positie;
  delete[]knoppen_positie;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst;
  delete[]knoppen_tekst_kleur;
  
  aantal_knoppen = 2;
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = new byte[aantal_knoppen];

  BKOS_GIT_ALLOWED = fetchAlowedVersions();
  BKOS5_VERSIE_GIT = fetchVersion5();  // BKOS5a versie ophalen
  String deler = "\n";
  String sonderdeel;
  char conderdeel[10];
  int p;
  while (deler.indexOf(BKOS_GIT_ALLOWED) > 0) {
    p = deler.indexOf(BKOS_GIT_ALLOWED);
    sonderdeel = BKOS_GIT_ALLOWED.substring(0, p);
    sonderdeel.toCharArray(allowed_versions[aantal_knoppen - 2], 10);
    // allowed_versions[aantal_knoppen - 2] = conderdeel;
    BKOS_GIT_ALLOWED = BKOS_GIT_ALLOWED.substring(p+1);
    aantal_knoppen++;
  }
  sonderdeel = BKOS_GIT_ALLOWED;
  sonderdeel.toCharArray(allowed_versions[aantal_knoppen - 2], 10);
  // allowed_versions[aantal_knoppen-2] = conderdeel;
  aantal_knoppen++;

  for (int i  = 0 ; i < aantal_knoppen ; i++) {
    knoppen_positie[i] = update_knoppen_positie[i];
    knoppen_teken_positie[i] = update_knoppen_positie[i];
    // knoppen_tekst[i] = io_namen[io_knoppen[i]];
    if (i == 0) {
      knoppen_tekst[i] = "BKOS 4";      
    } else if (i == 1) {
      knoppen_tekst[i] = "BKOS 5";      
    } else {
      knoppen_tekst[i] = allowed_versions[i - 2];
    }
    knoppen_status[i] = 0;
    knoppen_basiskleur[i] = schakelscherm_knoppen_kleur;
    knoppen_tekst_kleur[i] = schakelscherm_knoppen_tekst_kleur;
    
  }

  header_plaatsen("Instellingen");
  achtergrond(kleur_donker, true);
  
  tft.setTextSize(3);
  tft.setTextColor(kleur_licht);
  setCursor(0, 40);

  tft.print("  Actuele versie: ");
  tft.println(BKOS_VERSIE);
  tft.setTextSize(2);
  tft.println("   In de testperiode wordt BKos automatisch geupdate");
  tft.println("   U kunt wel kiezen tussen versie 4 en 5");
  tft.println("");
  tft.println("   Versie 4 betreft de handmatig gecodeerde versie");
  tft.println("   Versie 5 wordt de AI gecodeerde versie");
  tft.println("");
  tft.println("   Klik om naar de gewenste versie te gaan:");

  alle_knoppen_plaatsen();

  tft.println("");
  tft.println("   Allowed versions:");
  tft.println(BKOS_GIT_ALLOWED);
  tft.print("   ");
  tft.print("'");
  tft.print(aantal_knoppen);
  tft.print("' knoppen gevonden");

  // // bouw_io_instellingen(0);
  // header_plaatsen("Instellingen");
  // achtergrond(tft.color565(0, 0, 0), false);
  // center_tekst(120, 100, "Nog niet gemaakt", 2, tft.color565(255, 255, 255));
  // center_tekst(120, 150, "Tik om terug te gaan", 1, tft.color565(255, 255, 255));
}


String fetchVersion4() {
  HTTPClient http;
  http.begin("https://raw.githubusercontent.com/brennyc86/BKOS4/main/firmware/versie4.txt");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String v = http.getString();
    v.trim();
    http.end();
    return v;
  }
  http.end();
  return "latest";
}
void run_update() {
  int druk;
  if (actieve_touch) {
    int x = touch_x();
    int y = touch_y();

    druk = klik(ts_x, ts_y);

    if (druk == 0) {
      // Installeren BKOS 4
      // Eerst de scherm opmaak even doen en daarna de installatie starten voor BKOS 4
      updaten = true;
      delay(250);
      tft.fillScreen(kleur_zwart);
      bkos_logo(10, 10, kleur_donker);
      tft.setCursor(300, 10);
      tft.setTextSize(3);
      tft.setTextColor(kleur_groen);
      tft.println("BKOS update");
      tft.setTextColor(kleur_beige);
      tft.setCursor(300, 45);
      tft.print(BKOS_VERSIE);
      tft.print(" > ");
      tft.println(BKOS_VERSIE_GIT);
      String target4 = fetchVersion4();
      tft.setCursor(300, 70);
      tft.setTextSize(2);
      tft.setTextColor(kleur_beige);
      tft.print("Updating to BKOS4 ");
      tft.println(target4);
      downloadAndApplyFirmware(4);
    } else if (druk == 1) {
      // Installeren BKOS 5a
      updaten = true;
      delay(250);
      tft.fillScreen(kleur_zwart);
      bkos_logo(10, 10, kleur_donker);
      tft.setCursor(300, 10);
      tft.setTextSize(3);
      tft.setTextColor(kleur_groen);
      tft.println("BKOS update");
      tft.setTextColor(kleur_beige);
      tft.setCursor(300, 45);
      tft.print(BKOS_VERSIE);
      tft.print(" > ");
      tft.println(BKOS5_VERSIE_GIT.length() > 0 ? BKOS5_VERSIE_GIT : "5a");
      downloadAndApplyFirmware(5);
    }

    if (ts_y >= home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (ts_y < 30) {
      klik_header(ts_x, ts_y);
    }
  }
}
#line 1 "/a0/usr/workdir/BKOS5a/KNOPPEN.ino"
// BKOS5a: Visuele touch feedback - korte kleurflits bij aanraking
void knop_feedback(int knop_nummer) {
  if (knop_nummer < 0 || knop_nummer >= aantal_knoppen) return;
  // Teken knop tijdelijk lichter
  uint16_t feedback_kleur = tft.color565(235, 235, 100); // beige flits
  fillRoundRect(
    knoppen_teken_positie[knop_nummer][0],
    knoppen_teken_positie[knop_nummer][1],
    knoppen_teken_positie[knop_nummer][2],
    knoppen_teken_positie[knop_nummer][3],
    5, feedback_kleur);
  delay(60);
  // Herstel naar normale staat
  knop_plaatsen(knop_nummer, true);
}

void knop_plaatsen(int knop_nummer) {
  knop_plaatsen(knop_nummer, false);
}

void knop_plaatsen(int knop_nummer, bool tekst) {
knop_plaatsen(knop_nummer, tekst, knoppen_basiskleur[knop_nummer][knoppen_status[knop_nummer]]);
}

void knop_plaatsen(int knop_nummer, bool tekst, uint16_t basis_kleur) {
  fillRoundRect(knoppen_teken_positie[knop_nummer][0], knoppen_teken_positie[knop_nummer][1], knoppen_teken_positie[knop_nummer][2], knoppen_teken_positie[knop_nummer][3], 5, basis_kleur);
  // tft.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
  if ((knoppen_tekst[knop_nummer][0] == '*') && (knoppen_tekst[knop_nummer][1] == '*')){
    if ((knoppen_tekst[knop_nummer][2] == 'U') && (knoppen_tekst[knop_nummer][3] == 'S') && (knoppen_tekst[knop_nummer][4] == 'B')){
      // USB
      // drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 7, knoppen_teken_positie[knop_nummer][1]+10, 15, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]], icon_15_usb, sizeof(icon_15_usb)/sizeof(byte));
      drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_usb, sizeof(icon_30_usb)/sizeof(byte));
    } else if ((knoppen_tekst[knop_nummer][2] == '2') && (knoppen_tekst[knop_nummer][3] == '3') && (knoppen_tekst[knop_nummer][4] == '0')){
      // 230 volt AC
      // drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 7, knoppen_teken_positie[knop_nummer][1]+10, 15, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]], icon_15_230, sizeof(icon_15_230)/sizeof(byte));
      if (knoppen_status[knop_nummer] == 1) {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_geel, icon_30_230, sizeof(icon_30_230)/sizeof(byte));
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_oranje, icon_30_230, sizeof(icon_30_230)/sizeof(byte));
      }
    } else if ((knoppen_tekst[knop_nummer][2] == 't') && (knoppen_tekst[knop_nummer][3] == 'v')){
      // TB
      drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]], icon_30_tv, sizeof(icon_30_tv)/sizeof(byte));
    } else if ((knoppen_tekst[knop_nummer][2] == 'L') && (knoppen_tekst[knop_nummer][3] == '_') && (knoppen_tekst[knop_nummer][4] == 'n') && (knoppen_tekst[knop_nummer][5] == 'a') && (knoppen_tekst[knop_nummer][6] == 'v') && (knoppen_tekst[knop_nummer][7] == 'i')){
      // Navigatie verlichting
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_actief_rood, icon_30_L_navi_1, sizeof(icon_30_L_navi_1)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_actief_groen, icon_30_L_navi_2, sizeof(icon_30_L_navi_2)/sizeof(byte));
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_passief_rood, icon_30_L_navi_1, sizeof(icon_30_L_navi_1)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_passief_groen, icon_30_L_navi_2, sizeof(icon_30_L_navi_2)/sizeof(byte));
      }
    } else if ((knoppen_tekst[knop_nummer][2] == 'L') && (knoppen_tekst[knop_nummer][3] == '_') && (knoppen_tekst[knop_nummer][4] == '3') && (knoppen_tekst[knop_nummer][5] == 'k') && (knoppen_tekst[knop_nummer][6] == 'l') && (knoppen_tekst[knop_nummer][7] == ' ')){
      // 3 kleuren verlichting
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_actief_rood, icon_30_L_3kl_1, sizeof(icon_30_L_3kl_1)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_actief_groen, icon_30_L_3kl_2, sizeof(icon_30_L_3kl_2)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_wit, icon_30_L_3kl_3, sizeof(icon_30_L_3kl_2)/sizeof(byte));
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_passief_rood, icon_30_L_3kl_1, sizeof(icon_30_L_3kl_1)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_passief_groen, icon_30_L_3kl_2, sizeof(icon_30_L_3kl_2)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_zwart, icon_30_L_3kl_3, sizeof(icon_30_L_3kl_3)/sizeof(byte));
      }
    } else if ((knoppen_tekst[knop_nummer][2] == 'L') && (knoppen_tekst[knop_nummer][3] == '_') && (knoppen_tekst[knop_nummer][4] == 's') && (knoppen_tekst[knop_nummer][5] == 't') && (knoppen_tekst[knop_nummer][6] == 'o') && (knoppen_tekst[knop_nummer][7] == 'o') && (knoppen_tekst[knop_nummer][8] == 'm')){
      // stoomlicht
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_wit, icon_30_L_stoom, sizeof(icon_30_L_stoom)/sizeof(byte));
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, (knoppen_teken_positie[knop_nummer][1]+5), 30, kleur_zwart, icon_30_L_stoom, sizeof(icon_30_L_stoom)/sizeof(byte));
      }
    } else if ((knoppen_tekst[knop_nummer][2] == 'L') && (knoppen_tekst[knop_nummer][3] == '_') && (knoppen_tekst[knop_nummer][4] == 'a') && (knoppen_tekst[knop_nummer][5] == 'n') && (knoppen_tekst[knop_nummer][6] == 'k') && (knoppen_tekst[knop_nummer][7] == 'e') && (knoppen_tekst[knop_nummer][8] == 'r')){
      // Ankerlicht
      if (knoppen_status[knop_nummer] == 1){
        fillCircle((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2), (knoppen_teken_positie[knop_nummer][1]+knoppen_teken_positie[knop_nummer][3]/2), 15, kleur_wit);
        fillCircle((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2), (knoppen_teken_positie[knop_nummer][1]+knoppen_teken_positie[knop_nummer][3]/2), 2, kleur_zwart);
        // drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_wit, icon_30_L_anker, sizeof(icon_30_L_anker)/sizeof(byte));
      } else {
        fillCircle((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2), (knoppen_teken_positie[knop_nummer][1]+knoppen_teken_positie[knop_nummer][3]/2), 15, kleur_zwart);
        fillCircle((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2), (knoppen_teken_positie[knop_nummer][1]+knoppen_teken_positie[knop_nummer][3]/2), 2, kleur_wit);
        // drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_L_anker, sizeof(icon_30_L_anker)/sizeof(byte));
      }
    } else if (charstrip(knoppen_tekst[knop_nummer]) == "**anker"){
      // Anker
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_actief_groen, icon_30_M_anker, sizeof(icon_30_M_anker)/sizeof(byte));  
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_M_anker, sizeof(icon_30_M_anker)/sizeof(byte));
      }
      
    } else if (charstrip(knoppen_tekst[knop_nummer]) == "**zeilen"){
      // Anker
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_actief_groen, icon_30_M_zeilen, sizeof(icon_30_M_zeilen)/sizeof(byte));  
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_M_zeilen, sizeof(icon_30_M_zeilen)/sizeof(byte));
      }
      
    } else if (charstrip(knoppen_tekst[knop_nummer]) == "**motor"){
      // Anker
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_actief_groen, icon_30_M_motor, sizeof(icon_30_M_motor)/sizeof(byte));  
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_M_motor, sizeof(icon_30_M_motor)/sizeof(byte));
      }
      
    } else if (charstrip(knoppen_tekst[knop_nummer]) == "**E_dek"){
      // Deklicht - spotlight in de mast die het voordek verlicht
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_wit, icon_30_dek, sizeof(icon_30_dek)/sizeof(byte));
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_dek, sizeof(icon_30_dek)/sizeof(byte));
      }
    } else if (charstrip(knoppen_tekst[knop_nummer]) == "**haven"){

      // Anker
      if (knoppen_status[knop_nummer] == 1){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_geel, icon_30_M_zon, sizeof(icon_30_M_zon)/sizeof(byte));  
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) + 5, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_actief_groen, icon_30_M_haven, sizeof(icon_30_M_haven)/sizeof(byte));
        drawLine((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 5, knoppen_teken_positie[knop_nummer][1]+35, (knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) + 5, knoppen_teken_positie[knop_nummer][1]+5, kleur_actief_groen);
      } else {
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_geel, icon_30_M_zon, sizeof(icon_30_M_zon)/sizeof(byte));  
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) + 5, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_zwart, icon_30_M_haven, sizeof(icon_30_M_haven)/sizeof(byte));
        drawLine((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 5, knoppen_teken_positie[knop_nummer][1]+35, (knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) + 5, knoppen_teken_positie[knop_nummer][1]+5, kleur_zwart);
      }
    } else if (charstrip(knoppen_tekst[knop_nummer]) == "**I_licht"){
      // Anker
      if (knoppen_status[knop_nummer] == 3){
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 15, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_wit, icon_30_IL_2K_1, sizeof(icon_30_IL_2K_1)/sizeof(byte));
        drawIcon((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2) - 13, knoppen_teken_positie[knop_nummer][1]+5, 30, kleur_rood, icon_30_IL_2K_2, sizeof(icon_30_IL_2K_2)/sizeof(byte));
      } else {
        uint32_t kleur = kleur_wit;
        if (knoppen_status[knop_nummer] == 0) {
          kleur = kleur_zwart;
        } else if (knoppen_status[knop_nummer] == 1) {
          kleur = kleur_wit;
        } else if (knoppen_status[knop_nummer] == 2) {
          kleur = kleur_rood;
        }
        fillCircle((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2), (knoppen_teken_positie[knop_nummer][1]+knoppen_teken_positie[knop_nummer][3]/2), 15, kleur);
      }
      
    } else if ((knoppen_tekst[knop_nummer][2] == 'T') && (knoppen_tekst[knop_nummer][3] == 'B')) {
      uint32_t kleur = kleur_wit;
      if ((knoppen_tekst[knop_nummer][5] == 'r') && (knoppen_tekst[knop_nummer][6] == 'o') && (knoppen_tekst[knop_nummer][7] == 'o') && (knoppen_tekst[knop_nummer][8] == 'd')) {
        kleur = kleur_actief_rood;
      } else if ((knoppen_tekst[knop_nummer][5] == 'b') && (knoppen_tekst[knop_nummer][6] == 'l') && (knoppen_tekst[knop_nummer][7] == 'a') && (knoppen_tekst[knop_nummer][8] == 'u')) {
        kleur = kleur_blauw;
      } else if ((knoppen_tekst[knop_nummer][5] == 'g') && (knoppen_tekst[knop_nummer][6] == 'r') && (knoppen_tekst[knop_nummer][7] == 'o') && (knoppen_tekst[knop_nummer][8] == 'e')) {
        kleur = kleur_passief_groen;
      } else if ((knoppen_tekst[knop_nummer][5] == 'g') && (knoppen_tekst[knop_nummer][6] == 'e') && (knoppen_tekst[knop_nummer][7] == 'e') && (knoppen_tekst[knop_nummer][8] == 'l')) {
        kleur = kleur_geel;
      }
      fillCircle((knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2), (knoppen_teken_positie[knop_nummer][1]+knoppen_teken_positie[knop_nummer][3]/2), 15, kleur);

    } else {
      center_tekst(knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2, knoppen_teken_positie[knop_nummer][1]+10, knoppen_tekst[knop_nummer], 1.5, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]]); // tijdelijk
    }
  // } else if ((tekst)) & (!knoppen_tekst2[knop_nummer] != "          ")) {
  //   center_tekst(knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2, knoppen_teken_positie[knop_nummer][1]+5, knoppen_tekst[knop_nummer], 1.5, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]]); // tijdelijk
  //   center_tekst(knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2, knoppen_teken_positie[knop_nummer][1]+20, knoppen_tekst2[knop_nummer], 1.5, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]]); // tijdelijk
  } else {
    center_tekst(knoppen_teken_positie[knop_nummer][0]+knoppen_teken_positie[knop_nummer][2]/2, knoppen_teken_positie[knop_nummer][1]+10, knoppen_tekst[knop_nummer], 1.5, knoppen_tekst_kleur[knop_nummer][knoppen_status[knop_nummer]]); // tijdelijk
  }
  // tft.setCursor(scherm_x(knoppen_teken_positie[knop_nummer][0] * 2), scherm_y(knoppen_teken_positie[knop_nummer][1] * 2));
  // tft.println(knoppen_tekst[knop_nummer]);
  
}


void alle_knoppen_plaatsen() {
  alle_knoppen_plaatsen(false);
}
void alle_knoppen_plaatsen(bool tekst) {
  for (byte i = 0; i < aantal_knoppen; i++) {
    knop_plaatsen(i, tekst);
  }
}

int klik(int x, int y) {
  for (byte i = 0; i < aantal_knoppen; i++) {
    if ((x >= knoppen_positie[i][0]) && (x <= knoppen_positie[i][0]+knoppen_positie[i][2]) && (y >= knoppen_positie[i][1]) && (y <= knoppen_positie[i][1]+knoppen_positie[i][3])) {
      return i;
    }
  }
  return -1;
}


void homeknop_plaatsen() {
  fillRect(home_knop[0], home_knop[1], home_knop[2], home_knop[3], kleur_home_knop);
  center_tekst(home_knop[0] + home_knop[2]/2, home_knop[1] + 10, "Home", 2, kleur_home_tekst);
  homeknop_actief = true;
  homeknop_half_actief = false;
}

void halve_homeknop_plaatsen() {
  fillRect(home_knop[0], home_knop[1], home_knop[2]/2 -4, home_knop[3], kleur_home_knop);
  center_tekst(home_knop[0] + home_knop[2]/4 - 2, home_knop[1] + 10, "Home", 2, kleur_home_tekst);
  
  fillRect(home_knop[0] + home_knop[2]/2 -4, home_knop[1], 8 , home_knop[3], kleur_home_tekst);
}

void halve_2eknop_plaatsen(String tekst) {
  fillRect(home_knop[0] + home_knop[2]/2 + 4, home_knop[1], home_knop[2]/2 -4, home_knop[3], kleur_home_knop);
  center_tekst(home_knop[0] + home_knop[2]/2 + 4 + home_knop[2]/4 - 2, home_knop[1] + 10, tekst, 2, kleur_home_tekst);
}




void homeknop_plaatsen(bool invert) {
  if (invert) {
    fillRect(home_knop[0], home_knop[1], home_knop[2], home_knop[3], kleur_licht);
    center_tekst(home_knop[0] + home_knop[2]/2, home_knop[1] + 10, "Home", 2, kleur_donker);
  } else {
    fillRect(home_knop[0], home_knop[1], home_knop[2], home_knop[3], kleur_donker);
    center_tekst(home_knop[0] + home_knop[2]/2, home_knop[1] + 10, "Home", 2, kleur_licht);
  }
  homeknop_actief = true;
  homeknop_half_actief = false;
}

#line 1 "/a0/usr/workdir/BKOS5a/app_configmodus.ino"
void configmodus(String actie) {
  if (actie == "bouw") {
    // Serial.println("Bouw");
    bouw_configmodus();
  } else if (actie == "run") {
    run_configmodus();
  } else if (actie == "naam") {
    item_naam = ("Config");
  } else if (actie == "print") {
    configmodus("naam");
    tft.print(item_naam);
  } else if (actie == "icon_groot") {
    int x = getCursorX() + 20;
    int y = getCursorY() + 25;
    setCursor(getCursorX(), getCursorY()+4);
    drawIcon10x10(x, y, icon_instellingen, kleur_donker, 3);
    tft.setTextColor(kleur_rood);
  } else if (actie == "icon_klein") {
    int x = getCursorX() + 20;
    int y = getCursorY() + 25;
    setCursor(getCursorX(), getCursorY()+4);
    drawIcon10x10(x, y, icon_instellingen, kleur_donker, 3);
    tft.setTextColor(kleur_rood);
  }
}

void bouw_configmodus() {

  // header_plaatsen("Schakelscherm");
  achtergrond(kleur_licht);

  aantal_knoppen = 8;
  
  delete[]knoppen_positie;
  delete[]knoppen_teken_positie;
  delete[]knoppen_tekst;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst_kleur;
  delete[]knoppen_status;

  
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = new byte[aantal_knoppen];

  huidige_configuratie = EEPROM.read(16);


  for (int i  = 0 ; i < aantal_knoppen ; i++) {
    knoppen_positie[i] = configmodus_knoppen_positie[i];
    knoppen_teken_positie[i] = configmodus_knoppen_positie[i];
    knoppen_tekst[i] = configmodus_knoppen_namen[i];
    if (i == huidige_configuratie) {
      knoppen_status[i] = 1; //exterieurscherm_status[i];
    } else if (i == 7) {
      knoppen_status[i] = 2;
    } else {
      knoppen_status[i] = 0;
    }
    knoppen_basiskleur[i] = schakelscherm_knoppen_kleur;
    knoppen_tekst_kleur[i] = schakelscherm_knoppen_tekst_kleur;
    
  }
  alle_knoppen_plaatsen();

  tft.setCursor(200, 200);
  tft.setTextSize(4);
  tft.println(huidige_configuratie);

}

// void exterieur_teken_boot(int32_t x, int32_t y, uint32_t kleur){
//   tekenItem(x, y, kleur_groen, teken_boot, sizeof(teken_boot)/sizeof(int)/2);
//   tekenCircels(x, y, kleur_groen, circels_boot, sizeof(circels_boot)/sizeof(int)/3);
//   tekenTeksten(x, y, kleur_groen, teken_tekst_positie_boot, teken_tekst_boot, sizeof(teken_tekst_positie_boot)/sizeof(int)/3);
// }

// void exterieur_symbolen_verlichting(int32_t x, int32_t y){

//   int32_t ix;
//   int32_t iy;
//   bool gevonden;
//   // hek
//   ix = 0;
//   iy = 150;
//   for (int i = 0; i < io_knoppen_cnt; i++){
//     if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'h') && (io_namen[io_knoppen[i]][5] == 'e') && (io_namen[io_knoppen[i]][6] == 'k')) {
//       if (io_input[io_knoppen[i]] == 1) {
//         if (io_output[io_knoppen[i]] == 1) {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
//         } else {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
//         }
//       } else if (io_output[io_knoppen[i]] == 1) {
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
//       } else {
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
//       }
//     }
//   }

//   // top
//   ix = 67;
//   iy = 0;
//   gevonden = false;
//   for (int i = 0; i < io_knoppen_cnt; i++){
//     if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == '3') && (io_namen[io_knoppen[i]][5] == 'k') && (io_namen[io_knoppen[i]][6] == 'l')) {
//       if (io_input[io_knoppen[i]] == 1) {
//         if (io_output[io_knoppen[i]] == 1) {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
//           gevonden = true;
//         } else {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
//         }
//       } else if (io_output[io_knoppen[i]] == 1) {
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
//         gevonden = true;
//       }
//     }
//   }
//   for (int i = 0; i < io_knoppen_cnt; i++){
//     if (!gevonden) {
//       if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'a') && (io_namen[io_knoppen[i]][5] == 'n') && (io_namen[io_knoppen[i]][6] == 'k') && (io_namen[io_knoppen[i]][7] == 'e') && (io_namen[io_knoppen[i]][8] == 'r')){
//         if (io_input[io_knoppen[i]] == 1){
//           if (io_output[io_knoppen[i]] == 1) {
//             drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
//           } else {
//             drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
//           }
//         } else if (io_output[io_knoppen[i]] == 1) {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
//         } else {
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
//         }
//       }
//     }
//   }

//   // stoom
//   ix = 75;
//   iy = 75;
//   for (int i = 0; i < io_knoppen_cnt; i++){
//     if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 's') && (io_namen[io_knoppen[i]][5] == 't') && (io_namen[io_knoppen[i]][6] == 'o') && (io_namen[io_knoppen[i]][7] == 'o') && (io_namen[io_knoppen[i]][8] == 'm')) {
//       if (io_input[io_knoppen[i]] == 1){
//         if (io_output[io_knoppen[i]] == 1) {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
//         } else {
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
//         }
//       } else if (io_output[io_knoppen[i]] == 1) {
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
//       } else {
//       drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
//       }
//     }
//   }

//   // boordlichten
//   ix = 80;
//   iy = 143;
//   for (int i = 0; i < io_knoppen_cnt; i++){
//     if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'n') && (io_namen[io_knoppen[i]][5] == 'a') && (io_namen[io_knoppen[i]][6] == 'v') && (io_namen[io_knoppen[i]][7] == 'i')) {
//       if (io_input[io_knoppen[i]] == 1){
//         if (io_output[io_knoppen[i]] == 1) {
//           drawIcon(x+ix - 15, y+iy-23 , 30, kleur_actief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
//         } else {
//           drawIcon(x+ix - 15, y+iy-23 , 30, kleur_oranje, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
//           drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
//         }
//       } else if (io_output[io_knoppen[i]] == 1) {
//         drawIcon(x+ix - 15, y+iy-23 , 30, kleur_passief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
//         drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
//       } else {
//       drawIcon(x+ix - 15, y+iy-23 , 30, kleur_zwart, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
//       drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
//       }
//     }
//   }
// }

void run_configmodus() {
  int druk;
  if (actieve_touch) {
    druk = klik(ts_x, ts_y);

    if (druk > -1){
      if (druk < 7) {
        knoppen_status[huidige_configuratie] = 0;
        knop_plaatsen(huidige_configuratie);
        huidige_configuratie = druk;
        knoppen_status[druk] = 1;
        knop_plaatsen(druk);
      } else if (druk == 7) {
        if (huidige_configuratie != EEPROM.read(16)){
          tft.fillScreen(kleur_zwart);
          tft.setCursor(200, 200);
          tft.setTextSize(5);
          tft.setTextColor(kleur_wit);
          tft.print(EEPROM.read(16));
          tft.print(" > ");
          tft.print(huidige_configuratie);
          delay(1000);
          EEPROM.write(16, huidige_configuratie);
          delay(2000);
          ESP.restart();
        }
      }
    }

    while (ts_touched()) {
      ts_begin();
    }

    if (ts_y >= home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (ts_y < 30) {
      klik_header(ts_x, ts_y);
    }
  }
  // if (io_runned) {
  //   io_runned = false;
  //   exterieur_symbolen_verlichting(ext_x, ext_y);
  // }
  
  ts_begin(); 
}

// void exterieurscherm_schakel(int knop) {
//   if (knop > 3) {
//     if (charstrip(exterieur_knoppen_namen[knop]) == "**I_licht") {
//       interieur_verlichting(true);
//       exterieurscherm_status[knop] = interieur_licht_variant;
//       knoppen_status[knop] = interieur_licht_variant;
//       // knop_plaatsen(knop);
//     } else {
//       if (knoppen_status[knop] == 0) {
//         // uit wordt aan
//         knoppen_status[knop] = 1;
//         exterieurscherm_status[knop] = 1;
//         for (int i = 0; i < io_knoppen_cnt; i++){
//           int j = 0;
//           bool fl = false;
//           bool stp = false;
//           while (!stp){
//             if (io_namen[io_knoppen[i]][j] == knoppen_tekst[knop][j]) {
//               stp = false;
//             } else {
//               fl = true;
//               stp = true;
//             }
//             if (j >= 9){
//               stp = true;
//             }
//             j++;
//           }
//           if (!fl) {
//             if ((io_output[io_knoppen[i]] == 0) || (io_output[io_knoppen[i]] == 1)){
//               io_output[io_knoppen[i]] = 1;
//             } else if ((io_output[io_knoppen[i]] == 2) || (io_output[io_knoppen[i]] == 3)){
//               io_output[io_knoppen[i]] = 3;
//             }
//           }
//         }
//       } else{
//         // aan wordt uit
//         knoppen_status[knop] = 0;
//         exterieurscherm_status[knop] = 0;
//         for (int i = 0; i < io_knoppen_cnt; i++){
//           int j = 0;
//           bool fl = false;
//           bool stp = false;
//           while (!stp){
//             if (io_namen[io_knoppen[i]][j] == knoppen_tekst[knop][j]) {
//               stp = false;
//             } else {
//               fl = true;
//               stp = true;
//             }
//             if (j >= 9){
//               stp = true;
//             }
//             j++;
//           }
//           if (!fl) {
//             if ((io_output[io_knoppen[i]] == 0) || (io_output[io_knoppen[i]] == 1)){
//               io_output[io_knoppen[i]] = 0;
//             } else if ((io_output[io_knoppen[i]] == 2) || (io_output[io_knoppen[i]] == 3)){
//               io_output[io_knoppen[i]] = 2;
//             }
//           }
//         }
//       }
//     }
//     // knop_plaatsen(knop);
//   } else {
//     for (int i = 0; i < 4; i++) {
//       if (i == knop) {
//         if (knoppen_status[i] == 1) {
//           exterieur_licht_variant++;
//         } else {
//           knoppen_status[i] = 1;
//           exterieurscherm_status[i] = 1;
//           exterieur_licht_variant = 0;
//         }
//       } else {
//         knoppen_status[i] = 0;
//         exterieurscherm_status[i] = 0;
//       }
//       knop_plaatsen(i);
//     }
//     if (knop == 0) {
//       exterieur_verlichting_uit();
//       interieur_verlichting();
//     } else if (knop == 1) {
//       exterieur_zeilverlichting();
//       interieur_verlichting();
//     } else if (knop == 2) {
//       exterieur_motorverlichting();
//       interieur_verlichting();
//     } else if (knop == 3) {
//       exterieur_ankerverlichting();
//       interieur_verlichting();
//     }
    
//   }
//   knop_plaatsen(knop);
//   exterieur_symbolen_verlichting(ext_x, ext_y);
//   io_now = true;
//   // io();
//   // exterieur_symbolen_verlichting(ext_x, ext_y);
//   // delay(50);
//   // io();
//   // exterieur_symbolen_verlichting(ext_x, ext_y);
// }

// void exterieur_verlichting_uit(){
//   for (int knop = 0; knop < io_knoppen_cnt; knop++){
//     if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
//       if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//         io_output[io_knoppen[knop]] = 0;
//       } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//         io_output[io_knoppen[knop]] = 2;
//       }
//     }
//   }
// }

// void exterieur_zeilverlichting(){
//   bool aan_uit;
//   for (int knop = 0; knop < io_knoppen_cnt; knop++){
//     if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
//       aan_uit = false;
//       if (exterieur_licht_variant > 1) {
//         exterieur_licht_variant = 0;
//       }
//       if (exterieur_licht_variant == 0) {
//         // Enkel de 3 kleurenlamp aan ( **L_3kl)
//         if ((io_namen[io_knoppen[knop]][4] == '3') && (io_namen[io_knoppen[knop]][5] == 'k') && (io_namen[io_knoppen[knop]][6] == 'l')){
//           aan_uit = true;
//         }
//       } else if (exterieur_licht_variant == 1) {
//         // navigatie verlichting en heklicht aan (**L_navi  & **L_hek)
//         if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
//           aan_uit = true;
//         } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
//           aan_uit = true;
//         }
//       } 
      
//       if (aan_uit) {
//         if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//           io_output[io_knoppen[knop]] = 1;
//         } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//           io_output[io_knoppen[knop]] = 3;
//         }
//       } else {
//         if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//           io_output[io_knoppen[knop]] = 0;
//         } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//           io_output[io_knoppen[knop]] = 2;
//         }
//       }
//     }
//   }
// }

// void exterieur_motorverlichting(){
//   bool aan_uit;
//   for (int knop = 0; knop < io_knoppen_cnt; knop++){
//     if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
//       aan_uit = false;
//       if (exterieur_licht_variant > 2) {
//         exterieur_licht_variant = 0;
//       }
//       if (exterieur_licht_variant == 0) {
//         // ankerlicht en navigatie verlichting (**L_anker en **L_navi)
//         if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
//           aan_uit = true;
//         } else if ((io_namen[io_knoppen[knop]][4] == 'a') && (io_namen[io_knoppen[knop]][5] == 'n') && (io_namen[io_knoppen[knop]][6] == 'k') && (io_namen[io_knoppen[knop]][7] == 'e') && (io_namen[io_knoppen[knop]][8] == 'r')){
//           aan_uit = true;
//         }
//       } else if (exterieur_licht_variant == 1) {
//         // navigatie verlichting, stoomlicht en heklicht aan (**L_navi, **L_stoom  & **L_hek)
//         if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
//           aan_uit = true;
//         } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
//           aan_uit = true;
//         } else if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
//           aan_uit = true;
//         }
//       }  else if (exterieur_licht_variant == 2) {
//         // 3 kleurenlicht en stoomlicht aan (**L_3kl & **L_stoom)
//         if ((io_namen[io_knoppen[knop]][4] == '3') && (io_namen[io_knoppen[knop]][5] == 'k') && (io_namen[io_knoppen[knop]][6] == 'l')){
//           aan_uit = true;
//         } else if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
//           aan_uit = true;
//         }
//       } 
      
//       if (aan_uit) {
//         if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//           io_output[io_knoppen[knop]] = 1;
//         } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//           io_output[io_knoppen[knop]] = 3;
//         }
//       } else {
//         if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//           io_output[io_knoppen[knop]] = 0;
//         } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//           io_output[io_knoppen[knop]] = 2;
//         }
//       }
//     }
//   }
// }

// void exterieur_ankerverlichting(){
//   bool aan_uit;
//   for (int knop = 0; knop < io_knoppen_cnt; knop++){
//     if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
//       aan_uit = false;
//       if (exterieur_licht_variant > 1) {
//         exterieur_licht_variant = 0;
//       }
//       if (exterieur_licht_variant == 0) {
//         // Enkel het ankerlicht aan ( **L_anker)
//         if ((io_namen[io_knoppen[knop]][4] == 'a') && (io_namen[io_knoppen[knop]][5] == 'n') && (io_namen[io_knoppen[knop]][6] == 'k') && (io_namen[io_knoppen[knop]][7] == 'e') && (io_namen[io_knoppen[knop]][8] == 'r')){
//           aan_uit = true;
//         }
//       } else if (exterieur_licht_variant == 1) {
//         // stoomlicht en heklicht aan (**L_stoom  & **L_hek)
//         if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
//           aan_uit = true;
//         } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
//           aan_uit = true;
//         }
//       }   
      
//       if (aan_uit) {
//         if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//           io_output[io_knoppen[knop]] = 1;
//         } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//           io_output[io_knoppen[knop]] = 3;
//         }
//       } else {
//         if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
//           io_output[io_knoppen[knop]] = 0;
//         } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
//           io_output[io_knoppen[knop]] = 2;
//         }
//       }
//     }
//   }
// }

// void interieur_verlichting() {
//   interieur_verlichting(false);
// }

// void interieur_verlichting(bool schakelen) {
//   if (schakelen) {
//     interieur_licht_variant ++;
//   }
//   if (interieur_licht_variant >= 4) {
//     interieur_licht_variant = 0;
//   }
//   // tft.fillRect(10, 30, 100, 30, kleur_zwart);
//   // tft.setTextColor(kleur_wit);
//   // tft.setCursor(10, 31);
//   // tft.print(interieur_licht_variant);

//   int wit = 0;
//   int rood = 0;

//   if (interieur_licht_variant == 0) {
//     wit = 0;
//     rood = 0;
//   } else if (interieur_licht_variant == 1) {
//     wit = 1;
//     rood = 0;
//   } else if (interieur_licht_variant == 2) {
//     wit = 0;
//     rood = 1;
//   } else if (interieur_licht_variant == 3) {
//     for (int i = 0; i < sizeof(exterieurscherm_status); i++) {
//       if (exterieurscherm_status[i] == 1) {
//         if (charstrip(exterieur_knoppen_namen[i]) == "**haven") {
//           // tft.print("haven");
//           wit = 1;
//           rood = 0;
//         } else if (charstrip(exterieur_knoppen_namen[i]) == "**zeilen") {
//           // tft.print("zeilen");
//           wit = 0;
//           rood = 1;
//         } else if (charstrip(exterieur_knoppen_namen[i]) == "**motor") {
//           // tft.print("motor");
//           wit = 0;
//           rood = 1;
//         } else if (charstrip(exterieur_knoppen_namen[i]) == "**anker") {
//           // tft.print("anker");
//           wit = 1;
//           rood = 0;
//         // } else {
//         //   tft.print(i);
//         //   tft.print("[");
//         //   tft.print(charstrip(exterieur_knoppen_namen[i]));
//         //   tft.print("]");
//         }
//       }
//     }
//   }

//   for (int knop = 0; knop < io_knoppen_cnt; knop++){
//     if (io_namen[io_knoppen[knop]] == "**IL_wit "){
//       io_output[io_knoppen[knop]] = wit;
//     } else if (io_namen[io_knoppen[knop]] == "**IL_rood"){
//       io_output[io_knoppen[knop]] = rood;
//     }
//   }
// }


// String charstrip(char invoer[]) {
//   String uitvoer = "";
//   for (int i = 0; i < 10; i++){
//     if (invoer[i] != ' '){
//       uitvoer += invoer[i];
//     }
//   }
//   uitvoer.trim();
//   return uitvoer;
// }
#line 1 "/a0/usr/workdir/BKOS5a/apps.ino"
void app_uitvoeren() {
  ts_begin();
  if (scherm_bouwen) {
    homeknop_actief = true;
    homeknop_half_actief = false;
    if (DP_DRIVER == 2) {
      achtergrond(kleur_donker);
    }
    apps[actieve_app]("naam");
    header_plaatsen(item_naam);
    apps[actieve_app]("bouw");
    apps[actieve_app]("naam");
    if (homeknop_actief) {
      homeknop_plaatsen();
    } else if (homeknop_half_actief) {
      halve_homeknop_plaatsen();
    }

    scherm_bouwen = false;
  } else {
    if (actieve_touch) {
      if (ts_y < 30) {
        klik_header(ts_x, ts_y);
      } else if (homeknop_actief) {
        if (ts_y >= home_knop[1]) {
          actieve_app = 0;
          // actief_spel = 0;  // [BKOS5a removed]
          scherm_bouwen = true;
        }
      } else if (homeknop_half_actief) {
        if ((ts_y >= home_knop[1]) & (ts_x <= (SX_MAX / 2))) {
          actieve_app = 0;
          // actief_spel = 0;  // [BKOS5a removed]
          scherm_bouwen = true;
        }
      }
      
    }

    if (!scherm_bouwen) {
      apps[actieve_app]("run");
      if ((actieve_touch) && (!slepen_toestaan)) {

        bool lang = false;

        unsigned long startklik = millis();
        delay(50);
        ts_begin();
        while (ts_touched()) {
          delay(50);
          ts_begin();
          delay(50);
          if ((!homeknop_actief) && (!homeknop_half_actief) && (!lang)) {
            if (millis() > startklik + 1000) {
              fillCircle(ts_x, ts_y, 15, kleur_wit);
              lang = true;
            }
          }
        }

        if ((!homeknop_actief) && (!homeknop_half_actief) && (lang)) {
          if (beeld_vraag("Naar homescherm?", "", "Ja", "Nee") == 0) {
            actieve_app = 0;
            // actief_spel = 0;  // [BKOS5a removed]
            scherm_bouwen = true;
          } else {
            scherm_bouwen = true;
          }

        }
      }
    }
    delay(50);
  }
}
#line 1 "/a0/usr/workdir/BKOS5a/div_icon.ino"
void drawIcon(int32_t x, int32_t y, int breedte, uint32_t kleur, byte icon_to_draw[250], int num_instructions) {
  int k = 0;
  int r = 0;
  int32_t _x = scherm_x(x);
  int32_t _y = scherm_y(y);
  bool teken = false;
  int f = 1;
  for (int i = 1; i < 10; i++) {
    
    if ((scherm_x(1) >= i) && (scherm_y(1) >= i)){
      f = i;
    }
  }
  
  for (int i = 0; i < num_instructions; i ++) {
    if (teken) {
      if (icon_to_draw[i] > 0) {
        for (int ln = 0; ln < f; ln++)
          tft.drawLine(_x+(k*f), _y+r+ln, _x+((k+icon_to_draw[i])*f), _y+r+ln, kleur);
      }
      teken = false;
    } else {
      teken = true;
    }
    k += icon_to_draw[i];
    if (k >= breedte) {
      r += f;
      k = 0;
    }
  }

}

void drawIcon10x10(int x, int y, String icon[10], uint16_t kleur, int factor) {
  for (byte fx = 1; fx <= factor; fx++) {
    for (int i = 0; i < 10; i++) {
      for (byte fy = 1; fy <= factor; fy++) {
        for (int j = 0; j < 10; j++) {
          if (icon[i].indexOf(String(j)) > -1) {
            tft.drawPixel(scherm_x(x) + (j * factor - (fx -1)), scherm_y(y) + (i * factor - (fy -1)), kleur);
          }
        }
      }
    }
  }
}

void drawIcon10x10(int x, int y, String icon[10], uint16_t kleur) {
  drawIcon10x10(x, y, icon, kleur, 1);
  // for (int i = 0; i < 10; i++) {
  //   for (int j = 0; j < 10; j++) {
  //     if (icon[i].indexOf(String(j)) > -1) {
  //       tft.drawPixel(scherm_x(x) + j, scherm_y(y) + i, kleur);
  //     }
  //   }
  // }
}

void drawIconWifi10x10(int x, int y) {
  drawIconWifi10x10(x, y, true);
}

void drawIconWifi10x10(int x, int y, int grootte) {
  drawIconWifi10x10(x, y, true, grootte);
}

void drawIcon10x10(int x, int y, String icon[10], bool status) {
  drawIcon10x10(x, y, icon, status, 1);
}

void drawIcon10x10(int x, int y, String icon[10], bool status, int factor) {
  
  if (status) {
    drawIcon10x10(x, y, icon, kleur_wit, factor);
  } else {
    drawIcon10x10(x, y, icon, tft.color565(100, 100, 100), factor);
    drawIcon10x10(x, y, icon_streep, tft.color565(160, 0, 0), factor);
  }
}

void drawIcon10x10(int x, int y, String icon[10], uint16_t kleur, bool status) {
  drawIcon10x10(x, y, icon, kleur);
  if (!status) {
    drawIcon10x10(x, y, icon_streep, tft.color565(160, 0, 0));
  }
}

void drawIconWifi10x10(int x, int y, bool check) {
  drawIconWifi10x10(x, y, check, 1);
}

void drawIconWifi10x10(int x, int y, bool check, int grootte) {
  if (!wifi__aangesloten) {
    drawIcon10x10(x, y, icon_wifi, tft.color565(75, 75, 75), grootte);
    drawIcon10x10(x, y, icon_kruis, tft.color565(160, 0, 0), grootte);
  } else {
    if (wifi__verbonden) {
      drawIcon10x10(x, y, icon_wifi, kleur_wit, grootte);  
      if (check) {
        wifi_check();
      }
    }
    if (!wifi__verbonden) {
      drawIcon10x10(x, y, icon_wifi, tft.color565(100, 100, 100), grootte);
      drawIcon10x10(x, y, icon_streep, tft.color565(160, 0, 0), grootte);
    }
  }
}

void drawIconWifi10x10(int x, int y, uint16_t kleur) {
  drawIconWifi10x10(x, y, kleur, true, 1);
}


void drawIconWifi10x10(int x, int y, uint16_t kleur, bool check) {
  drawIconWifi10x10(x, y, kleur, check, 1);
}

void drawIconWifi10x10(int x, int y, uint16_t kleur, bool check, int grootte) {
  if (!wifi__aangesloten) {
    drawIcon10x10(x, y, icon_wifi, kleur, grootte);
    drawIcon10x10(x, y, icon_kruis, tft.color565(160, 0, 0), grootte);
  } else {
    if (wifi__verbonden) {
      drawIcon10x10(x, y, icon_wifi, kleur, grootte);  
      if ((check) & (millis() > laatste_wifi_check + 30000)) {
        wifi_check();
      }
    }
    if (!wifi__verbonden) {
      drawIcon10x10(x, y, icon_wifi, kleur, grootte);
      drawIcon10x10(x, y, icon_streep, tft.color565(160, 0, 0), grootte);
    }
  }
}

#line 1 "/a0/usr/workdir/BKOS5a/div_schermacties.ino"

// BKOS5a: Rode rand waarschuwing 10s voor scherm-timeout
void scherm_timeout_waarschuwing() {
  // Teken rode rand rondom het scherm als waarschuwing
  int dikte = 4;
  for (int i = 0; i < dikte; i++) {
    tft.drawRect(i, i, tft.width() - 2*i, tft.height() - 2*i, tft.color565(200, 0, 0));
  }
}
long uit_check = 0;

void beeld_melding(String hoofdmelding) {
  beeld_melding(hoofdmelding, "", "");
}

void beeld_melding(String hoofdmelding, String submelding) {
  beeld_melding(hoofdmelding, submelding, "");
}

void beeld_melding(String hoofdmelding, String submelding, String titel){
  beeld_melding(hoofdmelding, submelding, titel, {}, false);
}

void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10]){
  beeld_melding(hoofdmelding, submelding, titel, sec_icon, false);
}

void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10], bool sec_icon_bool) {
  if (waas != true) {
    gewaasd_scherm();
    waas = true;
  }
  fillRoundRect(15, 100, 210, 30, 10, kleur_donker);
  fillRect(15, 120, 10, 10, kleur_licht);
  fillRect(215, 120, 10, 10, kleur_licht);
  fillRoundRect(15, 120, 210, 80, 10, kleur_licht);
  drawRoundRect(15, 100, 210, 100, 10, kleur_zwart);
  drawIconWifi10x10(205, 105, kleur_licht, false);
  drawIcon10x10(190, 105, icon_sd, kleur_licht, sd_connect);
  if (sec_icon_bool) {
    drawIcon10x10(175, 105, sec_icon, kleur_licht);
  }
  center_tekst(120, 106, titel, 1, kleur_licht);
  if (submelding == "") {
    center_tekst(120, 150, hoofdmelding, 2, kleur_donker);
  } else {
    center_tekst(120, 140, hoofdmelding, 2, kleur_donker);
    center_tekst(120, 170, submelding, 1, kleur_zwart);
  }
  tft.setTextSize(1.5);
  tft.setTextColor(kleur_zwart);
  scherm_bouwen = true;
}

void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur) {
  tft.fillRoundRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), scherm_y(d), kleur);
}
void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal) {
  tft.fillRoundRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), scherm_y(d, schaal), kleur);
}
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur) {
  tft.drawRoundRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), scherm_y(d), kleur);
}
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal) {
  tft.drawRoundRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), scherm_y(d, schaal), kleur);
}
void fillRect(int x, int y, int w, int h, uint16_t kleur) {
  tft.fillRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), kleur);
}
void fillRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal) {
  tft.fillRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), kleur);
}
void drawRect(int x, int y, int w, int h, uint16_t kleur) {
  tft.drawRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), kleur);
}
void drawRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal) {
  tft.drawRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), kleur);
}
void setCursor(int x, int y) {
  tft.setCursor(scherm_x(x), scherm_y(y));
}
void setCursor(int x, int y, uint16_t schaal) {
  tft.setCursor(scherm_x(x, schaal), scherm_y(y, schaal));
}
void fillCircle(int x, int y, int r, uint16_t kleur) {
  tft.fillCircle(scherm_x(x), scherm_y(y), scherm_y(r), kleur);
}
void fillCircle(int x, int y, int r, uint16_t kleur, uint16_t schaal) {
  tft.fillCircle(scherm_x(x, schaal), scherm_y(y, schaal), scherm_y(r, schaal), kleur);
}
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur) {
  tft.drawLine(scherm_x(x1), scherm_y(y1), scherm_x(x2), scherm_y(y2), kleur);
}
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur, uint16_t schaal) {
  tft.drawLine(scherm_x(x1, schaal), scherm_y(y1, schaal), scherm_x(x2, schaal), scherm_y(y2, schaal), kleur);
}

// boolean string_in_array(String element, String array[], int lengte)
// byte beeld_vraag(String hoofdmelding, String submelding, String array[], byte opties_cnt)
byte beeld_vraag(String hoofdmelding, String submelding, String optie1) {
  return beeld_vraag(hoofdmelding, submelding, 1, optie1, "", "", "", "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2) {
  return beeld_vraag(hoofdmelding, submelding, 2, optie1, optie2, "", "", "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3) {
  return beeld_vraag(hoofdmelding, submelding, 3, optie1, optie2, optie3, "", "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4) {
  return beeld_vraag(hoofdmelding, submelding, 4, optie1, optie2, optie3, optie4, "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4, String optie5) {
  return beeld_vraag(hoofdmelding, submelding, 5, optie1, optie2, optie3, optie4, optie5);
}
byte beeld_vraag(String hoofdmelding, String submelding, byte opties_cnt, String optie1, String optie2, String optie3, String optie4, String optie5){
  String array[5] = {optie1, optie2, optie3, optie4, optie5};
  return beeld_vraag(hoofdmelding, submelding, array, opties_cnt);
}

byte beeld_vraag(String hoofdmelding, String submelding, String array[], byte opties_cnt) {
  
  if (waas != true) {
    gewaasd_scherm();
    waas = true;
  }
  fillRoundRect(15, 80, 210, 30, 10, kleur_donker);
  fillRect(15, 100, 10, 10, kleur_licht);
  fillRect(215, 100, 10, 10, kleur_licht);
  fillRoundRect(15, 100, 210, 120, 10, kleur_licht);
  drawRoundRect(15, 80, 210, 140, 10, kleur_zwart);
  drawIconWifi10x10(205, 85, kleur_licht, false);
  if (submelding == "") {
    center_tekst(120, 125, hoofdmelding, 2, tft.color565(0, 100, 0));
  } else {
    center_tekst(120, 115, hoofdmelding, 2, tft.color565(0, 100, 0));
    center_tekst(120, 145, submelding, 1, tft.color565(0, 0, 0));
  }

  int B = 210 / opties_cnt;

  for (byte i = 0; i < opties_cnt; i++){
    fillRoundRect(i*B + 20, 170, B-10, 35, 5, kleur_donker);
    center_tekst(i*B + B/2 + 20, 180, array[i], 1.5, kleur_wit);
  }

  bool verder = false;

  while (!verder) {
    io();
    delay(50);
    ts_begin();
    if (ts_touched()) {
      int x = touch_x();
      int y = touch_y();

      while (ts_touched()) {
        ts_begin();
        delay(50);
      }
      
      for (byte i = 0; i < opties_cnt; i++) {
        if ((x > i*B + 15) & (x < (i+1)*B + 15)  &  (y > 165) & (y < 225)) {
          verder = true;
          while (ts_touched()){
            ts_begin();
            delay(50);
          }
          return i;
        }
      }

      while (ts_touched()) {
        ts_begin();
      }

    }
  }

  while (ts_touched()) {
    ts_begin();
    delay(50);
  }

  ts_begin();


  return -1;
}

void gewaasd_scherm() {
  byte factor = 2;
  for ( int i = 0; i < SY_MAX / factor; i++) {
    tft.drawLine(0, i*factor, SX_MAX, i*factor, tft.color565(0,0,0));
  }
}

void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte=1, uint16_t tekst_kleur=tft.color565(255,255,255)) {
  center_tekst(x, y, tekst, tekst_grootte, tekst_kleur, true);
}

// void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte=1, uint16_t tekst_kleur=tft.color565(255,255,255), bool schalen=true) {
  void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte, uint16_t tekst_kleur, bool schalen) {

  x = scherm_x(x);
  y = scherm_y(y);
  
  int _tekst_grootte = 1;
  if (schalen) {
    for (int i = 1; i <= 20; i++){
      if ((scherm_x(tekst_grootte) >= i) && (scherm_y(tekst_grootte) >= i)) {
        _tekst_grootte = i;
      }
    }
  } else {
    _tekst_grootte = tekst_grootte;
  }


  tft.setTextSize(_tekst_grootte);
  tft.setTextColor(tekst_kleur);
  tft.setCursor((x - tekst.length()*_tekst_grootte*3), y);
  tft.print(tekst);

}

int16_t scherm_x(int16_t invoer) {
  return scherm_x(invoer, SCRIPT_RESOLUTIE);
}

int16_t scherm_x(int16_t invoer, int16_t code_resolutie) {
  return scherm_x(invoer, code_resolutie, RESOLUTIE);
  int16_t max_in;
  max_in = 240;
  return scherm_x(invoer, max_in);
}

int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie) {
  return scherm_x(invoer, code_resolutie, resolutie_x(scherm_resolutie, false), resolutie_x(scherm_resolutie, true));
}

int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max) {
  return map(invoer, resolutie_x(code_resolutie, false), resolutie_x(code_resolutie, true), res_min, res_max);
}

int16_t resolutie_x(int16_t resolutie, bool eind) {
  if (eind) {
    if (resolutie == 2432) {
      return 240;
    } else if (resolutie == 3248) {
      return 320;
    } else if (resolutie == 4880) {
      if (ORIENTATIE == 1) {
        return 480;
      } else if (ORIENTATIE == 0) {
        return 800;
      }
    } else if (resolutie == 100) {
      return 100;
    }
  } else {
    return 0;
  }
  return -1;
}

int getCursorX() {
  return getCursorX(SCRIPT_RESOLUTIE);
}

int getCursorX(int16_t code_resolutie) {
  return getCursorX(code_resolutie, RESOLUTIE);
}

int getCursorX(int16_t code_resolutie, int16_t scherm_resolutie) {
  return map(tft.getCursorX(), resolutie_x(scherm_resolutie, false), resolutie_x(scherm_resolutie, true), resolutie_x(code_resolutie, false), resolutie_x(code_resolutie, true));
}


int16_t scherm_y(int16_t invoer) {
  return scherm_y(invoer, SCRIPT_RESOLUTIE);
}

int16_t scherm_y(int16_t invoer, int16_t code_resolutie) {
  return scherm_y(invoer, code_resolutie, RESOLUTIE);
  int16_t max_in;
  max_in = 240;
  return scherm_y(invoer, max_in);
}

int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie) {
  return scherm_y(invoer, code_resolutie, resolutie_y(scherm_resolutie, false), resolutie_y(scherm_resolutie, true));
}

int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max) {
  return map(invoer, resolutie_y(code_resolutie, false), resolutie_y(code_resolutie, true), res_min, res_max);
}

int16_t resolutie_y(int16_t resolutie, bool eind) {
  if (eind) {
    if (resolutie == 2432) {
      return 320;
    } else if (resolutie == 3248) {
      return 480;
    } else if (resolutie == 4880) {
      if (ORIENTATIE == 1) {
        return 800;
      } else if (ORIENTATIE == 0) {
        return 480;
      }
    } else if (resolutie == 100) {
      return 100;
    }
  } else {
    return 0;
  }
  return -1;
}

int getCursorY() {
  return getCursorY(SCRIPT_RESOLUTIE);
}

int getCursorY(int16_t code_resolutie) {
  return getCursorY(code_resolutie, RESOLUTIE);
}

int getCursorY(int16_t code_resolutie, int16_t scherm_resolutie) {
  return map(tft.getCursorY(), resolutie_y(scherm_resolutie, false), resolutie_y(scherm_resolutie, true), resolutie_y(code_resolutie, false), resolutie_y(code_resolutie, true));
}



void run_scherm_pauze() {
  if (ts_touched()) {
    if (scherm_helderheid == 100) {
      digitalWrite(TFT_BL, HIGH);
    } else if (scherm_helderheid < 100) {
      analogWrite(TFT_BL, round(255*(scherm_helderheid/100)));
    }
    scherm_actief = true;

    while (ts_touched()) {
      ts_begin();
    }

    scherm_touched = millis();
    ts_begin();
  }
  else if (uit_check + 60000 > millis()) {
    digitalWrite(TFT_BL, LOW);
    uit_check = millis();
  }
}

#line 1 "/a0/usr/workdir/BKOS5a/div_status.ino"
void wifi_check(){
  
}
#line 1 "/a0/usr/workdir/BKOS5a/div_teken.ino"
void tekenItem(int32_t x, int32_t y, uint32_t kleur, int item_to_draw[250][2], int num_instructions) {
  bool teken = false;
  int laatste_punt[2] = {-1, -1};

  // int16_t _x;
  // int16_t _y;

  for (int i = 0; i < num_instructions; i ++) {
    if ((item_to_draw[i][0] == laatste_punt[0]) && (item_to_draw[i][1] == laatste_punt[1])) {
      if (teken) {
        teken = false;
      } else {
        teken = true;
      }
    } else if (teken) {
      tft.drawLine(scherm_x(x+laatste_punt[0]), scherm_y(y+laatste_punt[1]), scherm_x(x+item_to_draw[i][0]), scherm_y(y+item_to_draw[i][1]), kleur);
    }
    laatste_punt[0] = item_to_draw[i][0];
    laatste_punt[1] = item_to_draw[i][1];
  }
}

void tekenCircels(int32_t x, int32_t y, uint32_t kleur, int circle_to_draw[250][3], int num_instructions) {
  for (int i = 0; i < num_instructions; i ++) {
    tft.drawCircle(scherm_x(x+circle_to_draw[i][0]), scherm_y(y+circle_to_draw[i][1]), circle_to_draw[i][2], kleur);
  }
}

void tekenTeksten(int32_t x, int32_t y, uint32_t kleur, int tekst_positie[20][3], String teksten[20], int num_instructions) {
  for (int i = 0; i < num_instructions; i ++) {
    center_tekst(x + tekst_positie[i][0], y + tekst_positie[i][1], teksten[i], 1.5, kleur);
  }
}
#line 1 "/a0/usr/workdir/BKOS5a/div_tijd.ino"
#ifdef ESP32

  void internet_tijd(){
    timeClient.begin();
    timeClient.update();
  }

  String tijd() {
    return tijd("HM");
  }

  String tijd(String wat){
    timeClient.update();
    if (wat == "HM"){
      String uit = timeClient.getFormattedTime();
      uit = uit.substring(0, 5);
      return uit;
    }
    return "..:..";
  }

#else

  void internet_tijd() {
    return;
  }

  String tijd() {
    return tijd("HM");
  }

  String tijd(String wat) {
    /* Deze functie update de laatste wifi ontvangen tijd met de verstreken mili secondes en corrigeerd deze naar de tijdzonde.
    Vervolgens wordt een String van de tijd terug gegeven om elders te kunnen gebruiken */

    long mili_buffer = millis();
    seconde = seconde + ((mili_buffer - tijd_millis) / 1000);
    if ((tijdzone == "GMT") || (tijdzone.substring(0, tijdzone.length()-1) == "GMT")) {
      uur++;
      tijdzone = "GMT +1";
    }
    tijd_millis = mili_buffer;
    int buffer = seconde / 60;
    minuut = minuut + buffer;
    seconde = seconde - buffer * 60;
    buffer = minuut / 60;
    uur = uur + buffer;
    minuut = minuut - buffer * 60;
    buffer = uur / 60;
    uur = uur - buffer * 60;

    while (uur >= 24) {
      dag++; // Dag is plus 1. Omdat dit aan het einde van de maand niet klopt gaat het niet perfect, maar dit wordt gereset zodra er een app wordt verstuurd.
      uur -= 24;
      dagnm = ""; // Nog een functie maken om de dagnaam te veranderen. Voor nu omdat deze niet meer klopt weg halen
    }

    String response;
    if (wat.indexOf("H") > -1) {
      response += String(uur) + ":";
      if (wat.indexOf("M") > -1) {
        if (String(minuut).length() == 1) {
          response += "0";
        }
        response += String(minuut) + ":";
        if (wat.indexOf("S") > -1) {
          if (String(seconde).length() == 1) {
            response += "0";
          }
          response += String(seconde) + ":";
        }
      }
      response = response.substring(0, response.length()-1);
      if (wat.indexOf("Z") > -1) {
        // response += " " + tijdzone;
      } else if (wat.indexOf("uur") > -1) {
        response += " uur";
      }
    }
    
    return response;
  }

  long millis_wekker(int H, int M) {
    /* Deze functie op welke millis() het een gewenste tijd is om een timer te kunnen zetten... erg handig (denk ik)*/

    // Tijd aanroepen zodat ook de tijdzone wordt gecorrigeerd
    tijd();

    // Tijd bepalen tot de triger sinds de laatste meting
    int sec = 60 - seconde;
    int min = 60 - minuut + M - 1; // Oorspronkelijk nog -1, maar omdat het bericht nu 1 minuut te vroeg verstuurd werd deze verwijderd.
    int hr = 24 - uur + H - 1;
    
    // Correctie voor een volle minuut, uur of dag
    while (sec >= 60) {
      sec -= 60;
      min++;
    }
    while (min >= 60) {
      min -= 60;
      hr++;
    }
    while (hr >= 24) {
      hr -= 24;
    }

    // Omzetten in te verstrijken milli seconden
    return tijd_millis + (hr * 3600000) + (min * 60000) + (sec * 1000);
  }

  long millis_timer(int H, int M, int S) {
    /* Deze heel eenvoudige functie zet een millis timer voor over een bepaalde tijd */
    return millis() + (H * 3600000) + (M * 60000) + (S * 1000);
  }

  String tijd_tot_millis(long invoer) {
    long tijd = invoer - millis();
    int sec = tijd / 1000;
    if (sec < 60) {
      return String(sec) + " seconde";
    }
    
    int min = sec / 60;
    sec = sec - min * 60;
    if (sec < 0) {
      sec += 60;
      min--;
    }
    if (min < 60) {
      return String(min) + " minuten, " + String(sec) + " seconde";
    }

    int uur = min / 60;
    min = min - uur * 60;
    if (uur < 24) {
      return String(uur) + " uur, " + String(min) + " minuten";
    }

    int dag = uur / 24;
    uur = uur - dag * 24;
    return String(dag) + " dagen, " + String(uur) + " uur";
  }

#endif
#line 1 "/a0/usr/workdir/BKOS5a/hw_scherm.ino"
void tft_begin(){
  tft.begin();
}

void tft_setup(){
  // SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  pinMode(TFT_BL, OUTPUT);
  tft_begin();
  tft.setRotation(ROTATIE);
  
  digitalWrite(TFT_BL, HIGH);
  delay(50);
}


#line 1 "/a0/usr/workdir/BKOS5a/hw_sd.ino"
#if HARDWARE == 2
  #if RESOLUTIE == 2432
    bool sd_begin(){
      // return SD.begin();
      // return SD.begin(SD_CS, spi_sd, 80000000);
    }
    void sd_setup(){
      // pinMode(SD_CS, OUTPUT);
      // spi_sd.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
      // sd_begin();
    }
  #else
    bool sd_begin(){
      // return SD.begin(SD_CS);
    }
    void sd_setup(){
      // sd_begin();
    }
  #endif
#else
  bool sd_begin(){
    return SD.begin(SD_CS);
  }
  void sd_setup(){
    // sd_begin();
  }
#endif 

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    // Serial.printf("Listing directory: %s\n", dirname);

    // File root = fs.open(dirname);
    // if(!root){
    //     Serial.println("Failed to open directory");
    //     return;
    // }
    // if(!root.isDirectory()){
    //     Serial.println("Not a directory");
    //     return;
    // }

    // File file = root.openNextFile();
    // while(file){
    //     if(file.isDirectory()){
    //         Serial.print("  DIR : ");
    //         Serial.println(file.name());
    //         if(levels){
    //             listDir(fs, file.path(), levels -1);
    //         }
    //     } else {
    //         Serial.print("  FILE: ");
    //         Serial.print(file.name());
    //         Serial.print("  SIZE: ");
    //         Serial.println(file.size());
    //     }
    //     file = root.openNextFile();
    // }
}

void readFile(fs::FS &fs, const char * path){
    // Serial.printf("Reading file: %s\n", path);

    // File file = fs.open(path);
    // if(!file){
    //     Serial.println("Failed to open file for reading");
    //     return;
    // }

    // Serial.print("Read from file: ");
    // while(file.available()){
    //     Serial.write(file.read());
    // }
    // file.close();
}

void sd_lees() {
  // sd_setup();
  // Serial.println("TESTEN SD functionaliteit");
  // // if (!sd_begin()){
  // //   Serial.println("Geen SD kaart gevonden");
  // //   return;
  // // }
  // // uint8_t cardType = SD.cardType();
  // // Serial.println(cardType);
  // // if(cardType == CARD_MMC){
  // //     Serial.println("MMC");
  // // } else if(cardType == CARD_SD){
  // //     Serial.println("SDSC");
  // // } else if(cardType == CARD_SDHC){
  // //     Serial.println("SDHC");
  // // } else {
  // //     Serial.println("UNKNOWN");
  // // }

  // // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  // // Serial.printf("SD Card Size: %lluMB\n", cardSize);

  // // listDir(SD, "/", 0);
  // // listDir(SD, "/BKOS", 0);
  // readFile(SD, "/BKOS/io.cfg");

  // SD.end();
  // digitalWrite(SD_CS, HIGH);

  // ts_begin();
}
#line 1 "/a0/usr/workdir/BKOS5a/hw_touch.ino"
#if HARDWARE == 2
    void ts_begin(){
      digitalWrite(TS_CS, HIGH);
      ts.begin(spi_ts);
    }
    
    void ts_setup(){
      spi_ts.begin(TS_SCK, TS_MISO, TS_MOSI, TS_CS);
      ts_begin();
      ts.setRotation(0);
    }
#elif HARDWARE == 3
  void ts_begin(){
    ts.begin(spi_ts);
  }
  void ts_setup(){
    spi_ts.begin(TS_SCK, TS_MISO, TS_MOSI, TS_CS);
    ts_begin();
    ts.setRotation(0);
  }
#elif HARDWARE == 4
  void ts_begin(){
    ts.read();
  }
  void ts_setup(){
    ts.begin();
    ts.setRotation(0);
  }
#elif HARDWARE == 5
  void ts_begin(){
    ts.read();
  }
  void ts_setup(){
    // Hier onstaat helaas een fout in de Serial Monitor... dat is helaas wat het is niks aan te doen (anders werkt het touchscreen niet)
    ts.begin();
    ts.setRotation(0);
  }
#else
  void ts_begin(){
    ts.begin();
  }
  void ts_setup(){
    ts_begin();
    ts.setRotation(ROTATIE);
  }
#endif

#if TS_DRIVER == 3
  bool ts_touched() {
    // ts.read();
    if (ts.isTouched) {
      return true;
    }
    return false;
  }

  #if ORIENTATIE == 1
    #if HARDWARE == 4
      int touch_x() {
        ts.read();
        return map(ts.points[0].x, 5, 275, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].y, 5, 480, 0, 320);
      }
    #elif HARDWARE == 5
      int touch_x() {
        ts.read();
        return map(ts.points[0].x, 5, 500, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].y, 5, 800, 0, 320);
      }
    #endif
  #elif ORIENTATIE == 0
    #if HARDWARE == 4
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 5, 480, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 275, 5, 0, 320);
      }
    #elif HARDWARE == 5
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 5, 800, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 500, 5, 0, 320);
      }
    #endif
  #elif ORIENTATIE == 2
    // TEST
    #if HARDWARE == 4
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 5, 480, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 275, 5, 0, 320);
      }
    #elif HARDWARE == 5
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 800, 5, 240, 0);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 5, 500, 320, 0);
      }
    #endif
  #endif
#else
  bool ts_touched(){
    if (ts.tirqTouched()) {
      if (ts.touched()) {
        return true;
      }
    }
    return false;
  }

  int touch_x() {
    TS_Point p = ts.getPoint();
    float x = (p.x - xcorrectie) / xfactor;
    // actie = 1;
    return x;
  }

  int touch_y() {
    TS_Point p = ts.getPoint();
    float y = (p.y - ycorrectie) / yfactor;
    // actie = 1;
    return y;
  }
#endif
#line 1 "/a0/usr/workdir/BKOS5a/instellingen.ino"
void instellingen(String actie) {
  if (actief_instelling == 0) {
    if (actie == "print") {
      instellingen("naam");
      tft.print(item_naam);
    } else if (actie == "icon_klein") {
      int x = getCursorX() + 20;
      int y = getCursorY() + 25;
      setCursor(getCursorX(), getCursorY()+4);
      drawIcon10x10(x, y, icon_instellingen, kleur_donker, 3);
      tft.setTextColor(kleur_rood);
    } else {
      instellingen_bibliotheek[actief_instelling](actie);
    } 
  } else {
    if (actie == "bouw") {
      // [BKOS5a] spellen verwijderd - directe toewijzing
      homeknop_actief = true;
      homeknop_half_actief = false;
      slepen_toestaan = false;
    }
    instellingen_bibliotheek[actief_instelling](actie);
  }
}




void instellingen_menu(String actie){
  if (actie == "bouw") {
    bouw_instellingen();
  // } else if (actie == "run") {
  //   run_instellingen();
  } else if (actie == "naam") {
    item_naam = "Instellingen";
  } else if (actie == "run") {
    if (actieve_touch) {

      int druk = klik_instelling(ts_x, ts_y);

      if ((druk > -1) & (druk < app_cnt)) {
        actief_instelling = druk + 1;
        scherm_bouwen = true;
      }
    }
  }
}


void bouw_instellingen() {

  bouw_instellingen(false);

}

void bouw_instellingen(bool overlay) {

  delete[]knoppen_teken_positie;
  delete[]knoppen_positie;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst;
  delete[]knoppen_tekst_kleur;
  
  header_plaatsen("Instellingen");
  if (overlay) {
    overligger(kleur_zwart, instellingen_overlay_hoogte, instellingen_overlay_dicht_tot);
  } else {
    achtergrond(kleur_zwart, true);
  }


  alle_instelling_knoppen_plaatsen();
}

void insteling_knop_plaatsen(int model, int knop_nummer){
  if (model == 8) {
    fillRoundRect(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1], keuzescherm_knoppen_8[knop_nummer][2], keuzescherm_knoppen_8[knop_nummer][3], 5, kleur_licht);
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_donker);
    setCursor(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1]);
    if (knop_nummer <= 1) {
      instellingen_bibliotheek[knop_nummer+1]("icon_groot");
    } else {
      instellingen_bibliotheek[knop_nummer+1]("icon_klein");
    }
  } else if (model == 6) {
    fillRoundRect(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1], keuzescherm_knoppen_6[knop_nummer][2], keuzescherm_knoppen_6[knop_nummer][3], 5, kleur_licht);
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_donker);
    setCursor(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1]);
    if (knop_nummer <= 1) {
      instellingen_bibliotheek[knop_nummer+1]("icon_groot");
    } else {
      instellingen_bibliotheek[knop_nummer+1]("icon_middel");
    }
  } else if (model == 4) {
    fillRoundRect(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1], keuzescherm_knoppen_4[knop_nummer][2], keuzescherm_knoppen_4[knop_nummer][3], 5, kleur_licht);
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_donker);
    setCursor(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1]);
    instellingen_bibliotheek[knop_nummer+1]("icon_groot");
  }
  instellingen_bibliotheek[knop_nummer+1]("print");
}


void alle_instelling_knoppen_plaatsen(){
  int model;
  if (instellingen_cnt <= 5) {
    model = 4;
  } else if (instellingen_cnt <= 7) {
    model = 6;
  } else {
    model = 8;
  }

  for (byte i = 0; i < instellingen_cnt-1; i++) {
    insteling_knop_plaatsen(model, i);
  }
}

int klik_instelling(int x, int y) {
  if (instellingen_cnt < 6) {
    for (byte i = 0; i < instellingen_cnt -1; i++) {
      if ((x >= keuzescherm_knoppen_4[i][0]) && (x <= keuzescherm_knoppen_4[i][0]+keuzescherm_knoppen_4[i][2]) && (y >= keuzescherm_knoppen_4[i][1]) && (y <= keuzescherm_knoppen_4[i][1]+keuzescherm_knoppen_4[i][3])) {
        return i;
      }
    }
  } else if (instellingen_cnt < 8) {
    for (byte i = 0; i < instellingen_cnt -1; i++) {
      if ((x >= keuzescherm_knoppen_6[i][0]) && (x <= keuzescherm_knoppen_6[i][0]+keuzescherm_knoppen_6[i][2]) && (y >= keuzescherm_knoppen_6[i][1]) && (y <= keuzescherm_knoppen_6[i][1]+keuzescherm_knoppen_6[i][3])) {
        return i;
      }
    }
  } else {
    for (byte i = 0; i < instellingen_cnt -1; i++) {
      if ((x >= keuzescherm_knoppen_8[i][0]) && (x <= keuzescherm_knoppen_8[i][0]+keuzescherm_knoppen_8[i][2]) && (y >= keuzescherm_knoppen_8[i][1]) && (y <= keuzescherm_knoppen_8[i][1]+keuzescherm_knoppen_8[i][3])) {
        return i;
      }
    }
  }
  return -1;
}





// void spel_knop_plaatsen(int model, int knop_nummer){
//   if (model == 8) {
//     fillRoundRect(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1], keuzescherm_knoppen_8[knop_nummer][2], keuzescherm_knoppen_8[knop_nummer][3], 5, kleur_licht);
//     tft.setTextSize(1.5);
//     tft.setTextColor(kleur_donker);
//     setCursor(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1]);
//     if (knop_nummer <= 1) {
//       spellen_bibliotheek[knop_nummer+1]("icon_groot");
//     } else {
//       spellen_bibliotheek[knop_nummer+1]("icon_klein");
//     }
//   } else if (model == 6) {
//     fillRoundRect(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1], keuzescherm_knoppen_6[knop_nummer][2], keuzescherm_knoppen_6[knop_nummer][3], 5, kleur_licht);
//     tft.setTextSize(1.5);
//     tft.setTextColor(kleur_donker);
//     setCursor(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1]);
//     if (knop_nummer <= 1) {
//       spellen_bibliotheek[knop_nummer+1]("icon_groot");
//     } else {
//       spellen_bibliotheek[knop_nummer+1]("icon_middel");
//     }
//   } else if (model == 4) {
//     fillRoundRect(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1], keuzescherm_knoppen_4[knop_nummer][2], keuzescherm_knoppen_4[knop_nummer][3], 5, kleur_licht);
//     tft.setTextSize(1.5);
//     tft.setTextColor(kleur_donker);
//     setCursor(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1]);
//     spellen_bibliotheek[knop_nummer+1]("icon_groot");
//   }
//   spellen_bibliotheek[knop_nummer+1]("print");
// }


// void alle_spel_knoppen_plaatsen(){
//   int model;
//   if (spellen_cnt <= 5) {
//     model = 4;
//   } else if (spellen_cnt <= 7) {
//     model = 6;
//   } else {
//     model = 8;
//   }

//   for (byte i = 0; i < spellen_cnt-1; i++) {
//     spel_knop_plaatsen(model, i);
//   }
// }









// void instellingen(String actie) {
//   if (actie == "bouw") {
//     bouw_instellingen();
//   } else if (actie == "run") {
//     run_instellingen();
//   } else if (actie == "naam") {
//     item_naam = "Instellingen";
//   } else if (actie == "print") {
//     instellingen("naam");
//     tft.print(item_naam);
//   } else if (actie == "icon_klein") {
//     int x = getCursorX() + 20;
//     int y = getCursorY() + 25;
//     setCursor(getCursorX(), getCursorY()+4);
//     drawIcon10x10(x, y, icon_instellingen, kleur_donker, 3);
//     tft.setTextColor(kleur_rood);
//   }
// }




// void instellingen_restart() {
//   beeld_vraag("Helaas","De reset functie werkt nog niet", "ok");
//   scherm_bouwen=true;
// }

// void instellingen_sd_reset() {
  
//   byte druk;

//   if (false) { //(info_snaam.length() > 2) {
//     druk = beeld_vraag("SD kaart kopie", "Wilt u een kopie maken van de SD?", "JA", "NEE");
//     if (druk == 0){
//       beeld_melding("SD kopie maken", "Even geduld a.u.b.  . . .");

//       delay(1000);
//       // sd_maken();
//       delay(1000);
//     }
//   } else {
//     druk = beeld_vraag("schone SD", "Wilt u SD kaart inrichten?", "JA", "NEE");
//     if (druk == 0){
//       beeld_melding("schone SD", "Even geduld a.u.b.  . . .");

//       delay(1000);
//       // blanco_sd_maken();
//       delay(1000);
//     }
//   }

//   scherm_bouwen = true;

// }

// void instellingen_leven() {
//   // beeld_vraag("Volgende update:", tijd_tot_millis(timer_statusupdate), "ok");
//   scherm_bouwen = true;
// }

// void run_instellingen() {
//   int druk;
//   if (ts_touched()) {
//     int x = touch_x();
//     int y = touch_y();

//     druk = klik(x, y);

//     while (ts_touched()) {
//       ts.begin();
//     }

//     ts.begin();

//     if (druk > -1) {
//       instellingen_mean_knoppen_functies[druk]();
//     }

//     if (y > home_knop[1]) {
//       if (actieve_app == 9) {
//         actieve_app = 0;
//       }
//       scherm_bouwen = true;
//     }
//   }
//   ts.begin(); 
// }

// void loop_instellingen() {
  
//   while (! scherm_bouwen) {
//     if (scherm_actief) {
//       if ((millis() > h_klok_millis + 20000) || (millis() < h_klok_millis)) {
//         klok_update();
//       }
//     }

//     io();
//     ts.begin();
//     run_instellingen();
//   }
// }
#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_cnt.ino"
void bouw_instellingen_io_cnt() {
  // bouw_nummeriek("Aantal IO?");
}

void instellingen_io_cnt_aanpassen_naar(int cnt_nieuw) {
  beeld_melding("Even geduld a.u.b.", "aantal io poorten wordt aangepast");
  ts.begin();
  delay(500);
  SD.begin(SD_CS);
  delay(50);
  SD.begin(SD_CS);
  delay(50);
  if (SD.begin(SD_CS)) {
    // sd_save_io(cnt_nieuw);
    // sd_io();
    delay(50);
    // start_io();
  } else {
    beeld_melding("ERROR", "De SD kaart kan niet worden bereikt.");
  }
}

void run_instellingen_io_cnt() {
  if (nummeriek_actief == true) {
    run_nummeriek();
  } else {
    String vraag = "IO cnt op " + String(nummeriek_invoer);
    if (beeld_vraag(vraag, "er wordt gereset", "nee", "ja") == 1) {
      instellingen_io_cnt_aanpassen_naar(nummeriek_invoer);
    } else {
      beeld_melding("nee?");
    }
    naar_io_instellingen();
    
  }
  
}

#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_event.ino"

#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.ino"
void naar_instellingen_io_poort(int poort) {
  instellingen_io_poort_nummer = poort;
  io_sd[poort].substring(4).toCharArray(instellingen_io_poort_naam, 10);
//  io_namen[poort].toCharArray(instellingen_io_poort_naam, 10);
  instellingen_io_poort_functie = io_sd[poort].substring(0,1).toInt();
  // instellingen_io_poort_default = io_sd[poort].substring(2,3).toInt();
  instellingen_io_poort_alert = io_sd[poort].substring(3,4).toInt();
  if (io_sd[poort].substring(1,2) == "1") {
    instellingen_io_poort_opslaan = true;
  } else {
    instellingen_io_poort_opslaan = false;
  }
  if (io_sd[poort].substring(2,3).toInt() == 0) {
    instellingen_io_poort_default = false;
    instellingen_io_poort_inverse = false;
  } else if (io_sd[poort].substring(2,3).toInt() == 1) {
    instellingen_io_poort_default = true;
    instellingen_io_poort_inverse = false;
  } else if (io_sd[poort].substring(2,3).toInt() == 2) {
    instellingen_io_poort_default = false;
    instellingen_io_poort_inverse = true;
  } else if (io_sd[poort].substring(2,3).toInt() == 3) {
    instellingen_io_poort_default = true;
    instellingen_io_poort_inverse = true;
  }
  byte instellingen_io_poort_functie = io_sd[poort].substring(0,1).toInt();
}

void io_poorten_tijdelijk_opslaan() {
  String poort_regel = "";
  poort_regel += String(instellingen_io_poort_functie);
  poort_regel += String(instellingen_io_poort_opslaan);
  if (instellingen_io_poort_inverse) {
    if (instellingen_io_poort_default) {
      poort_regel += "3";
    } else {
      poort_regel += "2";
    }
  } else {
    if (instellingen_io_poort_default) {
      poort_regel += "1";
    } else {
      poort_regel += "0";
    }
  }
  poort_regel += String(instellingen_io_poort_alert);
  poort_regel += String(instellingen_io_poort_naam);

  poort_regel += io_poorten_io_events_instellingen_toevoegen();
  poort_regel += io_poorten_io_ruimte_instellingen_toevoegen();

  io_sd[instellingen_io_poort_nummer] = poort_regel;
}

String io_poorten_io_events_instellingen_toevoegen() {
  bool maken = false;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      if ((io_events[instellingen_io_poort_nummer][i][j][0] < 255) && (io_events[instellingen_io_poort_nummer][i][j][0] >= 0)) {
        maken = true;
      }
    }
  }
  String event_string = "";
  if (maken) {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        event_string += ";";
        for (int k = 0; k < 10; k++){
          if ((io_events[instellingen_io_poort_nummer][i][j][k] < 255) && (io_events[instellingen_io_poort_nummer][i][j][k] >= 0)) {
            if (k > 0) {
              event_string += ",";
            }
            event_string += String(io_events[instellingen_io_poort_nummer][i][j][k]);
          }
        }
      }
    }
  }
  return event_string;
}

String io_poorten_io_ruimte_instellingen_toevoegen() {
  if (io_object_ruimte[instellingen_io_poort_nummer] != 255) {
    return "|" + io_ruimtes_namen[io_object_ruimte[instellingen_io_poort_nummer]];
  }
  return "";
}

void bouw_instellingen_io_poort() {
  header_plaatsen();
  achtergrond(kleur_zwart, false);

  aantal_knoppen = instellingen_io_poort_knoppen_cnt;
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = instellingen_io_poort_knoppen_status;


  for (int i = 0; i < aantal_knoppen; i++) {
    knoppen_teken_positie[i] = instellingen_io_poort_knoppen[i];
    knoppen_positie[i] = instellingen_io_poort_knoppen[i];
    knoppen_basiskleur[i] = instelligen_io_poort_knoppen_kleur;
    knoppen_tekst[i] = instellingen_io_poort_knoppen_tekst[i];
    knoppen_tekst_kleur[i] = instelligen_io_poort_knoppen_tekstkleur;
  }
  
  bouw_instellingen_io_poort_knoppen_plaatsen();

}

void bouw_instellingen_io_poort_knoppen_plaatsen() {
  for (int i = 0; i < 4; i++) {
    if (instellingen_io_poort_functie == i+1) {
      knoppen_status[i] = 1;
    } else {
      knoppen_status[i] = 0;
    }
  }
  knoppen_status[4] = instellingen_io_poort_default;
  knoppen_status[5] = instellingen_io_poort_inverse;
  knoppen_status[6] = instellingen_io_poort_opslaan;
  if (instellingen_io_poort_alert == 0) {
    knoppen_status[7] = 1;
    knoppen_status[8] = 0;
  } else if (instellingen_io_poort_alert == 1) {
    knoppen_status[7] = 0;
    knoppen_status[8] = 1;
  } else if (instellingen_io_poort_alert == 2) {
    knoppen_status[7] = 1;
    knoppen_status[8] = 1;
  } else if (instellingen_io_poort_alert == 3) {
    knoppen_status[7] = 0;
    knoppen_status[8] = 0;
  }
  if (instellingen_io_poort_nummer == 0) {
    knoppen_status[10] = 5;
  } else {
    knoppen_status[10] = 4;
  }
  if (instellingen_io_poort_nummer+1 == io_sd_cnt) {
    knoppen_status[11] = 5;
  } else {
    knoppen_status[11] = 4;
  }

  fillRect(5, 35, 230, 40, kleur_wit);
  center_tekst(120, 45, instellingen_io_poort_naam, 2, kleur_donker);

  tft.setTextSize(1);

  tft.setTextColor(kleur_wit);
  tft.setCursor(10, 80);
  tft.print("io functie:");
  tft.setCursor(10, 140);
  tft.print("default status:");
  tft.setCursor(10, 200);
  tft.print("alert moment:");

  alle_knoppen_plaatsen();

}

void run_instellingen_io_poort() {
  // if (tekstinvoer_actief) {
  //   run_tekstinvoer();
  // } else if (tekst_invoer != "") {
  //   tekst_invoer.toCharArray(instellingen_io_poort_naam, 10);
  //   io_namen[instellingen_io_poort_nummer] = instellingen_io_poort_naam;
    
  // } else 
  if (ts_touched()) {
    int x = touch_x();
    int y = touch_y();

    while (ts_touched()) {
      ts.begin();
    }

    int knop = klik(x, y);


    // Aanpassen functie van een io poort
    if ((knop >= 0) & (knop < 4)) {
      if (instellingen_io_poort_functie == 0) {
        // Als die nu op 0 (ongebruikt) staat hem zetten op de aangeklikte functie
        instellingen_io_poort_functie = knop+1;
        knoppen_status[knop] = 1;
      } else if (instellingen_io_poort_functie == knop+1) {
        // Als die nu op de aangeklikte knop staat hem op 0 (ongebruikt) zetten
        instellingen_io_poort_functie = 0;
        knoppen_status[knop] = 0;
      } else {
        // Anders de status van de knop waar die nu op staat aanpassen en ook de io poort op de aangeklikte functie zetten.
        knoppen_status[instellingen_io_poort_functie - 1] = 0;
        knop_plaatsen(instellingen_io_poort_functie - 1);
        instellingen_io_poort_functie = knop + 1;
        knoppen_status[knop] = 1;
      }
    }


    // regel 2 (de standaard waarden van de uitgang van een io poort)
    if (knop == 4) {
      // default status tussen aan en uit schakelen
      instellingen_io_poort_default = !instellingen_io_poort_default;
      knoppen_status[knop] = instellingen_io_poort_default;
    } else if (knop == 5) {
      // inverse aan of uit
      instellingen_io_poort_inverse = !instellingen_io_poort_inverse;
      knoppen_status[knop] = instellingen_io_poort_inverse;
    } else if (knop == 6) {
      // automatisch opslaan aan of uit (functie werkt nog niet)
      instellingen_io_poort_opslaan = !instellingen_io_poort_opslaan;
      knoppen_status[knop] = instellingen_io_poort_opslaan;
    }

    // regel 3 alert triggers
    if (knop == 7) {
      if (instellingen_io_poort_alert == 0) {
        instellingen_io_poort_alert = 3;
        knoppen_status[knop] = 0;
      } else if (instellingen_io_poort_alert == 1) {
        instellingen_io_poort_alert = 2;
        knoppen_status[knop] = 1;
      } else if (instellingen_io_poort_alert == 2) {
        instellingen_io_poort_alert = 1;
        knoppen_status[knop] = 0;
      } else if (instellingen_io_poort_alert == 3) {
        instellingen_io_poort_alert = 0;
        knoppen_status[knop] = 1;
      }
    } else if (knop == 8) {
      if (instellingen_io_poort_alert == 0) {
        instellingen_io_poort_alert = 2;
        knoppen_status[knop] = 1;
      } else if (instellingen_io_poort_alert == 1) {
        instellingen_io_poort_alert = 3;
        knoppen_status[knop] = 0;
      } else if (instellingen_io_poort_alert == 2) {
        instellingen_io_poort_alert = 0;
        knoppen_status[knop] = 0;
      } else if (instellingen_io_poort_alert == 3) {
        instellingen_io_poort_alert = 1;
        knoppen_status[knop] = 1;
      }
    }

    if ((knop == 10) & (knoppen_status[knop] != 5)) {
      io_poorten_tijdelijk_opslaan();
      naar_instellingen_io_poort(instellingen_io_poort_nummer - 1);
      bouw_instellingen_io_poort_knoppen_plaatsen();
    } else if ((knop == 11) & (knoppen_status[knop] != 5)) {
      io_poorten_tijdelijk_opslaan();
      naar_instellingen_io_poort(instellingen_io_poort_nummer + 1);
      bouw_instellingen_io_poort_knoppen_plaatsen();
    }

    if (knop == 12) {
      io_poorten_tijdelijk_opslaan();
      naar_io_instellingen();
    }

    if ((knop >= 0) & (knop <9)) {
      knop_plaatsen(knop);
    }

    if (y < 40) {
      klik_header(x, y);
    } else if (y < 80) {
      // bouw_tekstinvoer("item naam", 9, instellingen_io_poort_naam);
    }

  }
  ts.begin();
}

#line 1 "/a0/usr/workdir/BKOS5a/io.ino"
void io_boot() {
  io_setup();  // Setup specifiek voor de computer
  io_detect(); // Detecteren hardware op basis van de computer
  bool done = false;
  // if (aparaten_cnt == 1) {
  //   if (aparaten[0] == 130){
  //     // Uitgaande van de basis test configuratie
  //     tft.println("1 module configuratie'(2)");
  //     io_set_defaults(2);
  //     done = true;
  //   }
  // } else if (aparaten_cnt == 2) {
  //   if ((aparaten[0] == 130) && (aparaten[1] == 254)){
  //     // Uitgaande van de basis test configuratie
  //     tft.println("1 module configuratie'(2)");
  //     io_set_defaults(2);
  //     done = true;
  //   } else if ((aparaten[0] == 130) && (aparaten[1] == 130)){
  //     // Uitgaande van de basis 2 module test configuratie
  //     tft.println("2 module configuratie'(3)");
  //     io_set_defaults(3);
  //     done = true;
  //   }
  // } else if (aparaten_cnt == 3) {
  //   if ((aparaten[0] == 130) && (aparaten[1] == 130) && (aparaten[2] == 130)){
  //     // Configuratie Glory
  //     tft.println("Configuratie voor de Glory (1)");
  //     io_set_defaults(1);
  //     done = true;
  //   }
  // }
  if (!done) {
    tft.println("Geen configuratie gevonden.");
    io_set_defaults();
  }
}


String io_code_naar_naam(byte code){
  if (code == 2) {
    return "8 bit Logische module";
  } else if (code == 50) {
    return "8 bit Hub";
  } else if (code == 51) {
    return "Analoge hub";
  } else if (code == 52) {
    return "uart hub";
  } else if (code == 66) {
    return "8 bit Communicatie";
  } else if (code == 130) {
    return "8 bit Schakelmodule";
  } else if (code == 3) {
    return "16 bit Logische module";
  } else if (code == 83) {
    return "16 bit Communicatie";
  } else if (code == 98) {
    return "8 bit Knoppen";
  } else if (code == 115) {
    return "16 bit Knoppen";
  } else if (code == 194) {
    return "8 bit Controller";
  } else if (code == 211) {
    return "16 bit Controller";
  } else if (code == 147) {
    return "16 bit Schakel module";
  } 
  return "";
}

# if IO_METHODE == 2
  /* In HARDWARE modus 2 wordt gebruik gemaakt van de CYD die niet beschikt over mijn communicatie poort.
   * Er zijn 2 oplossingen mogelijk om dit op te lossen. Beide zijn gebaseerd op seriele communicatie met AT tiny micro controller
   *   1. Een tussen module met enkel een ATtiny
   *   2. De eerste 74HC14 chip vervangen voor een 180 graden gedraaide ATtiny 44 of 84 die bij het eerste aparaat de omzetting doet
   * Er moet nog wat getest worden of het handig is om de standaard Serial.print() te gebruiken, of dat er beter een ander kanaal kan worden gebruikt.
   * Op het momnt heeft het eerste mijn voorkeur mocht dat lukken.
   */

  bool io_verbonden = true;

  void io_setup() {
    return;
  }

  void io_detect() {
    /* Deze functie zorgt er vopor dat wordt gedetecteerd welke externe hardware overal is aangesloten.
     * Dit is een vrij complexe taak en zal zonder meer veel tweaken vragen.
     * Alle randapparatuur heeft een ID code ter grootte van 8 bit (1 byte) dat kan worden uitgelezen
     * Nog niet alles is uitgedacht en getest, maar op het moment is het handig uit te gaan van de volgende codes:
     *
     * ID  :   Naam                 |  Registratie  |   Omschrijving (primaire) doel
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   2 : 8 bit logische module  |   8 sensoren  | Ontvangst logisch voltage sensoren (kan ook voor aansturen logisch voltage aparaten worden gebruikt)
     *  50 : 8 bit hub              |       -       | Schakelt tussen verschillende uitgangen. Bij deze moduele moeten alle uitgangen worden bekeken wat hier achter zit.
     *  51 : Analoge hub            |       -       | Schakelt analoog 1 poort van de MCU analoog door zodat een analoge input of output van een andere bijvoorbeeld sensor kan worden geregistreerd.
     *  52 : uart hub               |       -       | Schakelt direct 2 poorten van de MCU analoog door zodat bijvoorbeeld de uart naar een ander aparaat kan linken.
     *  66 : 8 bit communicatie     |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen. Deze items overslaan in het overzicht.
     * 130 : 8 bit Schakel module   |   8 knoppen   | Aansturen aparaten (ook voor 12 volt sensoren te gebruiken)
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   3 : 16 bit logische module |  16 sensoren
     *  83 : 16 bit communicatie    |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen
     *  98 :  8 bit knoppen         |       -       | Aansturing van de boodcomputer met een 8 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 115 : 16 bit knoppen         |       -       | Aansturing van de boodcomputer met een 16 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 194 :  8 bit controller      |       -       | n.t.b. spel controller met 8 knoppen
     * 211 : 16 bit controller      |       -       | n.t.b. spel controller met 16 knoppen
     * 147 : 16 bit Schakel module  |  16 knoppen
     * 
     */
    aparaten_cnt = 0;
    
    int bit;
    int input;
    bool eind = false;
    io_verbonden = true;
    byte factor;
    char A;
    char L;
    // Stap 1 is uitzoeken hoeveel hardware er is aangesloten

    tft.println("  IO detectie");
    tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);

    // Serial.println();
    // Serial.println(Serial.available());

    delay(100);
    
    unsigned long timeout = 5000;
    unsigned long laatste = millis();
    // Start maken in communiceren met de Seriele module door het versie nummer op te vragen.
    // De verwachte response is een 'V' gevolgd door de versie aanduiding welk niet gespecificeerd is als 1 println statement.'

    // Eerst even de serial leeg maken
    Serial.println("IOD");
    delay(50);
    while (Serial.available()) {
      A = Serial.read();
      delay(50);
    }
    tft.print("  ");
    
    while (!eind) {
      bit = 1;
      laatste = millis();
      tft.print(aparaten_cnt+1);
      tft.print(": ");
      input = 0;
      
      Serial.print("00000000");
      while (!Serial.available() && (millis() < laatste + timeout)) {
        delay(50);
      }
      
      while (Serial.available()) {
        A = Serial.read();
        if (A == '1') {
          int add_number = 1;
          for (int i = 1; i < bit; i++){
            add_number = add_number*2;
          }
          input += add_number;
        } 
        bit ++;
        while (bit < 9 && !Serial.available() && (millis() < laatste + timeout)) {
          delay(50);
        } 
      }

      if (bit != 9) {
        eind = true;
        tft.print("Geen 8 bit ontvangen");
      }
      
      if (input == 0) {
        eind = true;
      } else if (input == 255){
        eind = true;
      } else {
        aparaten[aparaten_cnt] = input;
        tft.print(" (");
        tft.print(input);
        tft.print(") ");
        aparaten_cnt ++;
        bit = 1;
        if (aparaten_cnt >= 30) {
          eind = true;
          tft.print("Te veel voor nu");
        }
      }
    }
    
    Serial.print('\n');
    delay(50);
    while (Serial.available()) {
      L = Serial.read();
    }

    if (aparaten_cnt == 0) {
      io_verbonden = false; // was false
      io_lost = millis();
      tft.print("Geen BKOSS module gevonden");
      delay(2500);
    } else {
      tft.println('-');
      tft.print("  ");
      tft.print(aparaten_cnt);
      if (aparaten_cnt == 1) {
        tft.println(" Module gevonden");
      } else {
        tft.println(" Modules gevonden");
      }
      if (aparaten_cnt <= 2) {

      }
      tft.print("  ");
      delay(100);
    }
  }

  void io() {
    // Serial.print("IO");
    char invoer;
    bool tmp_status;
    int i_uit;
    int i_in;

    while (Serial.available()){
      // Leeg maken van oude communicatie
      invoer = Serial.read();
      // tft.print(invoer);
    }
    
    unsigned long timeout = 5000;
    unsigned long laatste = millis();

    if (!io_verbonden &&  millis() > io_lost + timeout) {
      io_verbonden = true;
    }

    if (io_verbonden) {
      if (!io_actief){
        io_actief = true;
        Serial.println("IO");
        delay(10);

        while (Serial.available()){
          delay(50);
        }

        if (io_cnt >= 8) {
          // Een werkvoorraadje maken van 8 opdrachten zodat de complete overdracht (denk ik) sneller verloopt
          for (int i = 0 ; i < 8 ; i++) {
            i_uit = io_cnt - (i+1);
            if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
              Serial.print('1');
            } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
              Serial.print('0');
            }
          }
        } else {
          for (int i = 0 ; i < io_cnt ; i++) {
            i_uit = io_cnt - (i+1);
            if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
              Serial.print('1');
            } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
              Serial.print('0');
            }
          }
        }

        int i = 0;
        while (i < io_cnt && io_verbonden) {
          i_in = i;

          delay(25);
          invoer = ' ';
          int r = 1;
          bool draaien = true;
          bool antwoord = false;
          
          while (!Serial.available() && millis() < laatste + timeout){
            delay(50);
          }

          if (Serial.available()) {
            tft.setTextColor(tft.color565(0, 255, 0));
            invoer = Serial.read();
            if ((invoer == '0') || (invoer == '1')){
              if (invoer == '1') {
                tmp_status = true;
              } else if (invoer == '0') {
                tmp_status = false;
              }

              if (tmp_status != io_input[i_in]) {
                // Bij een wijziging vaststellen dat deze is gewijzigd en ook de aanpassing opslaan
                io_gewijzigd[i_in] = true;
                io_input[i_in] = tmp_status;
                // io_meeschakelen(i_in);
              }

            } else {
              delay(5);
            }
          } else {
            io_verbonden = false; // was false
            io_lost = millis();
            
          }
  
          if (io_cnt > i+8) {
            i_uit = io_cnt - (i+9);
            if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
              Serial.print('1');
            } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
              Serial.print('0');
            }
          } else if (io_cnt == i+8) {
            Serial.print('\n');
          }
          i++;
        }

        while (Serial.available()) {
          invoer = Serial.read();
        }

        io_actief = false;
        io_runned = true;
      }
    }
  }

  void io_reset_alert(){

  }

#else
  /* Wanneer er eigen hardware wordt gebruikt is mijn 10 pins seriele poort wel beschikbaar.
   * Dan kan er gelijk gebruik worden gemaakt van de standaard communicatie wat veel enorm vereenvoudigd.
   */
  
  void io_setup() {
    pinMode(HC_PCK, OUTPUT);
    pinMode(HC_SCK, OUTPUT);
    pinMode(HC_IN, INPUT);
    pinMode(HC_UIT, OUTPUT);
    pinMode(HC_ID, INPUT);
  }
  
  void io_detect() {
    /* Deze functie zorgt er vopor dat wordt gedetecteerd welke externe hardware overal is aangesloten.
     * Dit is een vrij complexe taak en zal zonder meer veel tweaken vragen.
     * Alle randapparatuur heeft een ID code ter grootte van 8 bit (1 byte) dat kan worden uitgelezen
     * Nog niet alles is uitgedacht en getest, maar op het moment is het handig uit te gaan van de volgende codes:
     *
     * ID  :   Naam                 |  Registratie  |   Omschrijving (primaire) doel
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   2 : 8 bit logische module  |   8 sensoren  | Ontvangst logisch voltage sensoren (kan ook voor aansturen logisch voltage aparaten worden gebruikt)
     *  50 : 8 bit hub              |       -       | Schakelt tussen verschillende uitgangen. Bij deze moduele moeten alle uitgangen worden bekeken wat hier achter zit.
     *  51 : Analoge hub            |       -       | Schakelt analoog 1 poort van de MCU analoog door zodat een analoge input of output van een andere bijvoorbeeld sensor kan worden geregistreerd.
     *  52 : uart hub               |       -       | Schakelt direct 2 poorten van de MCU analoog door zodat bijvoorbeeld de uart naar een ander aparaat kan linken.
     *  66 : 8 bit communicatie     |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen. Deze items overslaan in het overzicht.
     * 130 : 8 bit Schakel module   |   8 knoppen   | Aansturen aparaten (ook voor 12 volt sensoren te gebruiken)
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   3 : 16 bit logische module |  16 sensoren
     *  83 : 16 bit communicatie    |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen
     *  98 :  8 bit knoppen         |       -       | Aansturing van de boodcomputer met een 8 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 115 : 16 bit knoppen         |       -       | Aansturing van de boodcomputer met een 16 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 194 :  8 bit controller      |       -       | n.t.b. spel controller met 8 knoppen
     * 211 : 16 bit controller      |       -       | n.t.b. spel controller met 16 knoppen
     * 147 : 16 bit Schakel module  |  16 knoppen
     * 
     */
    

  }

  void io() {

    if (io_actief == false) {

      io_actief = true;
  
      bool tmp_status;
      int i_uit;
      int i_in;
      // int i_in_ = 0;
      // int i_in__ = -1;
    
      // Paralelle klokslag geven
      digitalWrite(HC_PCK, LOW);
      delay(10);
      digitalWrite(HC_PCK, HIGH);
      // delay(10);
        
      // digitalWrite(HC_ID, LOW);
    
      // Nu het paralelle gedeelte geklokt heeft de data serieel uitsturen en uitlezen
      for (int i = 0 ; i < io_cnt ; i++) {
    
        // Seriele klok laag
        digitalWrite(HC_SCK, LOW);
        delay(1);
        
        i_uit = io_cnt - (i+1);
        i_in = i;
        
        // Gewenste object zenden (in omgekeerde volgorde)
        if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
          digitalWrite(HC_UIT, HIGH);
        } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
          digitalWrite(HC_UIT, LOW);
        }
        
        // Invoer lezen
        tmp_status = digitalRead(HC_IN);
        // Controleren of de invoer is gewijzigd. Dit wel in de gewone volgorde omdat dit binnen komend signaal is.
        if (tmp_status != io_input[i_in]) {
          // Bij een wijziging vaststellen dat deze is gewijzigd en ook de aanpassing opslaan
          io_gewijzigd[i_in] = true;
          io_input[i_in] = tmp_status;
          // io_meeschakelen(i_in);
        }
    
        delay(1);
        // Seriele klok hoog
        digitalWrite(HC_SCK, HIGH);
        delay(1);
      
      }
    
      // Paralelle klokslag geven
      digitalWrite(HC_PCK, LOW);
      delay(10);
      digitalWrite(HC_PCK, HIGH);

      io_actief = false;
      io_gecheckt = millis();
    
    }
  }

  void io_reset_alert(){

  }

#endif

void io_schakel(int knop) {
  if (io_output[io_knoppen[knop]] == 0) {
    // uit wordt aan
    io_output[io_knoppen[knop]] = 1;
    knoppen_status[knop] = 1;
  } else if (io_output[io_knoppen[knop]] == 1) {
    // aan wordt uit
    io_output[io_knoppen[knop]] = 0;
    knoppen_status[knop] = 0;
  } else if (io_output[io_knoppen[knop]] == 2) {
    // uit wordt aan (maar dan voor de omgekeerde schakeling)
    io_output[io_knoppen[knop]] = 3;
    knoppen_status[knop] = 1;
  } else if (io_output[io_knoppen[knop]] == 3) {
    // aan wordt uit (maar dan voor de omgekeerde schakeling)
    io_output[io_knoppen[knop]] = 2;
    knoppen_status[knop] = 0;
  }
  knop_plaatsen(knop);
  // io();
  // delay(50);
  // io();
  io_now = true;
}

int io_output_status(byte output) {
  /* Deze functie zet de io_output (0 - 5) om in de weer te geven status (0 - 2) */
  
  if (output == 0) {
    return 0;
  } else if (output == 1) {
    return 1;
  } else if (output == 2) {
    return 0;
  } else if (output == 3) {
    return 1;
  } else if (output == 4) {
    return 2;
  } else if (output == 5) {
    return 2;
  }
  return 99;
}


void io_set_defaults(){
  // io_set_defaults(2);
  io_set_defaults(EEPROM.read(16));
  #ifdef ESP32
  io_state_laden(); // BKOS5a: herstel laatste IO-staat uit NVS
  #endif
  
}

void io_set_defaults(int standaard){
  /* Ik wil een aantal standaarden maken om de module voor het werken met de SD kaart toch flexiebel te maken in de testfase
   * Mijn streven is om op adres 616 van de EEPROM een cijfer op te slaan die refereert aan de gebruikte standaard
   * De standaarden die ik wil inbouwen zijn:
   * 
   1: De setup zoals die op mijn boot is, die kan aanveranderingen onderhevig zijn
   2: Enkel module test/demo setup
   3: Dubbele module test/demo setup
   *
   4: Escaperoom setup met 1 module
   5: Escaperoom setup met 2 modules
   *
   6: demo
   */

  //  int standaard = 2;//EEPROM.read(16);
   tft.print("IO_standaard: ");
   tft.print(' ');
   tft.println(standaard);
   if (standaard == 1){
    io_set_1();
   } else if (standaard == 2){
    io_set_2();
   } else if (standaard == 3){
    io_set_3();
   } else if (standaard == 4){
    io_set_4();
   } else if (standaard == 5){
    io_set_5();
   } else if (standaard == 6){
    io_set_6();
   } else {
    EEPROM.write(16, 2);
    tft.println("Poging dit voor volgende keer op 1 te zetten");
    io_set_2();
    delay(500);
   }
}

void io_set_1(){
  /* Setup voor mijn eigen boot... Mogelijk aan veranderingen onder hevig */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 24;
  io_knoppen_cnt = 11;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**USB    ";
    } else if (nr == 1) {
      io_namen[nr] = "**230    ";
    } else if (nr == 14){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek";
    } else if (nr == 16){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 17){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 18){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 19){
      // io_objecten[nr] = 3;
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 20){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 21){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**IL_rood";
    } else if (nr == 22){
      io_knoppen[9] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 23){
      io_knoppen[10] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**tv     ";
    }
  }
}

void io_set_2(){
  /* Basis setup voor de test module en demo met 1 module 
   * De volgende Dingen kunnen worden aangesloten:
   *
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 8;
  io_knoppen_cnt = 8;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 7){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    }
  }
}

void io_set_3(){
  /* Setup voor de test/demo met 2 modulen
   * Uitgangspunt is een uitbreiding op bovenstaande
   * De eerste module kan worden vervangen door een module met kapote lamp detectie
   * De tweede module niet omdat dit de mogelijkheid van input blokkeert.
   * De volgende Dingen kunnen worden aangesloten:
   *
   MODULE 1:
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   *
   MODULE 2:
   1: USB stopcontacten
   2: 12 volt stopcontacten
   3: 230 volt omvormer
   4: Stuurautomaat
   5: TV
   6: ?
   7: Motor actief (signaal lampje dynamo)
   8: Walstroom actief (kleine 12 volt voeding op walstroom)
   *
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 16;
  io_knoppen_cnt = 13;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 7){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 8){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**USB    ";
    } else if (nr == 9){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**12v    ";
    } else if (nr == 10){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**230    ";
    } else if (nr == 11){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**ST_AUTO";
    } else if (nr == 12){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**TV     ";
    } else if (nr == 13){
      io_knoppen[8] = nr;
      io_objecten[nr] = 0;
      io_namen[nr] = "?";
    } else if (nr == 14){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_MOTOR";
    } else if (nr == 15){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_WAL  ";
    }
  }
}

void io_set_4(){
  /* Basis setup voor de test module en demo met 1 module nog om te schrijven naar escaperoom
   * De volgende Dingen kunnen worden aangesloten:
   *
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 8;
  io_knoppen_cnt = 8;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 7){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    }
  }
}

void io_set_5(){
  /* Setup voor de test/demo met 2 modulen nog om te schrijven naar 2 modulen escaperoom
   * Uitgangspunt is een uitbreiding op bovenstaande
   * De eerste module kan worden vervangen door een module met kapote lamp detectie
   * De tweede module niet omdat dit de mogelijkheid van input blokkeert.
   * De volgende Dingen kunnen worden aangesloten:
   *
   MODULE 1:
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   *
   MODULE 2:
   1: USB stopcontacten
   2: 12 volt stopcontacten
   3: 230 volt omvormer
   4: Stuurautomaat
   5: TV
   6: ?
   7: Motor actief (signaal lampje dynamo)
   8: Walstroom actief (kleine 12 volt voeding op walstroom)
   *
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 16;
  io_knoppen_cnt = 13;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 7){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 8){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**USB    ";
    } else if (nr == 9){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**12v    ";
    } else if (nr == 10){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**230    ";
    } else if (nr == 11){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**ST_AUTO";
    } else if (nr == 12){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**TV     ";
    } else if (nr == 13){
      io_knoppen[8] = nr;
      io_objecten[nr] = 0;
      io_namen[nr] = "?";
    } else if (nr == 14){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_MOTOR";
    } else if (nr == 15){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_WAL  ";
    }
  }
}

void io_set_6(){
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 8;
  io_knoppen_cnt = 8;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < 8; nr++) {
    io_objecten[nr] = 3;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_knoppen[nr] = nr;
    // io_namen[nr] = "?         ";
  }
  io_namen[0] = "**TB_rood";
  io_namen[1] = "**TB_wit ";
  io_namen[2] = "**TB_blau";
  io_namen[3] = "**TB_groe";
  io_namen[4] = "**TB_rood";
  io_namen[5] = "**TB_geel";
  io_namen[6] = "**TB_wit ";
  io_namen[7] = "**TB_blau";

}
#ifdef ESP32
// BKOS5a: NVS IO state persistentie
Preferences _io_prefs;
bool io_state_dirty = false;
unsigned long io_state_last_write = 0;

void io_state_opslaan() {
  if (!io_state_dirty) return;
  _io_prefs.begin("io_state", false);
  _io_prefs.putBytes("out", io_output, io_cnt * sizeof(byte));
  _io_prefs.end();
  io_state_dirty = false;
  io_state_last_write = millis();
}

void io_state_laden() {
  _io_prefs.begin("io_state", true);
  size_t opgeslagen = _io_prefs.getBytesLength("out");
  if (opgeslagen == io_cnt * sizeof(byte)) {
    _io_prefs.getBytes("out", io_output, io_cnt * sizeof(byte));
  }
  _io_prefs.end();
}

void io_state_check() {
  if (io_state_dirty && (millis() - io_state_last_write >= IO_WRITE_DELAY_MS)) {
    io_state_opslaan();
  }
}
#endif

#line 1 "/a0/usr/workdir/BKOS5a/io_instellingen.ino"
void naar_io_instellingen() {
  actieve_app = 9;
  scherm_bouwen = true;
}

void start_io_instellingen() {
  instellingen_io_knoppen_reeks_cnt = 0;
  int i = io_sd_cnt;
  while (i > 0) {
    instellingen_io_knoppen_reeks_cnt++;
    i -= 8;
  }
}
void bouw_io_instellingen () {
  header_plaatsen("io instellingen");
  achtergrond(kleur_zwart, false);
  delete[]knoppen_positie;
  delete[]knoppen_teken_positie;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst;
  delete[]knoppen_tekst2;
  delete[]knoppen_tekst_kleur;

  aantal_knoppen = instellingen_io_knoppen_cnt;
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_tekst2 = new char*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = io_instellingen_knoppen_status;


  for (int i = 0; i < aantal_knoppen; i++) {
    knoppen_teken_positie[i] = instellingen_io_knoppen_positie[i];
    knoppen_positie[i] = instellingen_io_knoppen_positie[i];
    knoppen_basiskleur[i] = instellingen_io_knoppen_kleur;
    knoppen_tekst[i] = instellingen_io_knoppen_tekst[i];
    knoppen_tekst2[i] = instellingen_io_knoppen_tekst2[i];
    knoppen_tekst_kleur[i] = instellingen_io_knoppen_tekstkleur;
  }
  bouw_io_instellingen_knoppen();
}
void bouw_io_instellingen_knoppen() {

  // Bepalen of de knoppen voor een reeks eerder of later actief gemaakt moeten worden
  if (instellingen_io_knoppen_reeks == 0) {
    knoppen_status[12] = 5;
  } else {
    knoppen_status[12] = 2;
  }
  if (instellingen_io_knoppen_reeks+1 <= instellingen_io_knoppen_reeks_cnt) {
    knoppen_status[13] = 2;
  } else {
    knoppen_status[13] = 5;
  }

  char buf[10];

  String(io_sd_cnt).toCharArray(knoppen_tekst[0], 10);
  // knoppen_tekst[0] = buf;

  // Nummering van de io poorten aan de knoppen toewijzen. Dit enkel als cijfer.
  for (int i = 0; i < 8; i++) {
    if ((i + instellingen_io_knoppen_reeks*8) < io_sd_cnt) {
      String(i + instellingen_io_knoppen_reeks*8).toCharArray(knoppen_tekst[i+2], 10);
      io_sd[i + instellingen_io_knoppen_reeks*8].substring(4).toCharArray(knoppen_tekst2[i+2], 10);
      // knoppen_status[i+2] = SD_byte(io_sd[i + instellingen_io_knoppen_reeks*8].substring(0, 1));
    } else {
      String tmp_string = "";
      tmp_string.toCharArray(knoppen_tekst[i+2], 10);
      // Serial.println(buf);
      // knoppen_tekst[i+2] = buf;
      knoppen_status[i+2] = 5;
    }
  }

  alle_knoppen_plaatsen(true);

}

void run_io_instellingen() {
  if (ts_touched()) {
    int x = touch_x();
    int y = touch_y();

    while (ts_touched()) {
      ts.begin();
    }

    int knop = klik(x, y);

    if (knop == 0) {
      actieve_app = 11;
      scherm_bouwen = true;
    } else if ((knop >= 1) & (knop < 11) & (knoppen_status[knop] != 5)) {
      actieve_app = 10;
      scherm_bouwen = true;
      naar_instellingen_io_poort(String(knoppen_tekst[knop]).toInt());
    } else if (knop == 11) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (knop == 14) {
      if (beeld_vraag("Opslaan", "Aanpassingen opslaan?", "JA", "NEE") == 0) {
        beeld_melding("Even geduld . . .", "Aanpassingen worden opgeslagen", "opslaan");
        delay(1000);
        // if (sd_save_io()) {
        //   SD_connect = true;
        //   beeld_melding("opgeslagen","IO instellingen opgeslagen", "opslaan");
        //   delay(1000);
        //   sd_io();
        //   start_io();
        // } else {
        //   SD_connect = false;
        //   beeld_vraag("mislukt","niet opgeslagen", "das balen :(");
        // }
        actieve_app = 0;
        scherm_bouwen = true;
      } else {
        scherm_bouwen = true;
      }
    } else if ((knop == 12) & (knoppen_status[12] == 2)) {
      instellingen_io_knoppen_reeks--;
      bouw_io_instellingen_knoppen();
    } else if ((knop == 13) & (knoppen_status[13] == 2)) {
      instellingen_io_knoppen_reeks++;
      bouw_io_instellingen_knoppen();
    // } else if ((knop >= 2) & (knop <= 10) & (knoppen_status[knop] != 5)) {
    //   beeld_melding("Knop instellingen in aanbouw");
    // } else {
      
    }
    if (y < 40) {
      klik_header(x, y);
    }

  }
  ts_begin();
}

#line 1 "/a0/usr/workdir/BKOS5a/nummeriek.ino"
/*
 * Nummeriek werkt door de parameter bouw_nummeriek te starten
 * 
 * Zorg dat in je functie staat 'if (nummeriek_actief) {run_nummeriek();}
 * 
 * wanneer nummeriek_actief op false staat zal de uitkomst op te halen zijn als 'nummeriek_invoer'
 */
void bouw_nummeriek() {
  bouw_nummeriek("");
}
void bouw_nummeriek(String naam) {
  delete[]knoppen_teken_positie;
  delete[]knoppen_positie;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst;
  delete[]knoppen_tekst_kleur;

  aantal_knoppen = 12;
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = knoppen_nummeriek_status;

  for (int i=0 ; i<aantal_knoppen ; i++) {
    knoppen_teken_positie[i] = knoppen_nummeriek_positie[i];
    knoppen_positie[i] = knoppen_nummeriek_positie[i];
    knoppen_basiskleur[i] = instellingen_io_knoppen_kleur;
    knoppen_tekst[i] = knoppen_nummeriek_inhoud[i];
    knoppen_tekst_kleur[i] = instellingen_io_knoppen_tekstkleur;
    
  }

  nummeriek_invoer = 0;
  nummeriek_actief = true;
  header_plaatsen(naam);
  achtergrond(kleur_wit, true);
  alle_knoppen_plaatsen();
  fillRect(5, 30, 230, 40, kleur_zwart);
//  homeknop_plaatsen();
}


void nummeriek_cijfer_toevoegen(int invoer) {
  // Deze functie voegt een cijfer toe
  center_tekst(120, 35, String(nummeriek_invoer), 4, kleur_zwart);
  nummeriek_invoer = nummeriek_invoer * 10 + invoer;
  center_tekst(120, 35, String(nummeriek_invoer), 4, kleur_wit);
}

void nummeriek_cijfer_verwijderen() {
  // Deze functie vewijderd een cijfer en bij 0 wordt afgesloten
  if (nummeriek_invoer == 0) {
    // escape functie
    nummeriek_enter_esc();
  } else {
    center_tekst(120, 35,String(nummeriek_invoer), 4, kleur_zwart);
    nummeriek_invoer = floor(nummeriek_invoer / 10);
    center_tekst(120, 35,String(nummeriek_invoer), 4, kleur_wit);
  }
}

void nummeriek_enter_esc() {
  // Deze functie sluit nummeriek af. Bij escape is de uitkomst 0 en anders wat het moet zijn
  nummeriek_actief = false;
}

void run_nummeriek() {
  if (ts_touched()) {
    int x = touch_x();
    int y = touch_y();

    while (ts_touched()) {
      ts.begin();
    }

    int knop = klik(x, y);


    if (y < 40) {
      klik_header(x, y);
    } else if (y > home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (knop <= 9) {
      // cijfer toevoegen
      nummeriek_cijfer_toevoegen(knop);
    } else if (knop == 10) {
      // cijfer weghalen
      nummeriek_cijfer_verwijderen();
    } else if (knop == 11) {
      // enter
      nummeriek_enter_esc();
    } 

  }
  ts.begin();
}

#line 1 "/a0/usr/workdir/BKOS5a/ota.ino"
void ota_setup(){
  ota_setup(false);
}

void ota_setup(bool displayed){
  connectToWiFi(displayed);
  ota_git_update();
  ArduinoOTA.setHostname("BKOS4");
  ota_wifi_setup(displayed);
}

void connectToWiFi(){
  connectToWiFi(false);
}

void connectToWiFi(bool displayed) {
  
  WiFi.mode(WIFI_STA);
  
  if (WiFi.status() != WL_CONNECTED) {
    uint16_t limiet = millis()+5000;
    WiFi.begin();
    while ((WiFi.status() != WL_CONNECTED) && (millis() < limiet)) {
      delay(500);
    }
  }

  // WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if (displayed){
      tft.print(".");
    }
  }
  wifi__verbonden = true;
  // tft.println("\nWiFi connected");
  // tft.println("IP address: " + WiFi.localIP().toString());
}

void disconnectWiFi() {
  WiFi.disconnect();
  wifi__verbonden = false;
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    wifi__verbonden = false;
  } else {
    wifi__verbonden = true;
  }
}
#line 1 "/a0/usr/workdir/BKOS5a/ota_git.ino"



// // Current firmware version
// const unsigned long updateCheckInterval = 5 * 60 * 1000;  // 5 minutes in milliseconds
// unsigned long lastUpdateCheck = 0;

void ota_git_update() {
  // tft.println("\nStarting ESP32 OTA Update");

  connectToWiFi();
  delay(1000);

  if (checkForFirmwareUpdate()) {
    updaten = true;
    delay(500);
    tft.fillScreen(kleur_zwart);
    bkos_logo(10, 10, kleur_donker);
    tft.setCursor(300, 10);
    tft.setTextSize(3);
    tft.setTextColor(kleur_groen);
    tft.println("BKOS update");
    tft.setTextColor(kleur_beige);
    tft.setCursor(300, 45);
    tft.print(BKOS_VERSIE);
    tft.print(" > ");
    tft.println(BKOS_VERSIE_GIT);
    downloadAndApplyFirmware();
  } else {
    // tft.setTextColor(kleur_actief_groen);
    // tft.println("BKOS4 versie is actueel:");
    // tft.setTextSize(3);
    // tft.println(BKOS_VERSIE_GIT);
    // tft.setTextSize(1);
    // tft.setTextColor(kleur_wit);
    // delay(3000);
  }
}

// void loop() {
//   tft.println("Current Firmware Version: " + String(BKOS_VERSIE));
//   delay(3000);  // delay to prevent flooding serial
// }



bool checkForFirmwareUpdate() {
  // tft.println("Checking for firmware update...");
  if (WiFi.status() != WL_CONNECTED) {
    // tft.println("WiFi not connected");
    return false;
  }

  // Step 1: Fetch the latest version from GitHub
  BKOS_VERSIE_GIT = fetchLatestVersion();
  BKOS_GIT_ALLOWED = fetchAlowedVersions();
  if (BKOS_VERSIE_GIT == "") {
    return false;
  }

  // Step 2: Compare versions
  // if (BKOS_VERSIE_GIT != BKOS_VERSIE) {
  //   tft.print(BKOS_VERSIE_GIT);
  //   return true;
  // }
  String str_BKOS_VERSIE = BKOS_VERSIE;
  if (BKOS_GIT_ALLOWED == "") {
    if (BKOS_VERSIE_GIT != BKOS_VERSIE) {
      tft.print(BKOS_VERSIE_GIT);
      return true;
    }
  }
  if (not str_BKOS_VERSIE.indexOf(BKOS_GIT_ALLOWED) > 0) {
    return true;
  }
  return false;
}

String fetchAlowedVersions() {
  HTTPClient http;
  if (strncmp(BKOS_VERSIE, "4", 1) == 0) {
    http.begin(aversionUrl);
  } else if (strncmp(BKOS_VERSIE, "5", 1) == 0) {
    http.begin(aversion5Url);
  } else {
    http.begin(aversionUrl);
  }

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String latestVersion = http.getString();
    latestVersion.trim();  // Remove any extra whitespace
    http.end();
    return latestVersion;
  } else {
    tft.printf("Failed to fetch version. HTTP code: %d\n", httpCode);
    http.end();
    return "";
  }
}

String fetchLatestVersion() {
  HTTPClient http;
  if (strncmp(BKOS_VERSIE, "4", 1) == 0) {
    http.begin(versionUrl);
  } else if (strncmp(BKOS_VERSIE, "5", 1) == 0) {
    http.begin(version5Url);
  } else {
    http.begin(versionUrl);
  }

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String latestVersion = http.getString();
    latestVersion.trim();  // Remove any extra whitespace
    http.end();
    return latestVersion;
  } else {
    tft.printf("Failed to fetch version. HTTP code: %d\n", httpCode);
    http.end();
    return "";
  }
}

String fetchVersion5() {
  HTTPClient http;
  http.begin(version5Url);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String v = http.getString();
    v.trim();
    http.end();
    return v;
  }
  http.end();
  return "";
}

void downloadAndApplyFirmware() {
  if (strncmp(BKOS_VERSIE, "4", 1) == 0) {
    downloadAndApplyFirmware(4);
  } else if (strncmp(BKOS_VERSIE, "5", 1) == 0) {
    downloadAndApplyFirmware(5);
  } else {
    downloadAndApplyFirmware(4);
  }
}

void downloadAndApplyFirmware(byte BKOS) {
  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  if (BKOS == 5) {
    http.begin(firmware5Url);
  } else {
    http.begin(firmwareUrl);  // BKOS 4 of standaard
  }

  int httpCode = http.GET();
  // tft.printf("HTTP GET code: %d\n", httpCode);

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    // tft.printf("Firmware size: %d bytes\n", contentLength);

    if (contentLength > 0) {
      WiFiClient* stream = http.getStreamPtr();
      if (startOTAUpdate(stream, contentLength)) {
        tft.println("OTA update successful, restarting...");
        delay(2000);
        ESP.restart();
      } else {
        tft.println("OTA update failed");
      }
    } else {
      tft.println("Invalid firmware size");
    }
  } else {
    tft.printf("Failed to fetch firmware. HTTP code: %d\n", httpCode);
  }
  http.end();
}


bool startOTAUpdate(WiFiClient* client, int contentLength) {
  // tft.println("Initializing update...");
  if (!Update.begin(contentLength)) {
    tft.printf("Update begin failed: %s\n", Update.errorString());
    return false;
  }

  // tft.println("Writing firmware...");
  size_t written = 0;
  int progress = 0;
  int lastProgress = 0;

  // Timeout variables
  const unsigned long timeoutDuration = 120*1000;  // 10 seconds timeout
  unsigned long lastDataTime = millis();

  tft.setTextColor(kleur_beige);
  tft.setTextSize(5);
  while (written < contentLength) {
    if (client->available()) {
      uint8_t buffer[128];
      size_t len = client->read(buffer, sizeof(buffer));
      if (len > 0) {
        Update.write(buffer, len);
        written += len;

        // Calculate and print progress
        progress = 50 + (written * 50.0 / contentLength); // geschatte progress
        if (progress != lastProgress) {
          // tft.fillRect(300, 120, 200, 200, kleur_zwart); // use function
          tft.setCursor(350, 125);
          draw_update_progress(progress);
          lastProgress = progress;
        }
      }
    }
    // Check for timeout
    if (millis() - lastDataTime > timeoutDuration) {
      tft.println("Timeout: No data received for too long. Aborting update...");
      Update.abort();
      return false;
    }

    yield();
  }
  tft.setTextColor(kleur_wit);
  tft.setTextSize(2);
  tft.print('\n');
  
  tft.println("\nWriting complete");

  if (written != contentLength) {
    tft.printf("Error: Write incomplete. Expected %d but got %d bytes\n", contentLength, written);
    Update.abort();
    return false;
  }

  if (!Update.end()) {
    tft.printf("Error: Update end failed: %s\n", Update.errorString());
    return false;
  }

  tft.println("Update geslaagd");
  return true;
}
void draw_update_progress(int progress) {
  tft.fillRect(100, 200, TFT_WIDTH-200, 20, kleur_zwart); // clear bar
  tft.fillRect(100, 200, (TFT_WIDTH-200)*(progress/100.0), 20, tft.color565(0,100,255)); // blue progress
  tft.setTextColor(kleur_wit);
  tft.setTextSize(2);
  tft.setCursor((TFT_WIDTH/2)-15, 200);
          draw_update_progress(progress);
}

#line 1 "/a0/usr/workdir/BKOS5a/ota_wifi.ino"
void ota_wifi_update() {
  ArduinoOTA.handle();
}

void ota_wifi_setup(){
  ota_wifi_setup(true);
}

void ota_wifi_setup(bool displayed) {
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    tft.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setPassword("admin");

  ArduinoOTA
    .onStart([]() {
      String type;
      digitalWrite(TFT_BL, HIGH);
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      tft.println("Start updating " + type);
      updaten = true;
      tft.fillScreen(kleur_zwart);
      bkos_logo(10, 10, kleur_donker);
      tft.setCursor(300, 10);
      tft.setTextSize(3);
      tft.setTextColor(kleur_groen);
      tft.println("BKOS update");
      // unsigned int lastProgress = 0;
      // unsigned int actProgress = 0;
  
    })
    .onEnd([]() {
      tft.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      // actProgress = "%u%%\r", (progress / (total / 100));
      if (true){//(actProgress != lastProgress) {
          tft.fillRect(300, 120, 200, 200, kleur_zwart);
          tft.setCursor(350, 125);
          tft.printf("%u%%\r", (progress / (total / 100)));
          // lastProgress = actProgress;
        }
    })
    .onError([](ota_error_t error) {
      tft.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  if (displayed) {
    tft.println("OTA Initialized");
    tft.print("IP address: ");
    tft.println(WiFi.localIP());
  }

}
#line 1 "/a0/usr/workdir/BKOS5a/wifimanager.ino"
void wifimanager_app(String actie) {
  if (actie == "bouw") {
    wifimanager(true);
  } else if (actie == "run") {
    actieve_app = 0;
    scherm_bouwen = true;
  } else if (actie == "naam") {
    item_naam = "WiFi Manager";
  } else if (actie == "print") {
    update_center("naam");
    tft.print(item_naam);
  } else if (actie == "icon_klein") {
    int x = getCursorX() + 20;
    int y = getCursorY() + 25;
    setCursor(getCursorX(), getCursorY()+4);
    drawIcon10x10(x, y, icon_wifi, kleur_zwart, 3);
    tft.setTextColor(kleur_rood);
  }
}

void wifimanager() {
  wifimanager(false);
}

void wifimanager(bool forceConfig) {
  // bool forceConfig = false;

  bool spiffsSetup = loadConfigFile();
  if (!spiffsSetup) {
    forceConfig = true;
  }

  // tft.println("WiFi.mode");
  // delay(250);
  WiFi.mode(WIFI_STA);

  // tft.println("loadConfigFile");
  // delay(250);
  loadConfigFile();
  // tft.println("WiFiManager wm");
  // delay(100);
  WiFiManager wm;
  // tft.println("setDebug");
  wm.setDebugOutput(false);
  // tft.println("setSaveConfigCallback");
  wm.setSaveConfigCallback(saveConfigCallback);
  // tft.println("setAPCallback");
  wm.setAPCallback(configModeCallback);

  // tft.println("WiFiManagerParameters");
  // delay(250);
  WiFiManagerParameter veld_bootnaam("bootnaam", "Naam boot", bootnaam, 25);
  WiFiManagerParameter veld_haven("haven", "Naam haven", haven, 25);
  WiFiManagerParameter veld_eigenaar("eigenaar", "Naam eigenaar", eigenaar, 25);
  WiFiManagerParameter veld_telefoon("telefoon", "Telefoonnummer", telefoon, 25);
  // WiFiManagerParameter veld_boot_configuratie("boot_configuratie", "boot configuratie", boot_configuratie, 2);

  tft.println("addParameters");
  wm.addParameter(&veld_bootnaam);
  wm.addParameter(&veld_haven);
  wm.addParameter(&veld_eigenaar);
  wm.addParameter(&veld_telefoon);
  // wm.addParameter(&veld_boot_configuratie);



  
  
  wifi__aangesloten = true;

  if (forceConfig) {
      tft.fillScreen(kleur_zwart);
  
    tft.setCursor(0,0);
    tft.setTextColor(kleur_wit);
    tft.setTextSize(2);
    bkos_logo(scherm_x(240) - 210, 10, kleur_donker);
    tft.println("    Verbinden met WiFi . . .");
    tft.println("    Ga met je telefoon naar het netwerk");
    tft.setTextSize(3);
    tft.setTextColor(kleur_beige);
    tft.println("               BKOS4");
    tft.setTextSize(2);
    tft.setTextColor(kleur_wit);
    tft.println("    Ga vervolgens naar het volgende IP adres:");
    tft.setTextSize(3);
    tft.setTextColor(kleur_beige);
    tft.println("            192.168.4.1");
    tft.setTextSize(2);
    tft.setTextColor(kleur_wit);
    tft.println("    om de juiste instellingen in te voeren");
    tft.println("");
    tft.setTextSize(3);
    tft.println("            LETOP ! ! !");
    tft.setTextColor(kleur_actief_rood);
    tft.setTextSize(2);
    tft.println("    Geef altijd de WiFi gegevens opnieuw op!");
    tft.setTextColor(kleur_wit);  
    tft.println("    anders gaat WiFi managr helaas niet verder.");
    
    wm.setConfigPortalTimeout(120);
    if (!wm.startConfigPortal("BKOS4")) {
      tft.println("Verbinding mislukt (startConfigPortal)");
      delay(3000);
      // ESP.restart();
      // delay(5000);
      wifi__aangesloten = false;
    }
  } else {
    tft.println("autoConnect");
    wm.setTimeout(10);
    if (!wm.autoConnect("BKOS4")) {
      tft.println("Verbinding mislukt (autoConnect)");
      delay(1000);
      wifi__aangesloten = false;
      // ESP.restart();
      // delay(5000);
    // } else {
    //   tft.print("continue");
    }
  }
  // tft.println("strncpy");

  strncpy(bootnaam, veld_bootnaam.getValue(), sizeof(bootnaam));
  strncpy(haven, veld_haven.getValue(), sizeof(haven));
  strncpy(eigenaar, veld_eigenaar.getValue(), sizeof(eigenaar));
  strncpy(telefoon, veld_telefoon.getValue(), sizeof(telefoon));
  // strncpy(boot_configuratie, veld_boot_configuratie.getValue(), sizeof(boot_configuratie));



  if (shouldSaveConfig) {
    tft.println("saceConfigFile");
    saveConfigFile();
  }

  // tft.println("WiFiManager compleet");
}

void saveConfigFile(){
  StaticJsonDocument<512> json;
  json["bootnaam"] = bootnaam;
  json["haven"] = haven;
  json["eigenaar"] = eigenaar;
  json["telefoon"] = telefoon;
  // json["boot_configuratie"] = boot_configuratie;

  File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");

  if (!configFile) {
    tft.print("Configuratiebestand kan niet worden geopend om te schfijven");
  }

  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0) {
    tft.print("Schrijven mislukt");
  }
  configFile.close();
}
  
bool loadConfigFile() {
  if (SPIFFS.begin(true || SPIFFS.begin(true))) {
    if (SPIFFS.exists(JSON_CONFIG_FILE)) {
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
      if (configFile) {
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        serializeJsonPretty(json, Serial);
        if (!error) {
          strcpy(bootnaam, json["bootnaam"]);
          strcpy(haven, json["haven"]);
          strcpy(eigenaar, json["eigenaar"]);
          strcpy(telefoon, json["telefoon"]);
          // strcpy(tmp_boot_configuratie, json["boot_configuratie"]);
          return true;
        }
      }
    }
  }
  return false;
}

void saveConfigCallback() {
  shouldSaveConfig = true;
}

void configModeCallback(WiFiManager *myWiFiManager) {
  tft.println(myWiFiManager->getConfigPortalSSID());
}
