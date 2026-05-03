// Lighting Manager for BKOS5a Revised

#ifndef LICHT_MANAGER_H
#define LICHT_MANAGER_H

#include <Arduino.h>
#include "parameters.h"
#include "globals.h"
#include "hw_io.h"

class LichtManager {
private:
  unsigned long fallback_start;
  bool fallback_active;
  LichtCombinatie actieve_combinatie;
  
  // Port numbers for lights
  const uint8_t PORT_STUURBOORD = 1;
  const uint8_t PORT_BAKBOORD = 2;
  const uint8_t PORT_HEKLICHT = 3;
  const uint8_t PORT_STOOMLICHT = 4;
  const uint8_t PORT_DECKLIGHT = 5;
  
  bool buitenverlichting_tijd_aan() {
    // Placeholder: check if exterior lights should be on based on time
    // This should check sunset/sunrise
    return false;
  }
  
public:
  LichtManager() : fallback_start(0), fallback_active(false), actieve_combinatie(COMB_NONE) {}
  
  // Set lighting mode
  void set_modus(LichtMode modus) {
    // Mode configurations
    LichtModusConfig configs[4] = {
      // Haven
      {"Haven", COMB_NONE, COMB_NONE, COMB_NONE, COMB_NONE, 0, true, true},
      // Zeilen
      {"Zeilen", COMB_3KL, COMB_NAVI_HEK, COMB_NAVI, COMB_NAVI, 10, false, false},
      // Motor
      {"Motor", COMB_MOTOR, COMB_3KL_STOOM, COMB_NAVI_HEK, COMB_NAVI, 10, false, false},
      // Anker
      {"Anker", COMB_ANKER, COMB_NAVI, COMB_NAVI, COMB_NAVI, 10, true, false}
    };
    
    LichtModusConfig config = configs[modus];
    config.huidige_licht_modus = modus;
    
    // Try preferred combination
    if (probeer_combinatie(config.voorkeur)) {
      actieve_combinatie = config.voorkeur;
      fallback_active = false;
      // Apply combination
      pas_combinatie_toe(config.voorkeur);
      return;
    }
    
    // Start fallback timer
    fallback_start = millis();
    fallback_active = true;
    
    // Use available combination
    LichtCombinatie beschikbaar = beschikbare_combinatie();
    actieve_combinatie = beschikbaar;
    pas_combinatie_toe(beschikbaar);
  }
  
  // Update lighting (call in loop)
  void update() {
    if (!fallback_active) return;
    
    // Check fallback timeout (10 seconds)
    if (millis() - fallback_start > 10000) {
      // Find alternative combination
      actieve_combinatie = zoek_alternatief();
      pas_combinatie_toe(actieve_combinatie);
      fallback_active = false;
    }
  }
  
  // Get interior color based on lighting
  uint32_t get_interieur_kleur() {
    if (config.huidige_licht_modus == MODE_HAVEN) {
      if (config.interieur_handmatig_rood) {
        return 0xFF0000; // Red if manually set
      }
      return 0xFFFFFF; // White in haven mode
    } else if (config.huidige_licht_modus == MODE_ANKER) {
      return 0xFFFFFF; // White in anchor mode
    } else {
      // Zeilen or Motor mode
      if (config.buitenverlichting_aan || 
          (config.buitenverlichting_tijdsgestuurd && buitenverlichting_tijd_aan())) {
        return 0xFF0000; // Red when exterior lights on
      }
      return 0xFFFFFF; // White otherwise
    }
  }
  
  // Switch individual lights (optional)
  void schakel_stuurboord(bool aan) {
    poort_schakelen(PORT_STUURBOORD, aan);
  }
  
  void schakel_bakboord(bool aan) {
    poort_schakelen(PORT_BAKBOORD, aan);
  }
  
  // Get current combination
  LichtCombinatie get_actieve_combinatie() {
    return actieve_combinatie;
  }
  
  // Check if lights are on
  bool is_buitenverlichting_aan() {
    return config.buitenverlichting_aan ||
           (config.buitenverlichting_tijdsgestuurd && buitenverlichting_tijd_aan());
  }
  
private:
  bool probeer_combinatie(LichtCombinatie comb) {
    // For now, assume all combinations are available
    // In real implementation, check hardware status
    return true;
  }
  
  LichtCombinatie beschikbare_combinatie() {
    // Find available combination
    // For now, return NAVI as fallback
    return COMB_NAVI;
  }
  
  LichtCombinatie zoek_alternatief() {
    // Search for alternative combination
    // For now, return NAVI
    return COMB_NAVI;
  }
  
