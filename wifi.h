#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

#include <WiFiManager.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h> 

#define JSON_CONFIG_FILE "/bkos4_config.json"

bool wifi__aangesloten;
bool wifi__verbonden;
bool shouldSaveConfig;

void wifimanager();
void wifimanager(bool forceConfig);
// void wifimanager_app(String actie);

void connectToWiFi();
void connectToWiFi(bool displayed);
void disconnectWiFi();
void checkWiFi();