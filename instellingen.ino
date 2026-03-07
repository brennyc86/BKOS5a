void instellingen(String actie) {
  if (actief_instelling == 0) {
    if (actie == "print") {
      instellingen("naam");
      tft.print(item_naam);
    } else if (actie == "icon_klein") {
      int x = getCursorX() + 20;
      int y = getCursorY() + 25;
      setCursor(getCursorX(), getCursorY()+4);
      drawIcon10x10(x, y, icon_instellingen, kleur_donker, 3);
      tft.setTextColor(kleur_rood);
    } else {
      instellingen_bibliotheek[actief_instelling](actie);
    } 
  } else {
    if (actie == "bouw") {
      // [BKOS5a] spellen verwijderd - directe toewijzing
      homeknop_actief = true;
      homeknop_half_actief = false;
      slepen_toestaan = false;
    }
    instellingen_bibliotheek[actief_instelling](actie);
  }
}




void instellingen_menu(String actie){
  if (actie == "bouw") {
    bouw_instellingen();
  // } else if (actie == "run") {
  //   run_instellingen();
  } else if (actie == "naam") {
    item_naam = "Instellingen";
  } else if (actie == "run") {
    if (actieve_touch) {

      int druk = klik_instelling(ts_x, ts_y);

      if ((druk > -1) & (druk < app_cnt)) {
        actief_instelling = druk + 1;
        scherm_bouwen = true;
      }
    }
  }
}


void bouw_instellingen() {

  bouw_instellingen(false);

}

void bouw_instellingen(bool overlay) {

  delete[]knoppen_teken_positie;
  delete[]knoppen_positie;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst;
  delete[]knoppen_tekst_kleur;
  
  header_plaatsen("Instellingen");
  if (overlay) {
    overligger(kleur_zwart, instellingen_overlay_hoogte, instellingen_overlay_dicht_tot);
  } else {
    achtergrond(kleur_zwart, true);
  }


  alle_instelling_knoppen_plaatsen();
}

void insteling_knop_plaatsen(int model, int knop_nummer){
  if (model == 8) {
    fillRoundRect(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1], keuzescherm_knoppen_8[knop_nummer][2], keuzescherm_knoppen_8[knop_nummer][3], 5, kleur_licht);
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_donker);
    setCursor(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1]);
    if (knop_nummer <= 1) {
      instellingen_bibliotheek[knop_nummer+1]("icon_groot");
    } else {
      instellingen_bibliotheek[knop_nummer+1]("icon_klein");
    }
  } else if (model == 6) {
    fillRoundRect(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1], keuzescherm_knoppen_6[knop_nummer][2], keuzescherm_knoppen_6[knop_nummer][3], 5, kleur_licht);
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_donker);
    setCursor(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1]);
    if (knop_nummer <= 1) {
      instellingen_bibliotheek[knop_nummer+1]("icon_groot");
    } else {
      instellingen_bibliotheek[knop_nummer+1]("icon_middel");
    }
  } else if (model == 4) {
    fillRoundRect(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1], keuzescherm_knoppen_4[knop_nummer][2], keuzescherm_knoppen_4[knop_nummer][3], 5, kleur_licht);
    tft.setTextSize(1.5);
    tft.setTextColor(kleur_donker);
    setCursor(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1]);
    instellingen_bibliotheek[knop_nummer+1]("icon_groot");
  }
  instellingen_bibliotheek[knop_nummer+1]("print");
}


void alle_instelling_knoppen_plaatsen(){
  int model;
  if (instellingen_cnt <= 5) {
    model = 4;
  } else if (instellingen_cnt <= 7) {
    model = 6;
  } else {
    model = 8;
  }

  for (byte i = 0; i < instellingen_cnt-1; i++) {
    insteling_knop_plaatsen(model, i);
  }
}

int klik_instelling(int x, int y) {
  if (instellingen_cnt < 6) {
    for (byte i = 0; i < instellingen_cnt -1; i++) {
      if ((x >= keuzescherm_knoppen_4[i][0]) && (x <= keuzescherm_knoppen_4[i][0]+keuzescherm_knoppen_4[i][2]) && (y >= keuzescherm_knoppen_4[i][1]) && (y <= keuzescherm_knoppen_4[i][1]+keuzescherm_knoppen_4[i][3])) {
        return i;
      }
    }
  } else if (instellingen_cnt < 8) {
    for (byte i = 0; i < instellingen_cnt -1; i++) {
      if ((x >= keuzescherm_knoppen_6[i][0]) && (x <= keuzescherm_knoppen_6[i][0]+keuzescherm_knoppen_6[i][2]) && (y >= keuzescherm_knoppen_6[i][1]) && (y <= keuzescherm_knoppen_6[i][1]+keuzescherm_knoppen_6[i][3])) {
        return i;
      }
    }
  } else {
    for (byte i = 0; i < instellingen_cnt -1; i++) {
      if ((x >= keuzescherm_knoppen_8[i][0]) && (x <= keuzescherm_knoppen_8[i][0]+keuzescherm_knoppen_8[i][2]) && (y >= keuzescherm_knoppen_8[i][1]) && (y <= keuzescherm_knoppen_8[i][1]+keuzescherm_knoppen_8[i][3])) {
        return i;
      }
    }
  }
  return -1;
}





