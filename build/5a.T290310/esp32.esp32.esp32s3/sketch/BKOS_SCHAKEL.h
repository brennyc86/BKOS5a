#line 1 "/a0/usr/workdir/BKOS5a/BKOS_SCHAKEL.h"
void schakelscherm(String actie);

int schakelscherm_knoppen_positie[10][4] = {{10,30,65,40}, {10,80,65,40}, {10,130,65,40}, {10,180,65,40}, {10,230,65,40}, {165,30,65,40}, {165,80,65,40}, {165,130,65,40}, {165,180,65,40}, {165,230,65,40}};
int schakelscherm_bolletjes[10][2] = {{100, 50}, {100, 100}, {100, 150}, {100, 200}, {100, 250}, {140, 50}, {140, 100}, {140, 150}, {140, 200}, {140, 250}};
uint16_t schakelscherm_knoppen_kleur[3] = {tft.color565(0, 255, 0), tft.color565(0, 255, 255), tft.color565(255, 0, 0 )}; // {uit, aan, geblokeerd} {groen, cyaan, rood}
uint16_t schakelscherm_knoppen_tekst_kleur[3] = {tft.color565(0, 0, 0), tft.color565(0, 0, 0), tft.color565(255, 255, 255 )}; // {uit, aan, geblokeerd} {zwart, zwart, wit}

