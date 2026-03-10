void exterieur(String actie) {
  if (actie == "bouw") {
    bouw_exterieur();
  } else if (actie == "run") {
    run_exterieur();
  } else if (actie == "naam") {
    item_naam = ("Exterieur");
  } else if (actie == "print") {
    exterieur("naam");
    tft.print(item_naam);
  } else if (actie == "icon_groot") {
    int x = getCursorX();
    int y = getCursorY();

    // Achtergrond: lucht boven, water onder
    uint16_t kl_lucht  = tft.color565(100, 160, 210);
    uint16_t kl_water  = tft.color565(20,  65,  120);
    uint16_t kl_romp   = tft.color565(30,  80,  40);
    uint16_t kl_opbouw = tft.color565(235, 235, 235);
    uint16_t kl_zeil   = tft.color565(155, 60,  35);
    uint16_t kl_mast   = tft.color565(190, 190, 190);

    int bx = x + 5;   // boot-tekening start x
    int by = y + 5;   // boot-tekening start y
    int wl = by + 62; // waterlijny in pixels

    // Lucht
    fillRect(x+3, y+3, 98, 95, kl_lucht);
    // Water
    fillRect(x+3, wl, 98, y+95-wl+3, kl_water);
    // Waterlijn
    drawLine(x+3, wl, x+100, wl, tft.color565(160, 210, 240));
    drawLine(x+3, wl+1, x+100, wl+1, tft.color565(100, 170, 220));

    // --- Mini boot (zijkant, schaalsysteem ~1:2.2 van origineel) ---
    // Romp (donkergroen): van bx tot bx+55, onderkant op wl
    // Origineel: {0,150} tot {120,140}, romp y=150-165
    // Schaal: x/2.2, y gebaseerd op wl
    // romp = wl-6 (waterlijny - iets erboven voor diepgang)
    int ry = wl - 3; // romp bovenkant
    // Romp als gevulde vorm
    for (int dy = 0; dy <= 4; dy++) {
      drawLine(bx+2, ry+dy, bx+46, ry+dy, kl_romp);
    }
    // Boeg (voorkant, links): punt naar links-boven
    drawLine(bx+2, ry, bx, ry+2, kl_romp);
    // Spiegel (achterkant, rechts): lichte hoek
    drawLine(bx+46, ry, bx+54, ry+4, kl_romp);
    for (int dy = 0; dy <= 4; dy++) {
      tft.drawPixel(bx+47+dy, ry+dy, kl_romp);
    }

    // Opbouw (wit): rechthoek op romp
    fillRect(bx+4, ry-12, 30, 12, kl_opbouw);
    // Kajuit ronde bovenkant
    drawLine(bx+4, ry-12, bx+34, ry-12, kl_mast);

    // Mast (grijs): verticaal op x=bx+28
    int mx = bx + 28; // mastx
    drawLine(mx, by, mx, ry-12, kl_mast);
    drawLine(mx+1, by, mx+1, ry-12, kl_mast);

    // Verstaging: lijn van mast-top naar boeg en naar hek
    drawLine(mx, by+1, bx, ry-1, kl_mast);
    drawLine(mx, by+1, bx+54, ry+1, kl_mast);

    // Grootzeil (roodbruin): driehoek langs mast
    for (int sy = 0; sy < (ry-12-by); sy++) {
      int sw = (sy * 14) / (ry-12-by);
      tft.drawPixel(mx - sw - 1, by + sy, kl_zeil);
    }

    // Fok/genua (roodbruin): driehoek voor de mast
    for (int sy = 0; sy < 20; sy++) {
      int sx = (sy * 20) / 20;
      tft.drawPixel(mx + sy/2, ry-12 - sy, kl_zeil);
    }

    // --- Knoppen status rechtsonder (4 dots) ---
    // Kleine bolletjes die de status van de 4 hoofd-knoppen tonen
    // haven=0, zeilen=1, motor=2, anker=3
    int dot_x = x + 72;
    int dot_y = y + 38;
    const char* labels[] = {"H","Z","M","A"};
    for (int d = 0; d < 4; d++) {
      uint16_t dot_kleur = tft.color565(60, 60, 80);
      if (exterieurscherm_status[d] == 1) {
        dot_kleur = tft.color565(255, 220, 50);
      }
      fillCircle(dot_x + (d%2)*14, dot_y + (d/2)*16, 5, dot_kleur);
      tft.setTextColor(tft.color565(200,200,200));
      tft.setTextSize(1);
      setCursor(dot_x + (d%2)*14 - 3, dot_y + (d/2)*16 - 4);
      tft.print(labels[d]);
    }

    tft.setTextColor(kleur_donker);
    tft.setTextSize(1.5);
    setCursor(x+5, y+105);
  }
}

