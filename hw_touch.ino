
// 0 : Raspberry pi pico (2) + ESP8266 s01
// 1 : Raspberry pi pico (2)W
// 2 : ESP32-2432s028r (CYD, Cheap Yellow Display), kies "ESP32 WROOM DA Module"
// 3 : ESP32 VROOM     (38 pin versie) 
// 4 : ESP32-8048S043C_I  (4,3 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU
// 5 : ESP32-8048S070C_I  (7,0 inch versie van de CYD), Kies "ESP32S3 Dev Module" als MCU

#if HARDWARE == 2
    void ts_begin(){
      digitalWrite(TS_CS, HIGH);
      ts.begin(spi_ts);
    }
    
    void ts_setup(){
      spi_ts.begin(TS_SCK, TS_MISO, TS_MOSI, TS_CS);
      ts_begin();
      ts.setRotation(0);
    }
#elif HARDWARE == 3
  void ts_begin(){
    ts.begin(spi_ts);
  }
  void ts_setup(){
    spi_ts.begin(TS_SCK, TS_MISO, TS_MOSI, TS_CS);
    ts_begin();
    ts.setRotation(0);
  }
#elif HARDWARE == 4
  void ts_begin(){
    ts.read();
  }
  void ts_setup(){
    ts.begin();
    ts.setRotation(0);
  }
#elif HARDWARE == 5
  void ts_begin(){
    ts.read();
  }
  void ts_setup(){
    // Hier onstaat helaas een fout in de Serial Monitor... dat is helaas wat het is niks aan te doen (anders werkt het touchscreen niet)
    ts.begin();
    ts.setRotation(0);
  }
#else
  void ts_begin(){
    ts.begin();
  }
  void ts_setup(){
    ts_begin();
    ts.setRotation(ROTATIE);
  }
#endif

#if TS_DRIVER == 3
  bool ts_touched() {
    ts_begin();
    delay(10);
    // ts.read();
    if (ts.isTouched) {
      scherm_touched = millis();
      actieve_touch = true;
      return true;
    }
    actieve_touch = false;
    return false;
  }

  #if ORIENTATIE == 1
    #if HARDWARE == 4
      int touch_x() {
        ts.read();
        return map(ts.points[0].x, 5, 275, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].y, 5, 480, 0, 320);
      }
    #elif HARDWARE == 5
      int touch_x() {
        ts.read();
        return map(ts.points[0].x, 5, 500, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].y, 5, 800, 0, 320);
      }
    #endif
  #elif ORIENTATIE == 0
    #if HARDWARE == 4
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 5, 480, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 275, 5, 0, 320);
      }
    #elif HARDWARE == 5
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 5, 800, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 500, 5, 0, 320);
      }
    #endif
  #elif ORIENTATIE == 2
    // TEST
    #if HARDWARE == 4
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 5, 480, 0, 240);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 275, 5, 0, 320);
      }
    #elif HARDWARE == 5
      int touch_x() {
        ts.read();
        return map(ts.points[0].y, 800, 5, 240, 0);
      }
      int touch_y() {
        ts.read();
        return map(ts.points[0].x, 5, 500, 320, 0);
      }
    #endif
  #endif
#else
  bool ts_touched(){
    ts_begin():
    delay(10);
    if (ts.tirqTouched()) {
      if (ts.touched()) {
        scherm_touched = millis();
        actieve_touch = true;
        return true;
      }
    }
    actieve_touch = false;
    return false;
  }

  int touch_x() {
    TS_Point p = ts.getPoint();
    float x = (p.x - xcorrectie) / xfactor;
    // actie = 1;
    return x;
  }

  int touch_y() {
    TS_Point p = ts.getPoint();
    float y = (p.y - ycorrectie) / yfactor;
    // actie = 1;
    return y;
  }
#endif