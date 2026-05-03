void ota_loop() {
  if (ota_wifi_actief) {
    ota_wifi_update();
  }
}

void ota_setup() {
  ota_wifi_setup();
}


/*
 * Hier bevindt zich het script voor OTA over WiFi zonder tussenkomst van Github
 */


void ota_wifi_update() {
  ArduinoOTA.handle();
}

void ota_wifi_setup(){
  ota_wifi_setup(true);
}

void ota_wifi_setup(bool displayed) {
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    tft.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setPassword("admin");

  ArduinoOTA
    .onStart([]() {
      String type;
      digitalWrite(TFT_BL, HIGH);
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      tft.println("Start updating " + type);
      updaten = true;
      tft.fillScreen(kleur_zwart);
      bkos_logo(10, 10, kleur_donker);
      tft.setCursor(300, 10);
      tft.setTextSize(3);
      tft.setTextColor(kleur_groen);
      tft.println("BKOS update");
      // unsigned int lastProgress = 0;
      // unsigned int actProgress = 0;
  
    })
    .onEnd([]() {
      tft.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      // actProgress = "%u%%\r", (progress / (total / 100));
      if (true){//(actProgress != lastProgress) {
          tft.fillRect(300, 120, 200, 40, kleur_zwart);
          tft.setCursor(350, 125);
          tft.printf("%u%%\r", (progress / (total / 100)));
          tft.fillRect(100, 160, 6*total, 40, kleur_licht);
          tft.drawRect(100, 160, 600, 40, kleur_donker);
          // lastProgress = actProgress;
        }
    })
    .onError([](ota_error_t error) {
      tft.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  if (displayed) {
    tft.println("OTA Initialized");
    tft.print("IP address: ");
    tft.println(WiFi.localIP());
  }
}


/*
 * Hier bevindt zich de code van OTA via Github
 */




// // Current firmware version
// const unsigned long updateCheckInterval = 5 * 60 * 1000;  // 5 minutes in milliseconds
// unsigned long lastUpdateCheck = 0;

void ota_git_update() {
  // tft.println("\nStarting ESP32 OTA Update");

  connectToWiFi();
  delay(1000);

  if (checkForFirmwareUpdate()) {
    updaten = true;
    delay(500);
    tft.fillScreen(kleur_zwart);
    bkos_logo(10, 10, kleur_donker);
    tft.setCursor(300, 10);
    tft.setTextSize(3);
    tft.setTextColor(kleur_groen);
    tft.println("BKOS update");
    tft.setTextColor(kleur_beige);
    tft.setCursor(300, 45);
    tft.print(BKOS_VERSIE);
    tft.print(" > ");
    tft.println(BKOS_VERSIE_GIT);
    downloadAndApplyFirmware();
  }
}



bool checkForFirmwareUpdate() {
  // tft.println("Checking for firmware update...");
  if (WiFi.status() != WL_CONNECTED) {
    // tft.println("WiFi not connected");
    return false;
  }

  // Step 1: Fetch the latest version from GitHub
  BKOS_VERSIE_GIT = fetchLatestVersion();
  BKOS_GIT_ALLOWED = fetchAlowedVersions();
  if (BKOS_VERSIE_GIT == "") {
    return false;
  }

  // Step 2: Compare versions
  // if (BKOS_VERSIE_GIT != BKOS_VERSIE) {
  //   tft.print(BKOS_VERSIE_GIT);
  //   return true;
  // }
  String str_BKOS_VERSIE = BKOS_VERSIE;
  if (BKOS_GIT_ALLOWED == "") {
    if (BKOS_VERSIE_GIT != BKOS_VERSIE) {
      tft.print(BKOS_VERSIE_GIT);
      return true;
    }
  }
  if (not str_BKOS_VERSIE.indexOf(BKOS_GIT_ALLOWED) > 0) {
    return true;
  }
  return false;
}

String fetchAlowedVersions() {
  HTTPClient http;
  if (strncmp(BKOS_VERSIE, "4", 1) == 0) {
    http.begin(aversionUrl);
  } else if (strncmp(BKOS_VERSIE, "5", 1) == 0) {
    http.begin(aversion5Url);
  } else {
    http.begin(aversionUrl);
  }

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String latestVersion = http.getString();
    latestVersion.trim();  // Remove any extra whitespace
    http.end();
    return latestVersion;
  } else {
    tft.printf("Failed to fetch version. HTTP code: %d\n", httpCode);
    http.end();
    return "";
  }
}

String fetchLatestVersion() {
  HTTPClient http;
  if (strncmp(BKOS_VERSIE, "4", 1) == 0) {
    http.begin(versionUrl);
  } else if (strncmp(BKOS_VERSIE, "5", 1) == 0) {
    http.begin(version5Url);
  } else {
    http.begin(versionUrl);
  }

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String latestVersion = http.getString();
    latestVersion.trim();  // Remove any extra whitespace
    http.end();
    return latestVersion;
  } else {
    tft.printf("Failed to fetch version. HTTP code: %d\n", httpCode);
    http.end();
    return "";
  }
}

