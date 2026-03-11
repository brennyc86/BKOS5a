#line 1 "/a0/usr/workdir/BKOS5a/div_schermacties.h"
int scherm_helderheid = 100;
int scherm_timer = 20;
long scherm_touched = 0;

bool slepen_toestaan = false;

bool waas = false;



// #if scherm_ili == 9341
//   // Standaard scherm
//   #if hoofd_module__model <= 1 || hoofd_module__model == 3
//     Adafruit_ILI9341 tft = Adafruit_ILI9341(cs_tft, dc, mosi, sck, rst, miso);
//   #else
//     Adafruit_ILI9341 tft = Adafruit_ILI9341(cs_tft, dc, mosi, sck, rst);
//   #endif
// #elif scherm_ili == 9488
//   // 4 inch scherm
//   TFT_eSPI tft = TFT_eSPI(320, 480);
// #endif
// XPT2046_Touchscreen ts(cs_ts, irq);

void start_scherm();
void beeld_melding(String hoofdmelding);
void beeld_melding(String hoofdmelding , String submelding);
void beeld_melding(String hoofdmelding , String submelding, String titel);
void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10]);
void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10], bool sec_icon_bool);
byte beeld_vraag(String hoofdmelding, String submelding, String optie1);
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2);
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3);
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4);
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4, String optie5);
byte beeld_vraag(String hoofdmelding, String submelding, byte opties_cnt, String optie1, String optie2, String optie3, String optie4, String optie5);
void gewaasd_scherm();
void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte, uint16_t tekst_kleur);
void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte, uint16_t tekst_kleur, bool schalen);
void run_scherm_pauze();


// Omreken functie om punten naar een andere resolutie om te kunnen rekenen. Bedoeld om simpel van het 2,8 naar het 4,0 inch scherm te kunnen gaan
int16_t scherm_x(int16_t invoer);
int16_t scherm_x(int16_t invoer, int16_t code_resolutie);
int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie);
int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max);
int16_t resolutie_x(int16_t resolutie, bool eind);
int getCursorX();
int getCursorX(int16_t code_resolutie);
int getCursorX(int16_t code_resolutie, int16_t scherm_resolutie);

int16_t scherm_y(int16_t invoer);
int16_t scherm_y(int16_t invoer, int16_t code_resolutie);
int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie);
int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max);
int16_t resolutie_y(int16_t resolutie, bool eind);
int getCursorY();
int getCursorY(int16_t code_resolutie);
int getCursorY(int16_t code_resolutie, int16_t scherm_resolutie);


// Een simpele link naar de standaard tft. functies echter via de bovenstaande omreken functies om deze te kunnen toepassen door enkel tft. weg te halen :)
void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur);
void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal);
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur);
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t , uint16_t schaal);
void fillRect(int x, int y, int w, int h, uint16_t kleur);
void fillRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal);
void drawRect(int x, int y, int w, int h, uint16_t kleur);
void drawRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal);
void setCursor(int x, int y);
void setCursor(int x, int y, uint16_t schaal);
void fillCircle(int x, int y, int r, uint16_t kleur);
void fillCircle(int x, int y, int r, uint16_t kleur, uint16_t schaal);
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur);
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur, uint16_t schaal);

void scherm_timeout_waarschuwing(); // BKOS5a
