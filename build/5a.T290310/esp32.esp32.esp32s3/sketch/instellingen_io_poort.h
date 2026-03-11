#line 1 "/a0/usr/workdir/BKOS5a/instellingen_io_poort.h"
void naar_instellingen_io_poort(int poort);
void io_poorten_tijdelijk_opslaan();
void bouw_instellingen_io_poort();
void bouw_instellingen_io_poort_knoppen_plaatsen();
void run_instellingen_io_poort();

int instellingen_io_poort_knoppen_cnt = 13;

int instellingen_io_poort_nummer;
char instellingen_io_poort_naam[10];
byte instellingen_io_poort_functie;
bool instellingen_io_poort_default;
bool instellingen_io_poort_inverse; 
bool instellingen_io_poort_opslaan;
byte instellingen_io_poort_alert;

char instellingen_io_poort_knoppen_tekst[13][10] = {
  "knop", "sensor", "knop (uc)", "check",
  "aan / uit", "inverse", "opslaan",
  "passief", "actief",
  "event", "<", ">", "enter"
};

int instellingen_io_poort_knoppen[13][4] = {
  {10, 95, 50, 40}, {68, 95, 50, 40}, {123, 95, 50, 40}, {180, 95, 50, 40},
        {10, 155, 105, 40}          , {123, 155, 50, 40}, {180, 155, 50, 40},
                    {10, 215, 105, 40}, {125, 215, 105, 40},
  {10, 275, 50, 40}, {68, 275, 50, 40}, {123, 275, 50, 40}, {180, 275, 50, 40},
};

byte instellingen_io_poort_knoppen_status[13] = {
  0, 0, 0, 0,
    0,  0, 0,
     0, 0,
  2, 5, 5, 3,
};

uint16_t instelligen_io_poort_knoppen_kleur[6] = {
  // 0: niet                   1: wel                 2: event (blauw)        3: enter (fel groen)      4: navigatie/ overig         5: passief
  tft.color565(100, 0, 0), tft.color565(0, 200, 0), tft.color565(0, 0, 255), tft.color565(0, 255, 0), tft.color565(175, 175, 175), tft.color565(30, 30, 30)
};

uint16_t instelligen_io_poort_knoppen_tekstkleur[6] = {
  // 0: niet                        1: wel                   2: event (blauw)      3: enter (fel groen)   4: navigatie/ overig         5: passief
  tft.color565(255, 255, 255), tft.color565(0, 0, 0), tft.color565(255, 255, 255), tft.color565(0, 0, 0), tft.color565(0, 0, 0), tft.color565(45, 45, 45)
};