void bouw_exterieur() {

  // Kleuren zee/lucht sfeer
  uint16_t kl_lucht       = tft.color565(120, 180, 220);  // lichtblauw
  uint16_t kl_water       = tft.color565(18,  60,  115);  // diep zeeblauw
  uint16_t kl_golf_crest  = tft.color565(160, 215, 245);  // schuimkop
  uint16_t kl_golf_dal    = tft.color565(15,  55,  105);  // golfbal
  uint16_t kl_header      = tft.color565(10,  25,  65);   // donker header

  // Waterlijn in schermcoordinaten
  int wl_y = scherm_y(ext_y + 165); // = scherm_y(210) ≈ 315px

  // --- LUCHT (boven waterlijn) ---
  tft.fillRect(0, 0, tft.width(), wl_y, kl_lucht);

  // Lichte verloop lucht: iets donkerder aan horizon
  for (int i = 0; i < 30; i++) {
    uint8_t fade = (uint8_t)(i * 2);
    uint16_t kl_fade = tft.color565(120-fade/4, 180-fade/4, 220-fade/8);
    tft.drawFastHLine(0, wl_y - 30 + i, tft.width(), kl_fade);
  }

  // --- WATER (onder waterlijn) ---
  tft.fillRect(0, wl_y, tft.width(), tft.height()-wl_y, kl_water);

  // Golvende waterlijn (sinus)
  for (int gx = 0; gx < tft.width(); gx++) {
    int gy1 = wl_y + (int)(4.0 * sin(gx * 0.04));
    int gy2 = wl_y + (int)(3.0 * sin(gx * 0.04 + 1.2)) + 5;
    int gy3 = wl_y + (int)(2.5 * sin(gx * 0.06 + 0.5)) + 10;
    tft.drawPixel(gx, gy1,   kl_golf_crest);
    tft.drawPixel(gx, gy1+1, tft.color565(100, 170, 215));
    tft.drawPixel(gx, gy2,   tft.color565(60, 130, 185));
    tft.drawPixel(gx, gy3,   tft.color565(40, 100, 160));
  }

  // Kleine schuimvlekjes op de golven
  for (int gx = 20; gx < tft.width()-10; gx += 45) {
    int gy = wl_y + (int)(4.0 * sin(gx * 0.04)) - 1;
    tft.fillRect(gx, gy, 8, 2, tft.color565(220, 240, 255));
  }

  // --- HEADER BALK ---
  kleur_home_knop = tft.color565(255, 255, 255);
  kleur_home_tekst = kl_header;

  // Header achtergrond
  tft.fillRect(0, 0, tft.width(), scherm_y(22), kl_header);
  tft.setTextColor(tft.color565(120, 190, 255));
  tft.setTextSize(2);
  setCursor(12, 6);
  tft.print("EXTERIEUR VERLICHTING");
  tft.setTextSize(1.5);
  tft.setTextColor(kleur_donker);

  // --- KNOPPEN SETUP ---
  aantal_knoppen = exterieur_knoppen_cnt;

  delete[]knoppen_positie;
  delete[]knoppen_teken_positie;
  delete[]knoppen_tekst;
  delete[]knoppen_basiskleur;
  delete[]knoppen_tekst_kleur;
  delete[]knoppen_status;

  knoppen_positie      = new int*[aantal_knoppen];
  knoppen_teken_positie= new int*[aantal_knoppen];
  knoppen_tekst        = new char*[aantal_knoppen];
  knoppen_basiskleur   = new uint16_t*[aantal_knoppen];
  knoppen_tekst_kleur  = new uint16_t*[aantal_knoppen];
  knoppen_status       = new byte[aantal_knoppen];

  for (int i = 0; i < aantal_knoppen; i++) {
    knoppen_positie[i]       = exterieurscherm_knoppen_positie[i];
    knoppen_teken_positie[i] = exterieurscherm_knoppen_positie[i];
    knoppen_tekst[i]         = exterieur_knoppen_namen[i];
    knoppen_status[i]        = exterieurscherm_status[i];
    knoppen_basiskleur[i]    = exterieruscherm_knoppen_kleur;
    knoppen_tekst_kleur[i]   = schakelscherm_knoppen_tekst_kleur;
  }

  alle_knoppen_plaatsen();

  // --- BOOT TEKENEN ---
  exterieur_teken_boot(ext_x, ext_y);
  exterieur_symbolen_verlichting(ext_x, ext_y);
  interieur_verlichting();
}

