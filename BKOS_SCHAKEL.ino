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