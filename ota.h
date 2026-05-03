#include <ESPmDNS.h>

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

bool ota_wifi_actief = true;
bool updaten;
void ota_loop();
void ota_setup();


const char* firmwareUrl = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/build/esp32.esp32.esp32s3/BKOS4.ino.bin";
const char* firmwareMap = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/build/";
const char* firmwareFile = "/esp32.esp32.esp32s3/BKOS4.ino.bin";
const char* versionUrl = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/firmware/versie5.txt";
const char* aversionUrl = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/firmware/alowed5.txt";
const char* firmware5Url = "https://raw.githubusercontent.com/BrendanKoster86/BKOS5/main/build/esp32.esp32.esp32s3/BKOS5.ino.bin";
const char* version5Url = "https://raw.githubusercontent.com/BrendanKoster86/BKOS5/main/firmware/versie5.txt";
const char* aversion5Url = "https://raw.githubusercontent.com/BrendanKoster86/BKOS5/main/firmware/alowed5.txt";
const char* firmware5aUrl = "https://raw.githubusercontent.com/brennyc86/BKOS5a/main/build/esp32.esp32.esp32s3/BKOS5a.ino.bin";
const char* version5aUrl = "https://raw.githubusercontent.com/brennyc86/BKOS5a/main/firmware/versie5a.txt";
const char* aversion5aUrl = "https://raw.githubusercontent.com/brennyc86/BKOS5a/main/firmware/alowed5a.txt";