void exterieur_teken_boot(int32_t x, int32_t y) {
  // Kleuren
  uint16_t kl_romp   = tft.color565(30,  80,  40);   // donkergroen romp
  uint16_t kl_opbouw = tft.color565(235, 235, 235);  // wit opbouw
  uint16_t kl_zeil   = tft.color565(155, 60,  35);   // roodbruin zeilen
  uint16_t kl_mast   = tft.color565(185, 185, 185);  // aluminium grijs mast
  uint16_t kl_ramen  = tft.color565(140, 195, 235);  // lichtblauw ramen

  // Teken onderdelen in volgorde (achterste eerst)
  tekenItem(x, y, kl_zeil,   teken_boot_zeilen,  sizeof(teken_boot_zeilen)/sizeof(int)/2);
  tekenItem(x, y, kl_mast,   teken_boot_mast,    sizeof(teken_boot_mast)/sizeof(int)/2);
  tekenItem(x, y, kl_romp,   teken_boot_romp,    sizeof(teken_boot_romp)/sizeof(int)/2);
  tekenItem(x, y, kl_opbouw, teken_boot_opbouw,  sizeof(teken_boot_opbouw)/sizeof(int)/2);
  tekenItem(x, y, kl_ramen,  teken_boot_ramen,   sizeof(teken_boot_ramen)/sizeof(int)/2);
  tekenCircels(x, y, kl_romp, circels_boot, sizeof(circels_boot)/sizeof(int)/3);
  tekenTeksten(x, y, tft.color565(200,200,200), teken_tekst_positie_boot, teken_tekst_boot, sizeof(teken_tekst_positie_boot)/sizeof(int)/3);
}

