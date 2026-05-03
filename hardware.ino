void hw_setup(){
  /* Configuratie van alle hardware
   *
   */

  tft_setup();
  ts_setup();
  hw_io_eeprom();
  // io_boot();
  wifimanager();
  ota_setup();
}

void hw_loop(){
  ts_touched();
  tft_loop();
  ota_loop();
  // io_loop();
}