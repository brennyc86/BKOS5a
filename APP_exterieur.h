void exterieur(String actie);
void exterieur_teken_boot(int32_t x, int32_t y);

byte exterieur_knoppen_cnt = 9;

int exterieurscherm_knoppen_positie[9][4] = {{165,30,65,40}, {165,80,65,40}, {165,130,65,40}, {165,180,65,40}, {10,230,40,40}, {55,230,40,40}, {100,230,40,40}, {145,230,40,40}, {190,230,40,40}};
uint16_t exterieruscherm_knoppen_kleur[4] = {tft.color565(0, 255, 0), tft.color565(0, 255, 255), tft.color565(0, 255, 255), tft.color565(0, 255, 255)}; // {uit, aan, geblokeerd} {groen, cyaan, cyaan, cyaan}

char exterieur_knoppen_namen[][10] =          {"**haven  ",   "**zeilen ",   "**motor  ",    "**anker  ",    "**USB    ",   "**230    ",  "**I_licht", "**tv     ", "**E_dek  "};
// char exterieur_knoppen_namen[][10] =          {"**haven  ",   "**zeilen ",   "**motor  ",    "**anker  ",    "**TB_rood",   "**TB_wit ",  "**TB_blau", "**TB_groe", "**TB_geel"};
byte exterieurscherm_status[] = {1, 0, 0, 0, 0, 0, 3, 0, 0};

byte exterieur_licht_variant = 0;
byte interieur_licht_variant = 3;

int32_t ext_x = 25;
int32_t ext_y = 45;