void tft_begin(){
  tft.begin();
  tft_scherm_helderheid();
}

void tft_setup(){
  // SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  pinMode(TFT_BL, OUTPUT);
  tft_begin();
  tft.setRotation(ROTATIE);
  tft_setup_display();
  delay(50);
}

void tft_scherm_helderheid(){
  if (tft_actief) {
    analogWrite(TFT_BL, tft_helderheid * TFT_HELDER_FACTOR);
  } else {
    tft.print(" ...");
    analogWrite(TFT_BL, 0);
  }
}


void tft_loop() {
  if (!tft_actief) {
    if (actieve_touch) {
      tft_actief = true;
      tft_scherm_helderheid();
      while (actieve_touch) {
        ts_touched();
        delay(50);
      }
    }
  } else if (!actieve_touch) {
    if ((millis() > scherm_touched + scherm_timer*1000) || (millis() < scherm_touched)) {
      tft_actief = false;
      tft_scherm_helderheid();
    }
  }
}

#if RESOLUTIE == 4880
  #if ORIENTATIE == 0
    void tft_setup_display() {
      tft.fillScreen(kleur_zwart);
      bkos_logo(100,20, 3, kleur_donker);
      tft.setCursor(20, 250);
      tft.setTextSize(4);
      tft.setTextColor(kleur_licht);
      tft.print("BKOS testscherm");
      tft.setCursor(20, 285);
      tft.setTextSize(2);
      tft.print("Dit scherm toont aan dat basis functies werken.");
      tft.setCursor(20, 305);
      tft.print("Raak het scherm aan om te zien wat er gebeurd.");
      tft.setCursor(20, 325);
      tft.print("Je kan nu je BKOS verder uitbreiden.");
    }
  #else
    void tft_setup_display() {
      tft.fillScreen(kleur_zwart);
      bkos_logo(100,20, 2, kleur_donker);
      tft.setCursor(20, 190);
      tft.setTextSize(2);
      tft.setTextColor(kleur_licht);
      tft.print("BKOS testscherm");
      tft.setCursor(20, 210);
      tft.setTextSize(1);
      tft.print("Dit scherm toont aan dat basis functies werken.");
      tft.setCursor(20, 225);
      tft.print("Raak het scherm aan om te zien wat er gebeurd.");
      tft.setCursor(20, 240);
      tft.print("Je kan nu je BKOS verder uitbreiden.");
    }
  #endif
#else
  void tft_setup_display() {
      tft.fillScreen(kleur_zwart);
      bkos_logo(100,20, kleur_donker);
      tft.setCursor(20, 100);
      tft.setTextSize(2);
      tft.setTextColor(kleur_licht);
      tft.print("BKOS testscherm");
      tft.setCursor(20, 120);
      tft.setTextSize(1);
      tft.print("Dit scherm toont aan dat basis functies werken.");
      tft.setCursor(20, 135);
      tft.print("Raak het scherm aan om te zien wat er gebeurd.");
      tft.setCursor(20, 150);
      tft.print("Je kan nu je BKOS verder uitbreiden.");
    }
#endif


void bkos_logo(int32_t x, int32_t y, uint32_t kleur) {
  bkos_logo(x, y, 1, kleur);
}
void bkos_logo(int32_t x, int32_t y, int schaal, uint32_t kleur) {
  int k = 0;
  int r = 0;
  bool teken = false;
  for (int i = 0; i < sizeof(bkos_logo_200_75)/sizeof(byte); i ++) {
    if (teken) {
      if (bkos_logo_200_75[i] > 0) {
        for (int j = 0; j < schaal; j++) {
          tft.drawLine(x+k*schaal, y+r*schaal+j, x+(k+bkos_logo_200_75[i])*schaal, y+r*schaal+j, kleur);
        }
      }
      teken = false;
    } else {
      teken = true;
    }
    k += bkos_logo_200_75[i];
    if (k >= 200) {
      r += 1;
      k = 0;
    }
  }

}