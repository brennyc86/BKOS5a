// Includeer hier de header files van de aanwezige apps
#include "BKOS_HOME.h"
#include "BKOS_SCHAKEL.h"

#include "ota.h"
#include "wifimanager.h"

#include "APP_exterieur.h"
// #include "APP_spellen.h"  // [BKOS5a] verwijderd - niet essentieel
// #include "APP_morse.h"    // [BKOS5a] verwijderd - niet essentieel

#include "app_configmodus.h"



// Geef aan hoeveel apps er zijn
int app_cnt = 6;
// Zet in de lijst hieronder de apps. De eerste positie is gereserveerd voor het thuisscherm
void (*apps[])(String actie) = {thuisscherm, schakelscherm, exterieur, configmodus, update_center, wifimanager_app};
// Het indexnummer van de app waarin wordt opgestart
int actieve_app = 2;

String item_naam;
bool homeknop_actief = false;
bool homeknop_half_actief = false;

// APP INSTRUCTIES
/*
 * De hoofd void van een app moet een String parameter "actie" hebben
 * Deze actie kan zijn:
     * bouw       :   Bouw de app
     * run        :   Draai de app
     * naam       :   tft.print(<naam van de app>)
     * icon_groot :   Teken instructies voor een icon.
     * icon_klein :   Teken instructies voor een icon.
 */

void app_uitvoeren();
