
// BKOS5a: Rode rand waarschuwing 10s voor scherm-timeout
void scherm_timeout_waarschuwing() {
  // Teken rode rand rondom het scherm als waarschuwing
  int dikte = 4;
  for (int i = 0; i < dikte; i++) {
    tft.drawRect(i, i, tft.width() - 2*i, tft.height() - 2*i, tft.color565(200, 0, 0));
  }
}
long uit_check = 0;

void beeld_melding(String hoofdmelding) {
  beeld_melding(hoofdmelding, "", "");
}

void beeld_melding(String hoofdmelding, String submelding) {
  beeld_melding(hoofdmelding, submelding, "");
}

void beeld_melding(String hoofdmelding, String submelding, String titel){
  beeld_melding(hoofdmelding, submelding, titel, {}, false);
}

void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10]){
  beeld_melding(hoofdmelding, submelding, titel, sec_icon, false);
}

void beeld_melding(String hoofdmelding, String submelding, String titel, String sec_icon[10], bool sec_icon_bool) {
  if (waas != true) {
    gewaasd_scherm();
    waas = true;
  }
  fillRoundRect(15, 100, 210, 30, 10, kleur_donker);
  fillRect(15, 120, 10, 10, kleur_licht);
  fillRect(215, 120, 10, 10, kleur_licht);
  fillRoundRect(15, 120, 210, 80, 10, kleur_licht);
  drawRoundRect(15, 100, 210, 100, 10, kleur_zwart);
  drawIconWifi10x10(205, 105, kleur_licht, false);
  drawIcon10x10(190, 105, icon_sd, kleur_licht, sd_connect);
  if (sec_icon_bool) {
    drawIcon10x10(175, 105, sec_icon, kleur_licht);
  }
  center_tekst(120, 106, titel, 1, kleur_licht);
  if (submelding == "") {
    center_tekst(120, 150, hoofdmelding, 2, kleur_donker);
  } else {
    center_tekst(120, 140, hoofdmelding, 2, kleur_donker);
    center_tekst(120, 170, submelding, 1, kleur_zwart);
  }
  tft.setTextSize(1.5);
  tft.setTextColor(kleur_zwart);
  scherm_bouwen = true;
}

void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur) {
  tft.fillRoundRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), scherm_y(d), kleur);
}
void fillRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal) {
  tft.fillRoundRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), scherm_y(d, schaal), kleur);
}
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur) {
  tft.drawRoundRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), scherm_y(d), kleur);
}
void drawRoundRect(int x, int y, int w, int h, int d, uint16_t kleur, uint16_t schaal) {
  tft.drawRoundRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), scherm_y(d, schaal), kleur);
}
void fillRect(int x, int y, int w, int h, uint16_t kleur) {
  tft.fillRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), kleur);
}
void fillRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal) {
  tft.fillRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), kleur);
}
void drawRect(int x, int y, int w, int h, uint16_t kleur) {
  tft.drawRect(scherm_x(x), scherm_y(y), scherm_x(w), scherm_y(h), kleur);
}
void drawRect(int x, int y, int w, int h, uint16_t kleur, uint16_t schaal) {
  tft.drawRect(scherm_x(x, schaal), scherm_y(y, schaal), scherm_x(w, schaal), scherm_y(h, schaal), kleur);
}
void setCursor(int x, int y) {
  tft.setCursor(scherm_x(x), scherm_y(y));
}
void setCursor(int x, int y, uint16_t schaal) {
  tft.setCursor(scherm_x(x, schaal), scherm_y(y, schaal));
}
void fillCircle(int x, int y, int r, uint16_t kleur) {
  tft.fillCircle(scherm_x(x), scherm_y(y), scherm_y(r), kleur);
}
void fillCircle(int x, int y, int r, uint16_t kleur, uint16_t schaal) {
  tft.fillCircle(scherm_x(x, schaal), scherm_y(y, schaal), scherm_y(r, schaal), kleur);
}
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur) {
  tft.drawLine(scherm_x(x1), scherm_y(y1), scherm_x(x2), scherm_y(y2), kleur);
}
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t kleur, uint16_t schaal) {
  tft.drawLine(scherm_x(x1, schaal), scherm_y(y1, schaal), scherm_x(x2, schaal), scherm_y(y2, schaal), kleur);
}

