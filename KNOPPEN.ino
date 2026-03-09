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
