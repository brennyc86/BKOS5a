# BKOS5a Changelog

## Versie 5a.T260303
**Gebaseerd op BKOS4 (4.T260226)**  
Alle wijzigingen zijn backwards-compatible en houden de multi-board HAL intact.

---

### 🆕 Nieuwe features

#### 1. Touch Debounce (`BKOS4.ino`, `diverse.h`)
- Constante `TOUCH_DEBOUNCE_MS = 200ms` toegevoegd
- Variabele `laatste_touch_tijd` bijgehouden in `guiLoop()`
- Voorkomt dubbele tik-registraties bij snelle aanraking
- Long-press detectie blijft onaangetast

#### 2. Visuele Touch-feedback (`KNOPPEN.ino`, `KNOPPEN.h`)
- Nieuwe functie `knop_feedback(int knop_nummer)`
- Toont 60ms beige kleurflits bij aanraking van een knop
- Geïntegreerd in `BKOS_SCHAKEL.ino` bij elke schakelactie

#### 3. WiFi-statusicoon in Header
- Was al aanwezig via `drawIconWifi10x10()` in `BKOS_header.ino`
- Wordt ververst bij elke `klok_update()` aanroep (5s interval)
- Toont verbonden/verbroken staat via bestaande `wifi__aangesloten` variabele

#### 4. Schermtimer Waarschuwing (`BKOS4.ino`, `div_schermacties.ino`)
- Nieuwe functie `scherm_timeout_waarschuwing()` - rode rand van 4px
- Wordt getoond 10 seconden voor scherm-timeout
- Variabele `scherm_waarschuwing_actief` voorkomt herhaald tekenen
- Scherm hertrekt automatisch bij nieuw scherm-bouwen

#### 5. IO-State Persistentie via NVS (`io.ino`, `io.h`)
- ESP32 `Preferences` library gebruikt (NVS met wear leveling)
- Functies: `io_state_opslaan()`, `io_state_laden()`, `io_state_check()`
- Dirty-flag systeem: schrijft pas na 30 seconden inactiviteit
- `io_state_laden()` aangeroepen in `io_boot()` na `io_set_defaults()`
- `io_state_check()` draait in `ioLoop()` (FreeRTOS task)
- `#ifdef ESP32` guards voor Pico-compatibiliteit
- Pico: gebruik LittleFS + JSON als alternatief (TODO)

#### 6. Visuele Touch-feedback Knoppen
- `knop_feedback()` geïmplementeerd - zie punt 2

#### 7. QR-code Configureerbaar (`BKOS.h`, `BKOS.ino`)
- Nieuwe defines in `BKOS.h`:
  - `QR_TONEN true/false` - aan/uit schakelaar
  - `QR_URL` - aanpasbare URL
- Boot-scherm QR wrapped in `#if QR_TONEN`

---

### 📝 Technische notities
- Versiestring: `BKOS_VERSIE = "5a.T260303"`  
- Multi-board HAL (`#define HARDWARE`) intact gelaten
- Alle ESP32-specifieke code wrapped in `#ifdef ESP32`
- Geen breaking changes t.o.v. BKOS4

---

### 🔮 TODO (Fase 2)
- Pico LittleFS IO-state opslag
- FreeRTOS task prioriteiten (ioTask + wifiTask op Core 1)
- Magic numbers vervangen door defines
- String-actie dispatch naar enum migreren
- Spelletjes vervangen (Zeeslag, Dammen, Schaken)

## v5a.T260309b — Bugfixes
- Fix: Rode rand verdwijnt nu correct na scherm timeout + wake-up (scherm_bouwen altijd true bij wake-up)
- Fix: io_now volatile voor correcte cross-core zichtbaarheid (freeRTOS dual core)
- Fix: IO timer reset na directe schakelactie verwijderd - periodieke timer loopt nu onafhankelijk
- Verbeter: Meerdere schakelacties snel achter elkaar worden nu altijd direct doorgegeven

## v5a.T260309c - 2026-03-09
- Nieuw icon: **E_dek (deklicht - spotlight in de mast voor voordek)
  - Status 0 (uit): zwart
  - Status 1 (aan): wit
