void update_center(String actie) {
  if (actie == "bouw") {
    bouw_update();
  } else if (actie == "run") {
    run_update();
  } else if (actie == "naam") {
    item_naam = "Updates";
  } else if (actie == "print") {
    update_center("naam");
    tft.print(item_naam);
  } else if (actie == "icon_klein") {
    int x = getCursorX() + 20;
    int y = getCursorY() + 25;
    setCursor(getCursorX(), getCursorY()+4);
    drawIcon(x, y, 30, kleur_zwart, icon_30_S_update, sizeof(icon_30_S_update)/sizeof(byte));
    tft.setTextColor(kleur_rood);
  }
}


void bouw_update() {

  delete[]knoppen_teken_positie;
  delete[]knoppen_positie;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst;
  delete[]knoppen_tekst_kleur;
  
  aantal_knoppen = 2;
  knoppen_positie = new int*[aantal_knoppen];
  knoppen_teken_positie = new int*[aantal_knoppen];
  knoppen_tekst = new char*[aantal_knoppen];
  knoppen_basiskleur = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur = new uint16_t*[aantal_knoppen];
  knoppen_status = new byte[aantal_knoppen];

  BKOS_GIT_ALLOWED = fetchAlowedVersions();
  BKOS5_VERSIE_GIT = fetchVersion5();  // BKOS5a versie ophalen
  String deler = "\n";
  String sonderdeel;
  char conderdeel[10];
  int p;
  while (deler.indexOf(BKOS_GIT_ALLOWED) > 0) {
    p = deler.indexOf(BKOS_GIT_ALLOWED);
    sonderdeel = BKOS_GIT_ALLOWED.substring(0, p);
    sonderdeel.toCharArray(allowed_versions[aantal_knoppen - 2], 10);
    // allowed_versions[aantal_knoppen - 2] = conderdeel;
    BKOS_GIT_ALLOWED = BKOS_GIT_ALLOWED.substring(p+1);
    aantal_knoppen++;
  }
  sonderdeel = BKOS_GIT_ALLOWED;
  sonderdeel.toCharArray(allowed_versions[aantal_knoppen - 2], 10);
  // allowed_versions[aantal_knoppen-2] = conderdeel;
  aantal_knoppen++;

  for (int i  = 0 ; i < aantal_knoppen ; i++) {
    knoppen_positie[i] = update_knoppen_positie[i];
    knoppen_teken_positie[i] = update_knoppen_positie[i];
    // knoppen_tekst[i] = io_namen[io_knoppen[i]];
    if (i == 0) {
      knoppen_tekst[i] = "BKOS 4";      
    } else if (i == 1) {
      knoppen_tekst[i] = "BKOS 5";      
    } else {
      knoppen_tekst[i] = allowed_versions[i - 2];
    }
    knoppen_status[i] = 0;
    knoppen_basiskleur[i] = schakelscherm_knoppen_kleur;
    knoppen_tekst_kleur[i] = schakelscherm_knoppen_tekst_kleur;
    
  }

  header_plaatsen("Instellingen");
  achtergrond(kleur_donker, true);
  
  tft.setTextSize(3);
  tft.setTextColor(kleur_licht);
  setCursor(0, 40);

  tft.print("  Actuele versie: ");
  tft.println(BKOS_VERSIE);
  tft.setTextSize(2);
  tft.println("   In de testperiode wordt BKos automatisch geupdate");
  tft.println("   U kunt wel kiezen tussen versie 4 en 5");
  tft.println("");
  tft.println("   Versie 4 betreft de handmatig gecodeerde versie");
  tft.println("   Versie 5 wordt de AI gecodeerde versie");
  tft.println("");
  tft.println("   Klik om naar de gewenste versie te gaan:");

  alle_knoppen_plaatsen();

  tft.println("");
  tft.println("   Allowed versions:");
  tft.println(BKOS_GIT_ALLOWED);
  tft.print("   ");
  tft.print("'");
  tft.print(aantal_knoppen);
  tft.print("' knoppen gevonden");

  // // bouw_io_instellingen(0);
  // header_plaatsen("Instellingen");
  // achtergrond(tft.color565(0, 0, 0), false);
  // center_tekst(120, 100, "Nog niet gemaakt", 2, tft.color565(255, 255, 255));
  // center_tekst(120, 150, "Tik om terug te gaan", 1, tft.color565(255, 255, 255));
}

void run_update() {
  int druk;
  if (actieve_touch) {
    int x = touch_x();
    int y = touch_y();

    druk = klik(ts_x, ts_y);

    if (druk == 0) {
      // Installeren BKOS 4
      // Eerst de scherm opmaak even doen en daarna de installatie starten voor BKOS 4
      updaten = true;
      delay(250);
      tft.fillScreen(kleur_zwart);
      bkos_logo(10, 10, kleur_donker);
      tft.setCursor(300, 10);
      tft.setTextSize(3);
      tft.setTextColor(kleur_groen);
      tft.println("BKOS update");
      tft.setTextColor(kleur_beige);
      tft.setCursor(300, 45);
      tft.print(BKOS_VERSIE);
      tft.print(" > ");
      tft.println(BKOS_VERSIE_GIT);
      downloadAndApplyFirmware(4);
    } else if (druk == 1) {
      // Installeren BKOS 5a
      updaten = true;
      delay(250);
      tft.fillScreen(kleur_zwart);
      bkos_logo(10, 10, kleur_donker);
      tft.setCursor(300, 10);
      tft.setTextSize(3);
      tft.setTextColor(kleur_groen);
      tft.println("BKOS update");
      tft.setTextColor(kleur_beige);
      tft.setCursor(300, 45);
      tft.print(BKOS_VERSIE);
      tft.print(" > ");
      tft.println(BKOS5_VERSIE_GIT.length() > 0 ? BKOS5_VERSIE_GIT : "5a");
      downloadAndApplyFirmware(5);
    }

    if (ts_y >= home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (ts_y < 30) {
      klik_header(ts_x, ts_y);
    }
  }
}