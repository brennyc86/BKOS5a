String info_snaam = "";
String info_naam = "";
String info_boot = "";
String info_wifi_ssid = "";
String info_wifi_pass = "";
EOF; ls -la info.h; du -sh .; find . -name '*.ino' | wc -l; echo 'Dummy info.h created and versioned dir cleaned'; arduino-cli compile --fqbn 'esp32:esp32:esp32s3:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=opi,USBMode=hwcdc,CPUFreq=240 --build-property build.flash_mode=dio --build-property build.flash_freq=80M --build-property build.partitions=default_8MB.csv' --build-path /tmp/build_bkos5a_temp --warnings none .