// void spel_knop_plaatsen(int model, int knop_nummer){
//   if (model == 8) {
//     fillRoundRect(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1], keuzescherm_knoppen_8[knop_nummer][2], keuzescherm_knoppen_8[knop_nummer][3], 5, kleur_licht);
//     tft.setTextSize(1.5);
//     tft.setTextColor(kleur_donker);
//     setCursor(keuzescherm_knoppen_8[knop_nummer][0], keuzescherm_knoppen_8[knop_nummer][1]);
//     if (knop_nummer <= 1) {
//       spellen_bibliotheek[knop_nummer+1]("icon_groot");
//     } else {
//       spellen_bibliotheek[knop_nummer+1]("icon_klein");
//     }
//   } else if (model == 6) {
//     fillRoundRect(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1], keuzescherm_knoppen_6[knop_nummer][2], keuzescherm_knoppen_6[knop_nummer][3], 5, kleur_licht);
//     tft.setTextSize(1.5);
//     tft.setTextColor(kleur_donker);
//     setCursor(keuzescherm_knoppen_6[knop_nummer][0], keuzescherm_knoppen_6[knop_nummer][1]);
//     if (knop_nummer <= 1) {
//       spellen_bibliotheek[knop_nummer+1]("icon_groot");
//     } else {
//       spellen_bibliotheek[knop_nummer+1]("icon_middel");
//     }
//   } else if (model == 4) {
//     fillRoundRect(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1], keuzescherm_knoppen_4[knop_nummer][2], keuzescherm_knoppen_4[knop_nummer][3], 5, kleur_licht);
//     tft.setTextSize(1.5);
//     tft.setTextColor(kleur_donker);
//     setCursor(keuzescherm_knoppen_4[knop_nummer][0], keuzescherm_knoppen_4[knop_nummer][1]);
//     spellen_bibliotheek[knop_nummer+1]("icon_groot");
//   }
//   spellen_bibliotheek[knop_nummer+1]("print");
// }


// void alle_spel_knoppen_plaatsen(){
//   int model;
//   if (spellen_cnt <= 5) {
//     model = 4;
//   } else if (spellen_cnt <= 7) {
//     model = 6;
//   } else {
//     model = 8;
//   }

//   for (byte i = 0; i < spellen_cnt-1; i++) {
//     spel_knop_plaatsen(model, i);
//   }
// }









// void instellingen(String actie) {
//   if (actie == "bouw") {
//     bouw_instellingen();
//   } else if (actie == "run") {
//     run_instellingen();
//   } else if (actie == "naam") {
//     item_naam = "Instellingen";
//   } else if (actie == "print") {
//     instellingen("naam");
//     tft.print(item_naam);
//   } else if (actie == "icon_klein") {
//     int x = getCursorX() + 20;
//     int y = getCursorY() + 25;
//     setCursor(getCursorX(), getCursorY()+4);
//     drawIcon10x10(x, y, icon_instellingen, kleur_donker, 3);
//     tft.setTextColor(kleur_rood);
//   }
// }




// void instellingen_restart() {
//   beeld_vraag("Helaas","De reset functie werkt nog niet", "ok");
//   scherm_bouwen=true;
// }

// void instellingen_sd_reset() {
  
//   byte druk;

//   if (false) { //(info_snaam.length() > 2) {
//     druk = beeld_vraag("SD kaart kopie", "Wilt u een kopie maken van de SD?", "JA", "NEE");
//     if (druk == 0){
//       beeld_melding("SD kopie maken", "Even geduld a.u.b.  . . .");

//       delay(1000);
//       // sd_maken();
//       delay(1000);
//     }
//   } else {
//     druk = beeld_vraag("schone SD", "Wilt u SD kaart inrichten?", "JA", "NEE");
//     if (druk == 0){
//       beeld_melding("schone SD", "Even geduld a.u.b.  . . .");

//       delay(1000);
//       // blanco_sd_maken();
//       delay(1000);
//     }
//   }

//   scherm_bouwen = true;

// }

// void instellingen_leven() {
//   // beeld_vraag("Volgende update:", tijd_tot_millis(timer_statusupdate), "ok");
//   scherm_bouwen = true;
// }

// void run_instellingen() {
//   int druk;
//   if (ts_touched()) {
//     int x = touch_x();
//     int y = touch_y();

//     druk = klik(x, y);

//     while (ts_touched()) {
//       ts.begin();
//     }

//     ts.begin();

//     if (druk > -1) {
//       instellingen_mean_knoppen_functies[druk]();
//     }

//     if (y > home_knop[1]) {
//       if (actieve_app == 9) {
//         actieve_app = 0;
//       }
//       scherm_bouwen = true;
//     }
//   }
//   ts.begin(); 
// }

// void loop_instellingen() {
  
//   while (! scherm_bouwen) {
//     if (scherm_actief) {
//       if ((millis() > h_klok_millis + 20000) || (millis() < h_klok_millis)) {
//         klok_update();
//       }
//     }

//     io();
//     ts.begin();
//     run_instellingen();
//   }
// }