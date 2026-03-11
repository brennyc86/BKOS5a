#line 1 "/a0/usr/workdir/BKOS5a/app_configmodus.h"
void configmodus(String actie);


void schakelscherm(String actie);

int configmodus_knoppen_positie[10][4] = {{165,30,65,40}, {10,30,55,30}, {10,70,55,30}, {10,110,55,30}, {10,150,55,30}, {10,190,55,30}, {10,240,55,30}, {165,80,65,80}, {165,130,65,40}};
// int schakelscherm_bolletjes[10][2] = {{100, 50}, {100, 100}, {100, 150}, {100, 200}, {100, 250}, {140, 50}, {140, 100}, {140, 150}, {140, 200}, {140, 250}};
// uint16_t schakelscherm_knoppen_kleur[3] = {tft.color565(0, 255, 0), tft.color565(0, 255, 255), tft.color565(255, 0, 0 )}; // {uit, aan, geblokeerd} {groen, cyaan, rood}
// uint16_t schakelscherm_knoppen_tekst_kleur[3] = {tft.color565(0, 0, 0), tft.color565(0, 0, 0), tft.color565(255, 255, 255 )}; // {uit, aan, geblokeerd} {zwart, zwart, wit}

char configmodus_knoppen_namen[10][10] = {"AUTO", "1", "2", "3", "4", "5", "6", "Opslaan", "", ""};
int huidige_configuratie;