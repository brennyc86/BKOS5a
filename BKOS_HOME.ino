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