void exterieur_symbolen_verlichting(int32_t x, int32_t y){

  int32_t ix;
  int32_t iy;
  bool gevonden;
  // hek
  ix = 0;
  iy = 150;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'h') && (io_namen[io_knoppen[i]][5] == 'e') && (io_namen[io_knoppen[i]][6] == 'k')) {
      if (io_input[io_knoppen[i]] == 1) {
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
        } else {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
      } else {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
      }
    }
  }

  // top
  ix = 67;
  iy = 0;
  gevonden = false;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == '3') && (io_namen[io_knoppen[i]][5] == 'k') && (io_namen[io_knoppen[i]][6] == 'l')) {
      if (io_input[io_knoppen[i]] == 1) {
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
          gevonden = true;
        } else {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_hek, sizeof(icon_30_HL_hek)/sizeof(byte));
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
        gevonden = true;
      }
    }
  }
  for (int i = 0; i < io_knoppen_cnt; i++){
    if (!gevonden) {
      if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'a') && (io_namen[io_knoppen[i]][5] == 'n') && (io_namen[io_knoppen[i]][6] == 'k') && (io_namen[io_knoppen[i]][7] == 'e') && (io_namen[io_knoppen[i]][8] == 'r')){
        if (io_input[io_knoppen[i]] == 1){
          if (io_output[io_knoppen[i]] == 1) {
            drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
          } else {
            drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
          }
        } else if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
        } else {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_rond, sizeof(icon_30_HL_rond)/sizeof(byte));
        }
      }
    }
  }

  // stoom
  ix = 75;
  iy = 75;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 's') && (io_namen[io_knoppen[i]][5] == 't') && (io_namen[io_knoppen[i]][6] == 'o') && (io_namen[io_knoppen[i]][7] == 'o') && (io_namen[io_knoppen[i]][8] == 'm')) {
      if (io_input[io_knoppen[i]] == 1){
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_wit, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
        } else {
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_grijs, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
      } else {
      drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_stoom, sizeof(icon_30_HL_stoom)/sizeof(byte));
      }
    }
  }

  // boordlichten
  ix = 80;
  iy = 143;
  for (int i = 0; i < io_knoppen_cnt; i++){
    if ((io_namen[io_knoppen[i]][0] == '*') && (io_namen[io_knoppen[i]][1] == '*') && (io_namen[io_knoppen[i]][2] == 'L') && (io_namen[io_knoppen[i]][3] == '_') && (io_namen[io_knoppen[i]][4] == 'n') && (io_namen[io_knoppen[i]][5] == 'a') && (io_namen[io_knoppen[i]][6] == 'v') && (io_namen[io_knoppen[i]][7] == 'i')) {
      if (io_input[io_knoppen[i]] == 1){
        if (io_output[io_knoppen[i]] == 1) {
          drawIcon(x+ix - 15, y+iy-23 , 30, kleur_actief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_actief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
        } else {
          drawIcon(x+ix - 15, y+iy-23 , 30, kleur_oranje, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
          drawIcon(x+ix - 15, y+iy-15 , 30, kleur_oranje, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
        }
      } else if (io_output[io_knoppen[i]] == 1) {
        drawIcon(x+ix - 15, y+iy-23 , 30, kleur_passief_rood, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
        drawIcon(x+ix - 15, y+iy-15 , 30, kleur_passief_groen, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
      } else {
      drawIcon(x+ix - 15, y+iy-23 , 30, kleur_zwart, icon_30_HL_bb, sizeof(icon_30_HL_bb)/sizeof(byte));
      drawIcon(x+ix - 15, y+iy-15 , 30, kleur_zwart, icon_30_HL_sb, sizeof(icon_30_HL_sb)/sizeof(byte));
      }
    }
  }
}

void run_exterieur() {
  int druk;
  if (actieve_touch) {
    druk = klik(ts_x, ts_y);

    if (druk > -1){
      exterieurscherm_schakel(druk);
      knop_plaatsen(druk);
      io_wijziging = true;
    }

    while (ts_touched()) {
      ts_begin();
    }

    if (ts_y >= home_knop[1]) {
      actieve_app = 0;
      scherm_bouwen = true;
    } else if (ts_y < 30) {
      klik_header(ts_x, ts_y);
    }
  }
  if (io_runned) {
    io_runned = false;
    exterieur_symbolen_verlichting(ext_x, ext_y);
  }
  
  ts_begin(); 
}

void exterieurscherm_schakel(int knop) {
  if (knop > 3) {
    if (charstrip(exterieur_knoppen_namen[knop]) == "**I_licht") {
      interieur_verlichting(true);
      exterieurscherm_status[knop] = interieur_licht_variant;
      knoppen_status[knop] = interieur_licht_variant;
      // knop_plaatsen(knop);
    } else {
      if (knoppen_status[knop] == 0) {
        // uit wordt aan
        knoppen_status[knop] = 1;
        exterieurscherm_status[knop] = 1;
        for (int i = 0; i < io_knoppen_cnt; i++){
          int j = 0;
          bool fl = false;
          bool stp = false;
          while (!stp){
            if (io_namen[io_knoppen[i]][j] == knoppen_tekst[knop][j]) {
              stp = false;
            } else {
              fl = true;
              stp = true;
            }
            if (j >= 9){
              stp = true;
            }
            j++;
          }
          if (!fl) {
            if ((io_output[io_knoppen[i]] == 0) || (io_output[io_knoppen[i]] == 1)){
              io_output[io_knoppen[i]] = 1;
            } else if ((io_output[io_knoppen[i]] == 2) || (io_output[io_knoppen[i]] == 3)){
              io_output[io_knoppen[i]] = 3;
            }
          }
        }
      } else{
        // aan wordt uit
        knoppen_status[knop] = 0;
        exterieurscherm_status[knop] = 0;
        for (int i = 0; i < io_knoppen_cnt; i++){
          int j = 0;
          bool fl = false;
          bool stp = false;
          while (!stp){
            if (io_namen[io_knoppen[i]][j] == knoppen_tekst[knop][j]) {
              stp = false;
            } else {
              fl = true;
              stp = true;
            }
            if (j >= 9){
              stp = true;
            }
            j++;
          }
          if (!fl) {
            if ((io_output[io_knoppen[i]] == 0) || (io_output[io_knoppen[i]] == 1)){
              io_output[io_knoppen[i]] = 0;
            } else if ((io_output[io_knoppen[i]] == 2) || (io_output[io_knoppen[i]] == 3)){
              io_output[io_knoppen[i]] = 2;
            }
          }
        }
      }
    }
    // knop_plaatsen(knop);
  } else {
    for (int i = 0; i < 4; i++) {
      if (i == knop) {
        if (knoppen_status[i] == 1) {
          exterieur_licht_variant++;
        } else {
          knoppen_status[i] = 1;
          exterieurscherm_status[i] = 1;
          exterieur_licht_variant = 0;
        }
      } else {
        knoppen_status[i] = 0;
        exterieurscherm_status[i] = 0;
      }
      knop_plaatsen(i);
    }
    if (knop == 0) {
      exterieur_verlichting_uit();
      interieur_verlichting();
    } else if (knop == 1) {
      exterieur_zeilverlichting();
      interieur_verlichting();
    } else if (knop == 2) {
      exterieur_motorverlichting();
      interieur_verlichting();
    } else if (knop == 3) {
      exterieur_ankerverlichting();
      interieur_verlichting();
    }
    
  }
  knop_plaatsen(knop);
  exterieur_symbolen_verlichting(ext_x, ext_y);
  io_now = true;
  // io();
  // exterieur_symbolen_verlichting(ext_x, ext_y);
  // delay(50);
  // io();
  // exterieur_symbolen_verlichting(ext_x, ext_y);
}

void exterieur_verlichting_uit(){
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
        io_output[io_knoppen[knop]] = 0;
      } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
        io_output[io_knoppen[knop]] = 2;
      }
    }
  }
}

