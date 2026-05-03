// BKOS5a Revised Configuration
// Version: 5a.T260417
// Based on BaseKOS with enhanced lighting and device control

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>
#include <Preferences.h>

// Version information
#define BKOS_VERSIE "5a.T260417"
extern String BKOS_VERSIE_GIT;
extern String BKOS_GIT_ALLOWED;

// Owner information (password protected)
extern char eigenaar[25];
extern char telefoon[25];
extern char haven[25];
extern char bootnaam[25];

// Lighting modes enumeration
typedef enum {
  MODE_HAVEN = 0,   // All lights off, interior white
  MODE_ZEILEN,      // Sailing lights (3kl preferred)
  MODE_MOTOR,       // Motor lights (navi+stern+steaming)
  MODE_ANKER        // Anchor lights (stern+steaming)
} LichtMode;

// Light combination enumeration
typedef enum {
  COMB_3KL,           // Port + Starboard + Stern
  COMB_NAVI,          // Port + Starboard
  COMB_ANKER,         // Stern + Steaming
  COMB_MOTOR,         // Navi + Stern + Steaming
  COMB_NAVI_HEK,      // Navi + Stern
  COMB_3KL_STOOM,     // 3kl + Steaming (emergency for motor)
  COMB_NONE           // No combination
} LichtCombinatie;

// Device structure
struct Apparaat {
  char naam[20];
  uint8_t poort_nr;
  bool aan;
  bool auto_modus;
  bool tijdsgestuurd;
  uint32_t kleur_aan;
  uint32_t kleur_uit;
};

// Lighting mode configuration
struct LichtModusConfig {
  const char* naam;
  LichtCombinatie voorkeur;
  LichtCombinatie alternatief1;
  LichtCombinatie alternatief2;
  LichtCombinatie noodoplossing;
  uint8_t fallback_timeout_sec; // 10 seconds
  bool interieur_default_wit;
  bool handmatig_rood_mogelijk;
};

// Display settings
struct SchermConfig {
  uint8_t helderheid;            // 0-100
  bool auto_dim;
  uint8_t dim_niveau;
  bool roodlicht_dim;
  uint8_t roodlicht_dim_niveau;
  uint32_t achtergrond_kleur;
  uint32_t knop_kleur;
  uint32_t knop_ingedrukt_kleur;
  uint32_t tekst_kleur;
};

// WiFi configuration
struct WiFiConfig {
  char ssid[32];
  char pass[64];
  bool auto_connect;
  bool opslaan;
};

// OTA configuration
struct OTAConfig {
  bool altijd_actief;
  bool alleen_scherm;
  char github_url[100];
  char alternatieve_versie[20];
  bool handmatige_update;
};

// Main configuration structure
struct Config {
  // Display settings
  SchermConfig scherm;
  
  // Lighting
  LichtMode huidige_licht_modus;
  LichtCombinatie huidige_combinatie;
  bool buitenverlichting_aan;
  bool buitenverlichting_tijdsgestuurd;
  bool interieur_handmatig_rood;
  
  // Devices
  Apparaat apparaten[5];  // USB, 230V, TV, waterpomp, dekverlichting
  
  // WiFi
  WiFiConfig wifi;
  
  // OTA
  OTAConfig ota;
};

// Global configuration instance
extern Config config;

// Function declarations
extern void laad_config();
extern void sla_config_op();

extern bool check_wachtwoord(const char* ingevoerd);
extern void set_wachtwoord(const char* nieuw);

#endif
EOF && echo '=== Creating global variables ===' && cat > globals.h << 'EOF'
// Global variables for BKOS5a Revised

#ifndef GLOBALS_H
#define GLOBALS_H

#include "parameters.h"
#include <Preferences.h>

// Version strings
String BKOS_VERSIE_GIT = "5a.T260417";
String BKOS_GIT_ALLOWED = "5a";

// Owner information
char eigenaar[25] = "";
char telefoon[25] = "";
char haven[25] = "";
char bootnaam[25] = "Bootnaam";

// Global configuration
Config config;

// Preferences for persistent storage
Preferences preferences;

#endif
EOF && echo '=== Creating screen manager ===' && cat > scherm_manager.h << 'EOF'
// Screen Manager for BKOS5a Revised

#ifndef SCHERM_MANAGER_H
#define SCHERM_MANAGER_H

#include <Arduino.h>
#include "parameters.h"

// Screen type enumeration
typedef enum {
  SCHERM_BASIS = 0,           // Main control screen
  SCHERM_POORTEN,            // Port control screen
  SCHERM_WEERGAVE,           // Display settings screen
  SCHERM_IO_INSTELLINGEN,    // IO settings screen
  SCHERM_WIFI,               // WiFi settings screen
  SCHERM_INFO,               // Information screen
  SCHERM_OTA,                // OTA screen
  SCHERM_MENU,               // Menu for other screens
  SCHERM_AANTAL              // Total number of screens
} SchermType;

// Function pointer types
typedef void (*SchermToonFunc)();
typedef void (*SchermTouchFunc)(int x, int y);

// Screen structure
struct Scherm {
  SchermToonFunc toon;
  SchermTouchFunc verwerk_touch;
  const char* naam;
};

// Screen manager class
class SchermManager {
private:
  Scherm schermen[SCHERM_AANTAL];
  SchermType huidig_scherm;
  
public:
  SchermManager() {
    huidig_scherm = SCHERM_BASIS;
    for (int i = 0; i < SCHERM_AANTAL; i++) {
      schermen[i].toon = nullptr;
      schermen[i].verwerk_touch = nullptr;
      schermen[i].naam = "";
    }
  }
  
  // Register a screen
  void registreer(SchermType type, SchermToonFunc toon, SchermTouchFunc verwerk, const char* naam) {
    if (type < SCHERM_AANTAL) {
      schermen[type].toon = toon;
      schermen[type].verwerk_touch = verwerk;
      schermen[type].naam = naam;
    }
  }
  
  // Show a screen
  void toon(SchermType type) {
    if (type < SCHERM_AANTAL && schermen[type].toon) {
      // Call display clear function from hw_scherm
      // tft.fillScreen(config.scherm.achtergrond_kleur);
      schermen[type].toon();
      huidig_scherm = type;
      Serial.print("[SCHERM] Switching to: ");
      Serial.println(schermen[type].naam);
    }
  }
  
  // Process touch event
  void verwerk_touch(int x, int y) {
    if (huidig_scherm < SCHERM_AANTAL && schermen[huidig_scherm].verwerk_touch) {
      schermen[huidig_scherm].verwerk_touch(x, y);
    }
  }
  
  // Get current screen
  SchermType get_huidig() {
    return huidig_scherm;
  }
  
  // Navigate back to main screen
  void naar_basis() {
    toon(SCHERM_BASIS);
  }
};

// Global screen manager instance
extern SchermManager schermManager;

#endif
