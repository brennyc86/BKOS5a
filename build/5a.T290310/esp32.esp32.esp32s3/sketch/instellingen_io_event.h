#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_event.h"
int instellingen_io_poort_knoppen_cnt = 7;

char instellingen_io_event_knoppen_tekst[7][10] = {
  "passief_0", "passief_1",
  "actief_0", "actief_1",
  "<", ">", "enter"
};

int instellingen_io_evemt_knoppen[7][4] = {
  
  {10, 95, 105, 40}, {125, 95, 105, 40},
        {10, 155, 105, 40}          , {125, 155, 105, 40},
  {68, 275, 50, 40}, {123, 275, 50, 40}, {180, 275, 50, 40},
};

byte instellingen_io_event_knoppen_status[7] = {
  0, 0,
  0, 0,
  5, 5, 3,
};

uint16_t instelligen_io_event_knoppen_kleur[6] = {
  // 0: niet                   1: wel                 2: event (blauw)        3: enter (fel groen)      4: navigatie/ overig         5: passief
  tft.color565(100, 0, 0), tft.color565(0, 200, 0), tft.color565(0, 0, 255), tft.color565(0, 255, 0), tft.color565(175, 175, 175), tft.color565(30, 30, 30)
};

uint16_t instelligen_io_event_knoppen_tekstkleur[6] = {
  // 0: niet                        1: wel                   2: event (blauw)      3: enter (fel groen)   4: navigatie/ overig         5: passief
  tft.color565(255, 255, 255), tft.color565(0, 0, 0), tft.color565(255, 255, 255), tft.color565(0, 0, 0), tft.color565(0, 0, 0), tft.color565(45, 45, 45)
};