void exterieur_zeilverlichting(){
  bool aan_uit;
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      aan_uit = false;
      if (exterieur_licht_variant > 1) {
        exterieur_licht_variant = 0;
      }
      if (exterieur_licht_variant == 0) {
        // Enkel de 3 kleurenlamp aan ( **L_3kl)
        if ((io_namen[io_knoppen[knop]][4] == '3') && (io_namen[io_knoppen[knop]][5] == 'k') && (io_namen[io_knoppen[knop]][6] == 'l')){
          aan_uit = true;
        }
      } else if (exterieur_licht_variant == 1) {
        // navigatie verlichting en heklicht aan (**L_navi  & **L_hek)
        if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
          aan_uit = true;
        }
      } 
      
      if (aan_uit) {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 1;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 3;
        }
      } else {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 0;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 2;
        }
      }
    }
  }
}

void exterieur_motorverlichting(){
  bool aan_uit;
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      aan_uit = false;
      if (exterieur_licht_variant > 2) {
        exterieur_licht_variant = 0;
      }
      if (exterieur_licht_variant == 0) {
        // ankerlicht en navigatie verlichting (**L_anker en **L_navi)
        if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'a') && (io_namen[io_knoppen[knop]][5] == 'n') && (io_namen[io_knoppen[knop]][6] == 'k') && (io_namen[io_knoppen[knop]][7] == 'e') && (io_namen[io_knoppen[knop]][8] == 'r')){
          aan_uit = true;
        }
      } else if (exterieur_licht_variant == 1) {
        // navigatie verlichting, stoomlicht en heklicht aan (**L_navi, **L_stoom  & **L_hek)
        if ((io_namen[io_knoppen[knop]][4] == 'n') && (io_namen[io_knoppen[knop]][5] == 'a') && (io_namen[io_knoppen[knop]][6] == 'v') && (io_namen[io_knoppen[knop]][7] == 'i')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
          aan_uit = true;
        }
      }  else if (exterieur_licht_variant == 2) {
        // 3 kleurenlicht en stoomlicht aan (**L_3kl & **L_stoom)
        if ((io_namen[io_knoppen[knop]][4] == '3') && (io_namen[io_knoppen[knop]][5] == 'k') && (io_namen[io_knoppen[knop]][6] == 'l')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
          aan_uit = true;
        }
      } 
      
      if (aan_uit) {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 1;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 3;
        }
      } else {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 0;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 2;
        }
      }
    }
  }
}

