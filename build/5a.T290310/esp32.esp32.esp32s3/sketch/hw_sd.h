#line 1 "/a0/usr/workdir/BKOS5a/hw_sd.h"
#include <FS.h>
#include <SD.h>

#if HARDWARE <= 1  //  pi pico
  #define SD_SCK 18
  #define SD_MISO 16
  #define SD_MOSI 19
  #define SD_CS 12
  
#elif HARDWARE == 2
  #define SD_SCK  18
  #define SD_MISO 19
  #define SD_MOSI 23
  #define SD_CS    5

  SPIClass spi_sd = SPIClass(VSPI);

#elif HARDWARE == 3
  #define SD_SCK  14
  #define SD_MISO 12
  #define SD_MOSI 13
  #define SD_CS   16

#elif HARDWARE == 4
  #define SD_SCK  12
  #define SD_MISO 13
  #define SD_MOSI 11
  #define SD_CS   10

#elif HARDWARE == 5
  #define SD_SCK  12
  #define SD_MISO 13
  #define SD_MOSI 11
  #define SD_CS   10

#endif

bool sd_begin();
void sd_setup();
void sd_lees();