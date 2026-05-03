void io_boot();
void io_detect();
void io();

void io_loop() {
  if (millis() > io_gecheckt + io_timer) {
    io_runned = false;
  }
  if (!io_runned){
    io();
  }
}

// bool io_verbonden = true;