// boolean string_in_array(String element, String array[], int lengte)
// byte beeld_vraag(String hoofdmelding, String submelding, String array[], byte opties_cnt)
byte beeld_vraag(String hoofdmelding, String submelding, String optie1) {
  return beeld_vraag(hoofdmelding, submelding, 1, optie1, "", "", "", "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2) {
  return beeld_vraag(hoofdmelding, submelding, 2, optie1, optie2, "", "", "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3) {
  return beeld_vraag(hoofdmelding, submelding, 3, optie1, optie2, optie3, "", "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4) {
  return beeld_vraag(hoofdmelding, submelding, 4, optie1, optie2, optie3, optie4, "");
}
byte beeld_vraag(String hoofdmelding, String submelding, String optie1, String optie2, String optie3, String optie4, String optie5) {
  return beeld_vraag(hoofdmelding, submelding, 5, optie1, optie2, optie3, optie4, optie5);
}
byte beeld_vraag(String hoofdmelding, String submelding, byte opties_cnt, String optie1, String optie2, String optie3, String optie4, String optie5){
  String array[5] = {optie1, optie2, optie3, optie4, optie5};
  return beeld_vraag(hoofdmelding, submelding, array, opties_cnt);
}

byte beeld_vraag(String hoofdmelding, String submelding, String array[], byte opties_cnt) {
  
  if (waas != true) {
    gewaasd_scherm();
    waas = true;
  }
  fillRoundRect(15, 80, 210, 30, 10, kleur_donker);
  fillRect(15, 100, 10, 10, kleur_licht);
  fillRect(215, 100, 10, 10, kleur_licht);
  fillRoundRect(15, 100, 210, 120, 10, kleur_licht);
  drawRoundRect(15, 80, 210, 140, 10, kleur_zwart);
  drawIconWifi10x10(205, 85, kleur_licht, false);
  if (submelding == "") {
    center_tekst(120, 125, hoofdmelding, 2, tft.color565(0, 100, 0));
  } else {
    center_tekst(120, 115, hoofdmelding, 2, tft.color565(0, 100, 0));
    center_tekst(120, 145, submelding, 1, tft.color565(0, 0, 0));
  }

  int B = 210 / opties_cnt;

  for (byte i = 0; i < opties_cnt; i++){
    fillRoundRect(i*B + 20, 170, B-10, 35, 5, kleur_donker);
    center_tekst(i*B + B/2 + 20, 180, array[i], 1.5, kleur_wit);
  }

  bool verder = false;

  while (!verder) {
    io();
    delay(50);
    ts_begin();
    if (ts_touched()) {
      int x = touch_x();
      int y = touch_y();

      while (ts_touched()) {
        ts_begin();
        delay(50);
      }
      
      for (byte i = 0; i < opties_cnt; i++) {
        if ((x > i*B + 15) & (x < (i+1)*B + 15)  &  (y > 165) & (y < 225)) {
          verder = true;
          while (ts_touched()){
            ts_begin();
            delay(50);
          }
          return i;
        }
      }

      while (ts_touched()) {
        ts_begin();
      }

    }
  }

  while (ts_touched()) {
    ts_begin();
    delay(50);
  }

  ts_begin();


  return -1;
}

void gewaasd_scherm() {
  byte factor = 2;
  for ( int i = 0; i < SY_MAX / factor; i++) {
    tft.drawLine(0, i*factor, SX_MAX, i*factor, tft.color565(0,0,0));
  }
}

void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte=1, uint16_t tekst_kleur=tft.color565(255,255,255)) {
  center_tekst(x, y, tekst, tekst_grootte, tekst_kleur, true);
}

// void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte=1, uint16_t tekst_kleur=tft.color565(255,255,255), bool schalen=true) {
  void center_tekst(int16_t x, int16_t y, String tekst, int tekst_grootte, uint16_t tekst_kleur, bool schalen) {

  x = scherm_x(x);
  y = scherm_y(y);
  
  int _tekst_grootte = 1;
  if (schalen) {
    for (int i = 1; i <= 20; i++){
      if ((scherm_x(tekst_grootte) >= i) && (scherm_y(tekst_grootte) >= i)) {
        _tekst_grootte = i;
      }
    }
  } else {
    _tekst_grootte = tekst_grootte;
  }


  tft.setTextSize(_tekst_grootte);
  tft.setTextColor(tekst_kleur);
  tft.setCursor((x - tekst.length()*_tekst_grootte*3), y);
  tft.print(tekst);

}

int16_t scherm_x(int16_t invoer) {
  return scherm_x(invoer, SCRIPT_RESOLUTIE);
}

int16_t scherm_x(int16_t invoer, int16_t code_resolutie) {
  return scherm_x(invoer, code_resolutie, RESOLUTIE);
  int16_t max_in;
  max_in = 240;
  return scherm_x(invoer, max_in);
}

int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie) {
  return scherm_x(invoer, code_resolutie, resolutie_x(scherm_resolutie, false), resolutie_x(scherm_resolutie, true));
}

int16_t scherm_x(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max) {
  return map(invoer, resolutie_x(code_resolutie, false), resolutie_x(code_resolutie, true), res_min, res_max);
}

int16_t resolutie_x(int16_t resolutie, bool eind) {
  if (eind) {
    if (resolutie == 2432) {
      return 240;
    } else if (resolutie == 3248) {
      return 320;
    } else if (resolutie == 4880) {
      if (ORIENTATIE == 1) {
        return 480;
      } else if (ORIENTATIE == 0) {
        return 800;
      }
    } else if (resolutie == 100) {
      return 100;
    }
  } else {
    return 0;
  }
  return -1;
}

int getCursorX() {
  return getCursorX(SCRIPT_RESOLUTIE);
}

int getCursorX(int16_t code_resolutie) {
  return getCursorX(code_resolutie, RESOLUTIE);
}

int getCursorX(int16_t code_resolutie, int16_t scherm_resolutie) {
  return map(tft.getCursorX(), resolutie_x(scherm_resolutie, false), resolutie_x(scherm_resolutie, true), resolutie_x(code_resolutie, false), resolutie_x(code_resolutie, true));
}


int16_t scherm_y(int16_t invoer) {
  return scherm_y(invoer, SCRIPT_RESOLUTIE);
}

int16_t scherm_y(int16_t invoer, int16_t code_resolutie) {
  return scherm_y(invoer, code_resolutie, RESOLUTIE);
  int16_t max_in;
  max_in = 240;
  return scherm_y(invoer, max_in);
}

int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t scherm_resolutie) {
  return scherm_y(invoer, code_resolutie, resolutie_y(scherm_resolutie, false), resolutie_y(scherm_resolutie, true));
}

