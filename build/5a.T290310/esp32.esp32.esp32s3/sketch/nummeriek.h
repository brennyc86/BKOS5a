#line 1 "/a0/usr/workdir/BKOS5a/nummeriek.h"
void bouw_nummeriek();
void bouw_nummeriek(String naam);
void nummeriek_cijfer_toevoegen(int invoer);
void nummeriek_cijfer_verwijderen();
void nummeriek_enter_esc();
void run_nummeriek();


int knoppen_nummeriek_positie[12][4] = {{85, 230, 70, 40}, // 0 staat midden onderin, maar hier in de array om het technisch makkelijker te maken
                                    {10, 80, 70, 40}, {85, 80, 70, 40}, {160, 80, 70, 40},
                                    {10, 130, 70, 40}, {85, 130, 70, 40}, {160, 130, 70, 40},
                                    {10, 180, 70, 40}, {85, 180, 70, 40}, {160, 180, 70, 40},
                                    {10, 230, 70, 40},                    {160, 230, 70, 40}};

char knoppen_nummeriek_inhoud[12][10] = {"0", // 0 staat midden onderin, maar hier in de array om het technisch makkelijker te maken
                                       "1", "2", "3",
                                       "4", "5", "6",
                                       "7", "8", "9",
                                       "<",  "enter"};

byte knoppen_nummeriek_status[15] = {2,
                                         2, 2, 2,
                                         2, 2, 2,
                                         2, 2, 2,
                                         3  ,  1};

int nummeriek_invoer;
byte nummeriek_laatste;

bool nummeriek_actief = false;
