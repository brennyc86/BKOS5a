#line 1 "/a0/usr/workdir/BKOS5a/TESTPLAN.md"
# BKOS5a Testplan

Dit testplan beschrijft wat er op hardware getest moet worden na het flashen van BKOS5a.

## Hardware vereisten
- ESP32 board (bij voorkeur HARDWARE 5: ESP32-8048S070C 7")
- BKOSS IO-module aangesloten via Serial
- WiFi netwerk beschikbaar

---

## Test 1: Touch Debounce
**Doel:** Controleer dat snelle tikken niet dubbel geregistreerd worden
1. Open schakelscherm
2. Tik snel meerdere keren achter elkaar op dezelfde knop
3. **Verwacht:** Elke tik wordt slechts éénmalig verwerkt, geen dubbele schakelacties
4. Tik langzaam (>200ms interval) op knop
5. **Verwacht:** Elke tik wordt normaal verwerkt

## Test 2: Visuele Touch-feedback
**Doel:** Controleer dat knoppen visueel reageren bij aanraking
1. Open schakelscherm
2. Tik op een schakelknop
3. **Verwacht:** Knop toont kort (~60ms) een lichtere kleur voordat de actie wordt uitgevoerd
4. **Verwacht:** Na feedback toont de knop de nieuwe staat (aan/uit)

## Test 3: WiFi-statusicoon
**Doel:** Controleer WiFi-indicator in header
1. Start op met WiFi verbonden
2. **Verwacht:** WiFi-icoon in header toont verbonden staat
3. Verbreek WiFi verbinding (router uit of buiten bereik)
4. Wacht 5 seconden (header refresh interval)
5. **Verwacht:** WiFi-icoon toont verbroken staat

## Test 4: Schermtimer Waarschuwing
**Doel:** Controleer rode rand 10s voor scherm-timeout
1. Stel scherm_timer in op minimaal 15 seconden
2. Laat het scherm 5+ seconden inactief
3. **Verwacht:** Rode rand verschijnt rondom het scherm 10s voor timeout
4. Tik op het scherm
5. **Verwacht:** Rode rand verdwijnt, scherm timer reset
6. Laat scherm volledig timeout
7. **Verwacht:** Scherm gaat uit (backlight laag)

## Test 5: IO-State Persistentie (NVS)
**Doel:** Controleer dat IO-staat bewaard blijft na herstart
1. Schakel meerdere IO-uitgangen in
2. Wacht 35 seconden (NVS schrijft na 30s inactiviteit)
3. Herstart de ESP32 (reset knop of stroomonderbreking)
4. **Verwacht:** Na herstart zijn de uitgangen in dezelfde staat als voor herstart
5. Test ook: schakel uit, wacht 35s, herstart
6. **Verwacht:** Uitgangen zijn uit na herstart

## Test 6: QR Configuratie
**Doel:** Controleer QR_TONEN schakelaar
1. Stel `#define QR_TONEN false` in BKOS.h
2. Flash firmware
3. **Verwacht:** Geen QR-code op bootscherm
4. Stel `#define QR_TONEN true` in
5. Flash firmware  
6. **Verwacht:** QR-code verschijnt op bootscherm

---

## Regressietests
Controleer dat bestaande functionaliteit intact is:
- [ ] IO schakelscherm werkt normaal
- [ ] IO-configuratie laden/opslaan via EEPROM
- [ ] WiFiManager captive portal werkt bij eerste setup
- [ ] OTA update via GitHub werkt
- [ ] Alle apps bereikbaar via homescreen
- [ ] Header alert-iconen werken correct
- [ ] FreeRTOS tasks starten correct (ioTask, wifiTask)