int16_t scherm_y(int16_t invoer, int16_t code_resolutie, int16_t res_min, int16_t res_max) {
  return map(invoer, resolutie_y(code_resolutie, false), resolutie_y(code_resolutie, true), res_min, res_max);
}

int16_t resolutie_y(int16_t resolutie, bool eind) {
  if (eind) {
    if (resolutie == 2432) {
      return 320;
    } else if (resolutie == 3248) {
      return 480;
    } else if (resolutie == 4880) {
      if (ORIENTATIE == 1) {
        return 800;
      } else if (ORIENTATIE == 0) {
        return 480;
      }
    } else if (resolutie == 100) {
      return 100;
    }
  } else {
    return 0;
  }
  return -1;
}

int getCursorY() {
  return getCursorY(SCRIPT_RESOLUTIE);
}

int getCursorY(int16_t code_resolutie) {
  return getCursorY(code_resolutie, RESOLUTIE);
}

int getCursorY(int16_t code_resolutie, int16_t scherm_resolutie) {
  return map(tft.getCursorY(), resolutie_y(scherm_resolutie, false), resolutie_y(scherm_resolutie, true), resolutie_y(code_resolutie, false), resolutie_y(code_resolutie, true));
}



void run_scherm_pauze() {
  if (ts_touched()) {
    if (scherm_helderheid == 100) {
      digitalWrite(TFT_BL, HIGH);
    } else if (scherm_helderheid < 100) {
      analogWrite(TFT_BL, round(255*(scherm_helderheid/100)));
    }
    scherm_actief = true;

    while (ts_touched()) {
      ts_begin();
    }

    scherm_touched = millis();
    ts_begin();
  }
  else if (uit_check + 60000 > millis()) {
    digitalWrite(TFT_BL, LOW);
    uit_check = millis();
  }
}
