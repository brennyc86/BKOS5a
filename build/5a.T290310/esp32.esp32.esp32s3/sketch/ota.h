#line 1 "/a0/usr/workdir/BKOS5a/ota.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

void connectToWiFi();
void connectToWiFi(bool displayed);
void ota_setup();
void ota_setup(bool displayed);
void checkWiFi();

#include "ota_git.h"
#include "ota_wifi.h"

