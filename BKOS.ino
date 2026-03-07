// #if ORIENTATIE == 1
void BKOS_boot(){
  digitalWrite(TFT_BL, HIGH);
  // delay(1000);
  bkos_logo(scherm_x(120) - 100, 10, kleur_donker);
  tft.setCursor(scherm_x(120) - 95, 95);
  tft.setTextSize(8);
  tft.setTextColor(kleur_licht);
  tft.println("BKos");
  tft.setTextSize(2);
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println("Boordcomputer OS");
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println("      door");
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println(" Brendan Koster");
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.print("  versie ");
  tft.println(BKOS_VERSIE);
  tft.setCursor(scherm_x(120) - 95, tft.getCursorY() + 5);
  tft.println(MCU_TYPE);
  tft.setTextSize(1);
  tft.setTextColor(tft.color565(0, 0, 0));
  tft.setTextColor(tft.color565(255, 255, 255));
  // BKOS5a: QR-code configureerbaar via QR_TONEN in BKOS.h
//   #if QR_TONEN  // [BKOS5a disabled]
  if (ORIENTATIE == 1) {
//     qr_bytes(137, 220, qr_brendanintechYT, 3, kleur_donker, kleur_licht);  // [BKOS5a disabled]
  } else if (ORIENTATIE == 0) {
//     qr_bytes(150, 10, qr_brendanintechYT, scherm_x(3), kleur_donker, kleur_licht);  // [BKOS5a disabled]
  }
  // #endif  // [BKOS5a: QR_TONEN block end]

  tft.setTextColor(tft.color565(0, 0, 110));
  char c = ' ';
  while (Serial.available()) {
    c = Serial.read();
    // tft.print(c);
    if (!Serial.available()){
      delay(20);
    }
  }
  // tft.println("");
  if (RESOLUTIE == 4880) {
    tft.setCursor(0, 0);
  }
  tft.setTextColor(tft.color565(255, 255, 255));

  Serial.flush();
  Serial.print("AT S?");
  delay(50);
  BKOSS = "";
  unsigned int start = millis();
  while (!Serial.available() && millis() < start + 2500) {
    delay(50);
  }
  if (Serial.available()) {
    BKOSS = Serial.readStringUntil('\n');
  }
  
  Serial.print("AT V?");
  delay(50);
  BKOSSV = "";
  start = millis();
  while (!Serial.available() && millis() < start + 2500) {
    delay(50);
  }
  if (Serial.available()) {
    BKOSSV = Serial.readStringUntil('\n');
  }
  

  tft.print("  BKOSS  ");
  tft.setTextColor(tft.color565(0, 110, 0));
  tft.print(BKOSS);
  tft.setTextColor(kleur_wit);
  tft.print(" v ");
  tft.setTextColor(tft.color565(0, 110, 0));
  tft.print(BKOSSV);
  
  if (BKOSS == "" || BKOSSV == "") {
    tft.setTextColor(tft.color565(255, 0, 0));
    tft.println("Helaas, ontvangst BKOSS module ging niet helemaal goed");
    tft.println("          Dit is normaal wanneer je met UWB bent aangesloten");
    tft.println("          Aangesloten via UTP? dan kan het zijn dat de interne kabel niet goed zit.");
  } else {
    tft.print('\n');
  }
  tft.setTextColor(tft.color565(255, 255, 255));

  io_boot();
  // qr(120, 200, "brendanintech", 2)//, kleur_donker, kleur_licht);
  // qr(120, 200, "http://www.youtube.com/@brendanintech", 2, kleur_donker, kleur_licht);
  tft.println("");
  // io_detect();
  // sd_lees();
  // delay(1000); // Kleine vertraging waarin het opstartschemr blijft staan, wel zo mooi
  ts_setup();
}
// #elif ORIENTATIE == 0
//   void BKOS_boot(){
//     digitalWrite(TFT_BL, HIGH);
//     delay(1000);
//     bkos_logo(20, 10, kleur_donker);
//     tft.setCursor(25, 95);
//     tft.setTextSize(8);
//     tft.setTextColor(kleur_licht);
//     tft.println("BKos");
//     tft.setTextSize(2);
//     tft.println("  Boordcomputer OS");
//     tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);
//     tft.println("        door");
//     tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);
//     tft.println("   Brendan Koster");
//     tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);
//     tft.print("  versie ");
//     tft.println(BKOS_VERSIE);
//     tft.setCursor(tft.getCursorX()+25, tft.getCursorY()+5);
//     tft.println(MCU_TYPE);
//     // tft.setTextSize(1);
//     // tft.setTextColor(tft.color565(0, 0, 0));
//     tft.setTextColor(tft.color565(255, 255, 255));
//     qr_bytes(150, 10, qr_brendanintechYT, scherm_x(3), kleur_donker, kleur_licht);
//     // qr(120, 200, "brendanintech", 2)//, kleur_donker, kleur_licht);
//     // qr(120, 200, "http://www.youtube.com/@brendanintech", 2, kleur_donker, kleur_licht);
//     tft.println("");
//     io_detect();
//     delay(5000);
//   }
// #endif