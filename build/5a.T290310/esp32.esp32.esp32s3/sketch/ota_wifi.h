#line 1 "/a0/usr/workdir/BKOS5a/ota_wifi.h"
#include <ESPmDNS.h>

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void ota_wifi_setup();

void ota_wifi_update();
void ota_wifi_setup(bool displayed);