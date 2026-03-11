#line 1 "/a0/usr/workdir/BKOS5a/wifimanager.h"
#include <WiFiManager.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h> 

void wifimanager();
void wifimanager(bool forceConfig);
void wifimanager_app(String actie);