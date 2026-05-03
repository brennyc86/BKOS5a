// void wifimanager_app(String actie) {
//   if (actie == "bouw") {
//     wifimanager(true);
//   } else if (actie == "run") {
//     actieve_app = 0;
//     scherm_bouwen = true;
//   } else if (actie == "naam") {
//     item_naam = "WiFi Manager";
//   } else if (actie == "print") {
//     update_center("naam");
//     tft.print(item_naam);
//   } else if (actie == "icon_klein") {
//     int x = getCursorX() + 20;
//     int y = getCursorY() + 25;
//     setCursor(getCursorX(), getCursorY()+4);
//     drawIcon10x10(x, y, icon_wifi, kleur_zwart, 3);
//     tft.setTextColor(kleur_rood);
//   }
// }

void wifimanager() {
  wifimanager(false);
}

void wifimanager(bool forceConfig) {
  // bool forceConfig = false;

  bool spiffsSetup = loadConfigFile();
  if (!spiffsSetup) {
    forceConfig = true;
  }

  // tft.println("WiFi.mode");
  // delay(250);
  WiFi.mode(WIFI_STA);

  // tft.println("loadConfigFile");
  // delay(250);
  loadConfigFile();
  // tft.println("WiFiManager wm");
  // delay(100);
  WiFiManager wm;
  // tft.println("setDebug");
  wm.setDebugOutput(false);
  // tft.println("setSaveConfigCallback");
  wm.setSaveConfigCallback(saveConfigCallback);
  // tft.println("setAPCallback");
  wm.setAPCallback(configModeCallback);

  // tft.println("WiFiManagerParameters");
  // delay(250);
  WiFiManagerParameter veld_bootnaam("bootnaam", "Naam boot", bootnaam, 25);
  WiFiManagerParameter veld_haven("haven", "Naam haven", haven, 25);
  WiFiManagerParameter veld_eigenaar("eigenaar", "Naam eigenaar", eigenaar, 25);
  WiFiManagerParameter veld_telefoon("telefoon", "Telefoonnummer", telefoon, 25);
  // WiFiManagerParameter veld_ioconfig("io_config", "IO configuratie (1 - 7)", io_config, 2);

  tft.println("addParameters");
  wm.addParameter(&veld_bootnaam);
  wm.addParameter(&veld_haven);
  wm.addParameter(&veld_eigenaar);
  wm.addParameter(&veld_telefoon);
  // wm.addParameter(&veld_ioconfig);
  // wm.addParameter(&veld_boot_configuratie);



  
  
  wifi__aangesloten = true;

  if (forceConfig) {
      tft.fillScreen(kleur_zwart);
  
    tft.setCursor(0,0);
    tft.setTextColor(kleur_wit);
    tft.setTextSize(2);
    bkos_logo(200, 10, kleur_donker);
    tft.println("    Verbinden met WiFi . . .");
    tft.println("    Ga met je telefoon naar het netwerk");
    tft.setTextSize(3);
    tft.setTextColor(kleur_beige);
    tft.println("               BKOS4");
    tft.setTextSize(2);
    tft.setTextColor(kleur_wit);
    tft.println("    Ga vervolgens naar het volgende IP adres:");
    tft.setTextSize(3);
    tft.setTextColor(kleur_beige);
    tft.println("            192.168.4.1");
    tft.setTextSize(2);
    tft.setTextColor(kleur_wit);
    tft.println("    om de juiste instellingen in te voeren");
    tft.println("");
    tft.setTextSize(3);
    tft.println("            LETOP ! ! !");
    tft.setTextColor(kleur_actief_rood);
    tft.setTextSize(2);
    tft.println("    Geef altijd de WiFi gegevens opnieuw op!");
    tft.setTextColor(kleur_wit);  
    tft.println("    anders gaat WiFi managr helaas niet verder.");
    
    wm.setConfigPortalTimeout(120);
    if (!wm.startConfigPortal("BKOS4")) {
      tft.println("Verbinding mislukt (startConfigPortal)");
      delay(3000);
      // ESP.restart();
      // delay(5000);
      wifi__aangesloten = false;
    }
  } else {
    tft.println("autoConnect");
    wm.setTimeout(10);
    if (!wm.autoConnect("BKOS4")) {
      tft.println("Verbinding mislukt (autoConnect)");
      delay(1000);
      wifi__aangesloten = false;
      // ESP.restart();
      // delay(5000);
    // } else {
    //   tft.print("continue");
    }
  }
  // tft.println("strncpy");

  strncpy(bootnaam, veld_bootnaam.getValue(), sizeof(bootnaam));
  strncpy(haven, veld_haven.getValue(), sizeof(haven));
  strncpy(eigenaar, veld_eigenaar.getValue(), sizeof(eigenaar));
  strncpy(telefoon, veld_telefoon.getValue(), sizeof(telefoon));
  // strncpy(io_config, veld_ioconfig.getValue(), sizeof(io_config));
  // strncpy(boot_configuratie, veld_boot_configuratie.getValue(), sizeof(boot_configuratie));



  if (shouldSaveConfig) {
    tft.println("saceConfigFile");
    saveConfigFile();
  }

  // tft.println("WiFiManager compleet");
}

void saveConfigFile(){
  StaticJsonDocument<512> json;
  json["bootnaam"] = bootnaam;
  json["haven"] = haven;
  json["eigenaar"] = eigenaar;
  json["telefoon"] = telefoon;
  // json["io_config"] = io_config;
  // json["boot_configuratie"] = boot_configuratie;

  File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");

  if (!configFile) {
    tft.print("Configuratiebestand kan niet worden geopend om te schfijven");
  }

  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0) {
    tft.print("Schrijven mislukt");
  }
  configFile.close();
}
  
bool loadConfigFile() {
  if (SPIFFS.begin(true || SPIFFS.begin(true))) {
    if (SPIFFS.exists(JSON_CONFIG_FILE)) {
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
      if (configFile) {
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        serializeJsonPretty(json, Serial);
        if (!error) {
          strcpy(bootnaam, json["bootnaam"]);
          strcpy(haven, json["haven"]);
          strcpy(eigenaar, json["eigenaar"]);
          strcpy(telefoon, json["telefoon"]);
          // strcpy(io_config, json["io_config"]);
          // strcpy(tmp_boot_configuratie, json["boot_configuratie"]);
          return true;
        }
      }
    }
  }
  return false;
}

void saveConfigCallback() {
  shouldSaveConfig = true;
}

void configModeCallback(WiFiManager *myWiFiManager) {
  tft.println(myWiFiManager->getConfigPortalSSID());
}

void connectToWiFi(){
  connectToWiFi(false);
}

void connectToWiFi(bool displayed) {
  
  WiFi.mode(WIFI_STA);
  
  if (WiFi.status() != WL_CONNECTED) {
    uint16_t limiet = millis()+5000;
    WiFi.begin();
    while ((WiFi.status() != WL_CONNECTED) && (millis() < limiet)) {
      delay(500);
    }
  }

  // WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    if (displayed){
      tft.print(".");
    }
  }
  wifi__verbonden = true;
  // tft.println("\nWiFi connected");
  // tft.println("IP address: " + WiFi.localIP().toString());
}

void disconnectWiFi() {
  WiFi.disconnect();
  wifi__verbonden = false;
}

void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    wifi__verbonden = false;
  } else {
    wifi__verbonden = true;
  }
}