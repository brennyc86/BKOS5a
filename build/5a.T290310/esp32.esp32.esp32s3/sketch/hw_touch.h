#line 1 "/a0/usr/workdir/BKOS5a/hw_touch.h"
#if TS_DRIVER == 1
  #include <XPT2046_Touchscreen.h>
#elif TS_DRIVER == 3
  #include <TAMC_GT911.h>
#endif

bool actieve_touch = false;
int ts_x;
int ts_y;

#if HARDWARE <= 1
  #define TS_IRQ  11
  #define TS_MOSI 19
  #define TS_MISO 16
  #define TS_SCK  18
  #define TS_CS   13 

  XPT2046_Touchscreen ts(TS_CS, TS_IRQ);

#elif HARDWARE == 2
  #define TS_IRQ  36
  #define TS_MOSI 32
  #define TS_MISO 39
  #define TS_SCK  25
  #define TS_CS   33

  
  XPT2046_Touchscreen ts(TS_CS, TS_IRQ);
  SPIClass spi_ts = SPIClass(VSPI);

#elif HARDWARE == 3
  #define TS_IRQ  21
  #define TS_MOSI 13
  #define TS_MISO 12
  #define TS_SCK  14
  #define TS_CS   22

  SPIClass spi_ts = SPIClass(VSPI);
  XPT2046_Touchscreen ts(TS_CS, TS_IRQ);

#elif HARDWARE == 4
  #define TS_IRQ  21
  #define TS_MOSI 13
  #define TS_MISO 12
  #define TS_SCK  20
  #define TS_CS   22
  #define TS_SDA  19
  #define TS_RST  38

  TAMC_GT911 ts = TAMC_GT911(TS_SDA, TS_SCK, -1, TS_RST, 275, 295);

  // SPIClass spi_ts = SPIClass(VSPI);
  // XPT2046_Touchscreen ts(TS_CS, TS_IRQ);
#elif HARDWARE == 5
  #define TS_IRQ  18
  #define TS_MOSI 11
  #define TS_MISO 13
  #define TS_SCK  12
  #define TS_SCK_2  20
  #define TS_CS   38
  #define TS_SDA  19
  #define TS_RST  38

  TAMC_GT911 ts = TAMC_GT911(TS_SDA, TS_SCK_2, -1, TS_RST, 490, 480);

  // SPIClass spi_ts = SPIClass(VSPI);
  // XPT2046_Touchscreen ts(TS_CS, TS_IRQ);

#endif

int xfactor = -15;
int xcorrectie = 3855;
int yfactor = -11;
int ycorrectie = 3962;

void ts_begin();
void ts_setup();
bool ts_touched();
int touch_x();
int touch_y();