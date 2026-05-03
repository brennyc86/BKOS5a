#if TS_DRIVER == 1
  #include <XPT2046_Touchscreen.h>
#elif TS_DRIVER == 3
  #include <TAMC_GT911.h>
#endif

long scherm_touched;

bool actieve_touch = false;
int ts_x;
int ts_y;

// 0 : Raspberry pi pico (2) + ESP8266 s01
// 1 : Raspberry pi pico (2)W
// 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
// 3 : ESP32 VROOM     (38 pin versie) 
// 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
// 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU

#if HARDWARE == 5
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