void downloadAndApplyFirmware() {
  if (strncmp(BKOS_VERSIE, "4", 1) == 0) {
    downloadAndApplyFirmware(4);
  } else if (strncmp(BKOS_VERSIE, "5", 1) == 0) {
    downloadAndApplyFirmware(5);
  } else {
    downloadAndApplyFirmware(4);
  }
}

void downloadAndApplyFirmware(byte BKOS) {
  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  if (BKOS == 4) {
    http.begin(firmwareUrl);
  } if (BKOS == 5) {
    http.begin(firmware5Url);
  } if (BKOS == 6) {
    http.begin(firmware5aUrl);
  } else {
    http.begin(firmwareUrl);
  }

  int httpCode = http.GET();
  // tft.printf("HTTP GET code: %d\n", httpCode);

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    // tft.printf("Firmware size: %d bytes\n", contentLength);

    if (contentLength > 0) {
      WiFiClient* stream = http.getStreamPtr();
      if (startOTAUpdate(stream, contentLength)) {
        tft.println("OTA update successful, restarting...");
        delay(2000);
        ESP.restart();
      } else {
        tft.println("OTA update failed");
      }
    } else {
      tft.println("Invalid firmware size");
    }
  } else {
    tft.printf("Failed to fetch firmware. HTTP code: %d\n", httpCode);
  }
  http.end();
}


bool startOTAUpdate(WiFiClient* client, int contentLength) {
  // tft.println("Initializing update...");
  if (!Update.begin(contentLength)) {
    tft.printf("Update begin failed: %s\n", Update.errorString());
    return false;
  }

  // tft.println("Writing firmware...");
  size_t written = 0;
  int progress = 0;
  int lastProgress = 0;

  // Timeout variables
  const unsigned long timeoutDuration = 120*1000;  // 10 seconds timeout
  unsigned long lastDataTime = millis();

  tft.setTextColor(kleur_beige);
  tft.setTextSize(5);
  while (written < contentLength) {
    if (client->available()) {
      uint8_t buffer[128];
      size_t len = client->read(buffer, sizeof(buffer));
      if (len > 0) {
        Update.write(buffer, len);
        written += len;

        // Calculate and print progress
        progress = (written * 100) / contentLength;
        if (progress != lastProgress) {
          tft.fillRect(300, 120, 200, 40, kleur_zwart);
          tft.setCursor(350, 125);
          tft.printf("%d%%\n", progress);
          tft.fillRect(100, 160, 600*progress, 40, kleur_licht);
          tft.drawRect(100, 160, 600, 40, kleur_donker);
          lastProgress = progress;
        }
      }
    }
    // Check for timeout
    if (millis() - lastDataTime > timeoutDuration) {
      tft.println("Timeout: No data received for too long. Aborting update...");
      Update.abort();
      return false;
    }

    yield();
  }
  tft.setTextColor(kleur_wit);
  tft.setTextSize(2);
  tft.print('\n');
  
  tft.println("\nWriting complete");

  if (written != contentLength) {
    tft.printf("Error: Write incomplete. Expected %d but got %d bytes\n", contentLength, written);
    Update.abort();
    return false;
  }

  if (!Update.end()) {
    tft.printf("Error: Update end failed: %s\n", Update.errorString());
    return false;
  }

  tft.println("Update geslaagd");
  return true;
}