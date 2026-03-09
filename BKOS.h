#include <string.h>
#include <EEPROM.h>
#include "io.h"
#include "KNOPPEN.h"
#include "BKOS_kleuren.h"
#include "BKOS_logo.h"
#include "BKOS_header.h"
#include "instellingen.h"
#include "apps.h"
#include "info.h"

#define BKOS_VERSIE "5a.T260309c"

// BKOS5a: QR-code configuratie
#define QR_TONEN true           // true = toon QR op homescreen en bootscherm
#define QR_URL "https://youtube.com/@brendanintech"  // aanpasbaar

String BKOSS;
String BKOSSV;

void BKOS_boot();


// String boot_configuratie = "1";

  

unsigned int klok_getekend = 0;

#if HARDWARE == 5
  const char* firmwareUrl = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/build/esp32.esp32.esp32s3/BKOS4.ino.bin";
  const char* firmwareMap = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/build/";
  const char* firmwareFile = "/esp32.esp32.esp32s3/BKOS4.ino.bin";
  const char* versionUrl = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/firmware/versie5.txt";
  const char* aversionUrl = "https://raw.githubusercontent.com/BrendanKoster86/BKOS4/main/firmware/alowed5.txt";
  const char* firmware5Url = "https://raw.githubusercontent.com/brennyc86/BKOS5a/main/build/esp32.esp32.esp32s3/BKOS5a.ino.bin";
  const char* version5Url = "https://raw.githubusercontent.com/brennyc86/BKOS5a/main/firmware/versie5.txt";
  const char* aversion5Url = "https://raw.githubusercontent.com/brennyc86/BKOS5a/main/firmware/alowed5.txt";
  
#else
#endif