void exterieur_ankerverlichting(){
  bool aan_uit;
  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if ((io_namen[io_knoppen[knop]][0] == '*') && (io_namen[io_knoppen[knop]][1] == '*') && (io_namen[io_knoppen[knop]][2] == 'L') && (io_namen[io_knoppen[knop]][3] == '_')){
      aan_uit = false;
      if (exterieur_licht_variant > 1) {
        exterieur_licht_variant = 0;
      }
      if (exterieur_licht_variant == 0) {
        // Enkel het ankerlicht aan ( **L_anker)
        if ((io_namen[io_knoppen[knop]][4] == 'a') && (io_namen[io_knoppen[knop]][5] == 'n') && (io_namen[io_knoppen[knop]][6] == 'k') && (io_namen[io_knoppen[knop]][7] == 'e') && (io_namen[io_knoppen[knop]][8] == 'r')){
          aan_uit = true;
        }
      } else if (exterieur_licht_variant == 1) {
        // stoomlicht en heklicht aan (**L_stoom  & **L_hek)
        if ((io_namen[io_knoppen[knop]][4] == 's') && (io_namen[io_knoppen[knop]][5] == 't') && (io_namen[io_knoppen[knop]][6] == 'o') && (io_namen[io_knoppen[knop]][7] == 'o') && (io_namen[io_knoppen[knop]][8] == 'm')){
          aan_uit = true;
        } else if ((io_namen[io_knoppen[knop]][4] == 'h') && (io_namen[io_knoppen[knop]][5] == 'e') && (io_namen[io_knoppen[knop]][6] == 'k')){
          aan_uit = true;
        }
      }   
      
      if (aan_uit) {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 1;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 3;
        }
      } else {
        if ((io_output[io_knoppen[knop]] == 0) || (io_output[io_knoppen[knop]] == 1)){
          io_output[io_knoppen[knop]] = 0;
        } else if ((io_output[io_knoppen[knop]] == 2) || (io_output[io_knoppen[knop]] == 3)){
          io_output[io_knoppen[knop]] = 2;
        }
      }
    }
  }
}

void interieur_verlichting() {
  interieur_verlichting(false);
}

void interieur_verlichting(bool schakelen) {
  if (schakelen) {
    interieur_licht_variant ++;
  }
  if (interieur_licht_variant >= 4) {
    interieur_licht_variant = 0;
  }
  // tft.fillRect(10, 30, 100, 30, kleur_zwart);
  // tft.setTextColor(kleur_wit);
  // tft.setCursor(10, 31);
  // tft.print(interieur_licht_variant);

  int wit = 0;
  int rood = 0;

  if (interieur_licht_variant == 0) {
    wit = 0;
    rood = 0;
  } else if (interieur_licht_variant == 1) {
    wit = 1;
    rood = 0;
  } else if (interieur_licht_variant == 2) {
    wit = 0;
    rood = 1;
  } else if (interieur_licht_variant == 3) {
    for (int i = 0; i < sizeof(exterieurscherm_status); i++) {
      if (exterieurscherm_status[i] == 1) {
        if (charstrip(exterieur_knoppen_namen[i]) == "**haven") {
          // tft.print("haven");
          wit = 1;
          rood = 0;
        } else if (charstrip(exterieur_knoppen_namen[i]) == "**zeilen") {
          // tft.print("zeilen");
          wit = 0;
          rood = 1;
        } else if (charstrip(exterieur_knoppen_namen[i]) == "**motor") {
          // tft.print("motor");
          wit = 0;
          rood = 1;
        } else if (charstrip(exterieur_knoppen_namen[i]) == "**anker") {
          // tft.print("anker");
          wit = 1;
          rood = 0;
        // } else {
        //   tft.print(i);
        //   tft.print("[");
        //   tft.print(charstrip(exterieur_knoppen_namen[i]));
        //   tft.print("]");
        }
      }
    }
  }

  for (int knop = 0; knop < io_knoppen_cnt; knop++){
    if (io_namen[io_knoppen[knop]] == "**IL_wit "){
      io_output[io_knoppen[knop]] = wit;
    } else if (io_namen[io_knoppen[knop]] == "**IL_rood"){
      io_output[io_knoppen[knop]] = rood;
    }
  }
}


String charstrip(char invoer[]) {
  String uitvoer = "";
  for (int i = 0; i < 10; i++){
    if (invoer[i] != ' '){
      uitvoer += invoer[i];
    }
  }
  uitvoer.trim();
  return uitvoer;
}