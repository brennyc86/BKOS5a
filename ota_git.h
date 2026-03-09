void ota_git_update();
// void ota_git_update(bool displayed);
bool checkForFirmwareUpdate();
// bool checkForFirmwareUpdate(bool displayed);
void downloadAndApplyFirmware();
void downloadAndApplyFirmware(byte BKOS);

String fetchVersion5();  // Forward declaratie
String BKOS_VERSIE_GIT = "";
String BKOS5_VERSIE_GIT = "";  // BKOS5a versie van GitHub
String BKOS_GIT_ALLOWED = "";
bool update_ready;