  void pas_combinatie_toe(LichtCombinatie comb) {
    // Turn off all lights first
    poort_schakelen(PORT_STUURBOORD, false);
    poort_schakelen(PORT_BAKBOORD, false);
    poort_schakelen(PORT_HEKLICHT, false);
    poort_schakelen(PORT_STOOMLICHT, false);
    
    switch (comb) {
      case COMB_3KL:
        poort_schakelen(PORT_STUURBOORD, true);
        poort_schakelen(PORT_BAKBOORD, true);
        poort_schakelen(PORT_HEKLICHT, true);
        break;
      case COMB_NAVI:
        poort_schakelen(PORT_STUURBOORD, true);
        poort_schakelen(PORT_BAKBOORD, true);
        break;
      case COMB_ANKER:
        poort_schakelen(PORT_HEKLICHT, true);
        poort_schakelen(PORT_STOOMLICHT, true);
        break;
      case COMB_MOTOR:
        poort_schakelen(PORT_STUURBOORD, true);
        poort_schakelen(PORT_BAKBOORD, true);
        poort_schakelen(PORT_HEKLICHT, true);
        poort_schakelen(PORT_STOOMLICHT, true);
        break;
      case COMB_NAVI_HEK:
        poort_schakelen(PORT_STUURBOORD, true);
        poort_schakelen(PORT_BAKBOORD, true);
        poort_schakelen(PORT_HEKLICHT, true);
        break;
      case COMB_3KL_STOOM:
        poort_schakelen(PORT_STUURBOORD, true);
        poort_schakelen(PORT_BAKBOORD, true);
        poort_schakelen(PORT_HEKLICHT, true);
        poort_schakelen(PORT_STOOMLICHT, true);
        break;
      case COMB_NONE:
        // All lights already off
        break;
    }
    
    // Update exterior lighting status
    config.buitenverlichting_aan = (comb != COMB_NONE);
  }
};

// Global lighting manager instance
extern LichtManager lichtManager;

#endif
EOF && echo '=== Creating device manager ===' && cat > apparaat_manager.h << 'EOF'
// Device Manager for BKOS5a Revised

#ifndef APPARAAT_MANAGER_H
#define APPARAAT_MANAGER_H

#include <Arduino.h>
#include "parameters.h"
#include "globals.h"
#include "hw_io.h"

class ApparaatManager {
private:
  Apparaat* get_apparaat(int index) {
    if (index >= 0 && index < 5) {
      return &config.apparaten[index];
    }
    return nullptr;
  }
  
public:
  void init() {
    // Initialize devices
    Apparaat devices[5] = {
      {"USB", 6, true, false, false, 0x00FF00, 0xFF0000},
      {"230V", 7, false, false, false, 0x00FF00, 0xFF0000},
      {"TV", 8, false, false, false, 0x00FF00, 0xFF0000},
      {"Waterpomp", 9, false, true, false, 0x00FF00, 0xFF0000},
      {"Dekverlichting", 10, false, false, false, 0x00FF00, 0xFF0000}
    };
    
    for (int i = 0; i < 5; i++) {
      strcpy(config.apparaten[i].naam, devices[i].naam);
      config.apparaten[i].poort_nr = devices[i].poort_nr;
      config.apparaten[i].aan = devices[i].aan;
      config.apparaten[i].auto_modus = devices[i].auto_modus;
      config.apparaten[i].tijdsgestuurd = devices[i].tijdsgestuurd;
      config.apparaten[i].kleur_aan = devices[i].kleur_aan;
      config.apparaten[i].kleur_uit = devices[i].kleur_uit;
      
      // Set initial port state
      poort_schakelen(config.apparaten[i].poort_nr, config.apparaten[i].aan);
    }
  }
  
  // Toggle device on/off
  void toggle(int index) {
    Apparaat* device = get_apparaat(index);
    if (device) {
      device->aan = !device->aan;
      device->auto_modus = false;
      device->tijdsgestuurd = false;
      poort_schakelen(device->poort_nr, device->aan);
    }
  }
  
  // Set device auto mode
  void set_auto(int index, bool auto_modus) {
    Apparaat* device = get_apparaat(index);
    if (device) {
      device->auto_modus = auto_modus;
      if (auto_modus) {
        device->aan = false;
        device->tijdsgestuurd = false;
        // Auto logic would go here
      }
    }
  }
  
  // Set device timed
  void set_timed(int index, bool timed) {
    Apparaat* device = get_apparaat(index);
    if (device) {
      device->tijdsgestuurd = timed;
      if (timed) {
        device->aan = false;
        device->auto_modus = false;
        // Timed logic would go here
      }
    }
  }
  
  // Get device status text
  const char* get_status(int index) {
    Apparaat* device = get_apparaat(index);
    if (device) {
      if (device->aan) return "AAN";
      if (device->auto_modus) return "AUTO";
      if (device->tijdsgestuurd) return "TIMED";
      return "UIT";
    }
    return "ERROR";
  }
  
  // Get device color
  uint32_t get_kleur(int index) {
    Apparaat* device = get_apparaat(index);
    if (device) {
      return device->aan ? device->kleur_aan : device->kleur_uit;
    }
    return 0x666666;
  }
  
  // Get device name
  const char* get_naam(int index) {
    Apparaat* device = get_apparaat(index);
    if (device) {
      return device->naam;
    }
    return "?";
  }
};

// Global device manager instance
extern ApparaatManager apparaatManager;

#endif
