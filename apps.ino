void app_uitvoeren() {
  ts_begin();
  if (scherm_bouwen) {
    homeknop_actief = true;
    homeknop_half_actief = false;
    if (DP_DRIVER == 2) {
      achtergrond(kleur_donker);
    }
    apps[actieve_app]("naam");
    header_plaatsen(item_naam);
    apps[actieve_app]("bouw");
    apps[actieve_app]("naam");
    if (homeknop_actief) {
      homeknop_plaatsen();
    } else if (homeknop_half_actief) {
      halve_homeknop_plaatsen();
    }

    scherm_bouwen = false;
  } else {
    if (actieve_touch) {
      if (ts_y < 30) {
        klik_header(ts_x, ts_y);
      } else if (homeknop_actief) {
        if (ts_y >= home_knop[1]) {
          actieve_app = 0;
          // actief_spel = 0;  // [BKOS5a removed]
          scherm_bouwen = true;
        }
      } else if (homeknop_half_actief) {
        if ((ts_y >= home_knop[1]) & (ts_x <= (SX_MAX / 2))) {
          actieve_app = 0;
          // actief_spel = 0;  // [BKOS5a removed]
          scherm_bouwen = true;
        }
      }
      
    }

    if (!scherm_bouwen) {
      apps[actieve_app]("run");
      if ((actieve_touch) && (!slepen_toestaan)) {

        bool lang = false;

        unsigned long startklik = millis();
        delay(50);
        ts_begin();
        while (ts_touched()) {
          delay(50);
          ts_begin();
          delay(50);
          if ((!homeknop_actief) && (!homeknop_half_actief) && (!lang)) {
            if (millis() > startklik + 1000) {
              fillCircle(ts_x, ts_y, 15, kleur_wit);
              lang = true;
            }
          }
        }

        if ((!homeknop_actief) && (!homeknop_half_actief) && (lang)) {
          if (beeld_vraag("Naar homescherm?", "", "Ja", "Nee") == 0) {
            actieve_app = 0;
            // actief_spel = 0;  // [BKOS5a removed]
            scherm_bouwen = true;
          } else {
            scherm_bouwen = true;
          }

        }
      }
    }
    delay(50);
  }
}