


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
  } else {
    // tft.setTextColor(kleur_actief_groen);
    // tft.println("BKOS4 versie is actueel:");
    // tft.setTextSize(3);
    // tft.println(BKOS_VERSIE_GIT);
    // tft.setTextSize(1);
    // tft.setTextColor(kleur_wit);
    // delay(3000);
  }
}

// void loop() {
//   tft.println("Current Firmware Version: " + String(BKOS_VERSIE));
//   delay(3000);  // delay to prevent flooding serial
// }



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

String fetchVersion5() {
  HTTPClient http;
  http.begin(version5Url);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String v = http.getString();
    v.trim();
    http.end();
    return v;
  }
  http.end();
  return "";
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
  if (BKOS == 5) {
    http.begin(firmware5Url);
  } else {
    http.begin(firmwareUrl);  // BKOS 4 of standaard
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
          tft.fillRect(300, 120, 200, 200, kleur_zwart);
          tft.setCursor(350, 125);
          tft.printf("%d%%\n", progress);
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