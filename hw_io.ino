/* Hier wordt de opstart configuratie opgehaald uit de EEPROM
 * Het streven is dat dit de standaarden vervangt
 * Indien geen IO is opgeslagen in de EEPROM wordt een standaard daar opgeslagen
 */

#define IO_CONFIG_FILE "/bkos4_io.json"

void hw_io_eeprom() {
  /* 
   * Proberen de configuratie op te halen uit de EEPROM en als dat niet lukt een standaard configuratie laden
   */
  if (!lees_io_config()){
    tft.print(" -  schrijf config");
    schrijf_io_config(1);
    if (!lees_io_config()) {
      tft.print(" - Helaas... weer gefaald");
    }
  }

}
void io_ophalen_naam(int i, int code);

bool lees_io_config() {
  if (SPIFFS.begin(true || SPIFFS.begin(true))) {
    if (SPIFFS.exists(IO_CONFIG_FILE)) {
      File configFile = SPIFFS.open(IO_CONFIG_FILE, "r");
      if (configFile) {
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        serializeJsonPretty(json, Serial);

        delete[]io_objecten;
        delete[]io_object_ruimte;
        delete[]io_output;
        delete[]io_input;
        delete[]io_gewijzigd;
        delete[]io_open_alert;
        delete[]io_namen;
        delete[]io_events;

        if (!error) {
          io_cnt = json["io_cnt"];
          io_knoppen_cnt = 0;
          io_sensoren_cnt = 0;
          
          io_objecten = new byte[io_cnt];
          io_object_ruimte = new byte[io_cnt];
          io_output = new byte[io_cnt];
          io_input = new bool[io_cnt];
          io_gewijzigd = new bool[io_cnt];
          io_open_alert = new bool[io_cnt];
          io_alert = new byte[io_cnt];
          io_namen = new char*[io_cnt];

          for (int i = 0; i < io_cnt; i++) {
            io_objecten[i] = json["io_objecten"][i];
            io_object_ruimte[i] = json["io_object_ruimte"][i];
            io_output[i] = json["io_output"][i];
            io_input[i] = 0;
            io_gewijzigd[i] = 0;
            io_open_alert[i] = 0;
            io_alert[i] = json["io_alert"][i];
            io_ophalen_naam(i, json["io_namen"][i]);
            // for (int j = 0; j < 10; j++) {
            //   io_namen[i][j] = json["io_namen"][i][j];
            // }

            if (io_objecten[i] == 1){
              io_knoppen_cnt ++;
            } else if (io_objecten[i] == 2){
              io_sensoren_cnt ++;
            } else if (io_objecten[i] == 3){
              io_knoppen_cnt ++;
            }
          }

          int k = 0;
          int s = 0;
          io_knoppen = new int[io_knoppen_cnt];
          io_sensoren = new int[io_sensoren_cnt];

          for (int i = 0; i < io_cnt; i++) {
            if (io_objecten[i] == 1){
              io_knoppen[k] = i;
              k++;
            } else if (io_objecten[i] == 2){
              io_sensoren[s] = i;
              s++;
            } else if (io_objecten[i] == 3){
              io_knoppen[k] = i;
              k++;
            }
          }

          return true;
        }
      }
    }
  }
  return false;
}


char io_naam_opties[][10] = {"**IL_wit ", "**IL_rood", "**L_hek  ", "**L_anker", "**L_navi ",
                             "**L_3kl ", "L_stoom", "E_dek  ", "**USB    ", "**12v    ", "**230    ",
                             "**ST_AUTO", "**TV     ", "**S_MOTOR", "**S_WAL  "};

void io_ophalen_naam(int i, int code) {
  io_namen[i] = io_naam_opties[code];
}

int io_opslaan_naam(char naam[10]) {
  for (int i = 0; i < sizeof(io_naam_opties)/10; i++) {
    if (io_naam_opties[i] == naam) {
      return i;
    }
  }
  return -1;
}


void schrijf_io_config(int config) {
  StaticJsonDocument<512> json;
  if (config == 1) {
    json["io_cnt"] = 24;
    for (int i = 0; i < 24; i++) {
      json["io_objecten"][i] = 0;
      json["io_output"][i] = 0;
      json["io_alert"][i] = 0;
    }
    
    json["io_objecten"][0] = 3;
    json["io_objecten"][1] = 3;
    json["io_objecten"][13] = 3;
    for (int i = 16; i < 24; i++) {
      json["io_objecten"][i] = 3;
    }
    json["io_namen"][0] = io_opslaan_naam("**USB    ");
    json["io_namen"][1] = io_opslaan_naam("**230    ");
    json["io_namen"][2] = io_opslaan_naam("**  ?  **");
    json["io_namen"][13] = io_opslaan_naam("**L_hek  ");
    json["io_namen"][16] = io_opslaan_naam("**L_anker");
    json["io_namen"][17] = io_opslaan_naam("**L_navi ");
    json["io_namen"][18] = io_opslaan_naam("**L_3kl  ");
    json["io_namen"][19] = io_opslaan_naam("**IL_wit ");
    json["io_namen"][20] = io_opslaan_naam("**E_dek  ");
    json["io_namen"][21] = io_opslaan_naam("**IL_rood");
    json["io_namen"][22] = io_opslaan_naam("**IL_stoom");
    json["io_namen"][23] = io_opslaan_naam("**tv      ");
  } else if (config == 2) {
    json["io_cnt"] = 8;
  }

  File configFile = SPIFFS.open(IO_CONFIG_FILE, "w");

  if (!configFile) {
    tft.print("Configuratiebestand kan niet worden geopend om te schfijven");
  }

  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0) {
    tft.print("Schrijven mislukt");
  }
  configFile.close();
}
