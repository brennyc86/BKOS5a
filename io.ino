void io_boot() {
  io_setup();  // Setup specifiek voor de computer
  io_detect(); // Detecteren hardware op basis van de computer
  bool done = false;
  // if (aparaten_cnt == 1) {
  //   if (aparaten[0] == 130){
  //     // Uitgaande van de basis test configuratie
  //     tft.println("1 module configuratie'(2)");
  //     io_set_defaults(2);
  //     done = true;
  //   }
  // } else if (aparaten_cnt == 2) {
  //   if ((aparaten[0] == 130) && (aparaten[1] == 254)){
  //     // Uitgaande van de basis test configuratie
  //     tft.println("1 module configuratie'(2)");
  //     io_set_defaults(2);
  //     done = true;
  //   } else if ((aparaten[0] == 130) && (aparaten[1] == 130)){
  //     // Uitgaande van de basis 2 module test configuratie
  //     tft.println("2 module configuratie'(3)");
  //     io_set_defaults(3);
  //     done = true;
  //   }
  // } else if (aparaten_cnt == 3) {
  //   if ((aparaten[0] == 130) && (aparaten[1] == 130) && (aparaten[2] == 130)){
  //     // Configuratie Glory
  //     tft.println("Configuratie voor de Glory (1)");
  //     io_set_defaults(1);
  //     done = true;
  //   }
  // }
  if (!done) {
    tft.println("Geen configuratie gevonden.");
    io_set_defaults();
  }
}


String io_code_naar_naam(byte code){
  if (code == 2) {
    return "8 bit Logische module";
  } else if (code == 50) {
    return "8 bit Hub";
  } else if (code == 51) {
    return "Analoge hub";
  } else if (code == 52) {
    return "uart hub";
  } else if (code == 66) {
    return "8 bit Communicatie";
  } else if (code == 130) {
    return "8 bit Schakelmodule";
  } else if (code == 3) {
    return "16 bit Logische module";
  } else if (code == 83) {
    return "16 bit Communicatie";
  } else if (code == 98) {
    return "8 bit Knoppen";
  } else if (code == 115) {
    return "16 bit Knoppen";
  } else if (code == 194) {
    return "8 bit Controller";
  } else if (code == 211) {
    return "16 bit Controller";
  } else if (code == 147) {
    return "16 bit Schakel module";
  } 
  return "";
}

# if IO_METHODE == 2
  /* In HARDWARE modus 2 wordt gebruik gemaakt van de CYD die niet beschikt over mijn communicatie poort.
   * Er zijn 2 oplossingen mogelijk om dit op te lossen. Beide zijn gebaseerd op seriele communicatie met AT tiny micro controller
   *   1. Een tussen module met enkel een ATtiny
   *   2. De eerste 74HC14 chip vervangen voor een 180 graden gedraaide ATtiny 44 of 84 die bij het eerste aparaat de omzetting doet
   * Er moet nog wat getest worden of het handig is om de standaard Serial.print() te gebruiken, of dat er beter een ander kanaal kan worden gebruikt.
   * Op het momnt heeft het eerste mijn voorkeur mocht dat lukken.
   */

  bool io_verbonden = true;

  void io_setup() {
    return;
  }

  void io_detect() {
    /* Deze functie zorgt er vopor dat wordt gedetecteerd welke externe hardware overal is aangesloten.
     * Dit is een vrij complexe taak en zal zonder meer veel tweaken vragen.
     * Alle randapparatuur heeft een ID code ter grootte van 8 bit (1 byte) dat kan worden uitgelezen
     * Nog niet alles is uitgedacht en getest, maar op het moment is het handig uit te gaan van de volgende codes:
     *
     * ID  :   Naam                 |  Registratie  |   Omschrijving (primaire) doel
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   2 : 8 bit logische module  |   8 sensoren  | Ontvangst logisch voltage sensoren (kan ook voor aansturen logisch voltage aparaten worden gebruikt)
     *  50 : 8 bit hub              |       -       | Schakelt tussen verschillende uitgangen. Bij deze moduele moeten alle uitgangen worden bekeken wat hier achter zit.
     *  51 : Analoge hub            |       -       | Schakelt analoog 1 poort van de MCU analoog door zodat een analoge input of output van een andere bijvoorbeeld sensor kan worden geregistreerd.
     *  52 : uart hub               |       -       | Schakelt direct 2 poorten van de MCU analoog door zodat bijvoorbeeld de uart naar een ander aparaat kan linken.
     *  66 : 8 bit communicatie     |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen. Deze items overslaan in het overzicht.
     * 130 : 8 bit Schakel module   |   8 knoppen   | Aansturen aparaten (ook voor 12 volt sensoren te gebruiken)
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   3 : 16 bit logische module |  16 sensoren
     *  83 : 16 bit communicatie    |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen
     *  98 :  8 bit knoppen         |       -       | Aansturing van de boodcomputer met een 8 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 115 : 16 bit knoppen         |       -       | Aansturing van de boodcomputer met een 16 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 194 :  8 bit controller      |       -       | n.t.b. spel controller met 8 knoppen
     * 211 : 16 bit controller      |       -       | n.t.b. spel controller met 16 knoppen
     * 147 : 16 bit Schakel module  |  16 knoppen
     * 
     */
    aparaten_cnt = 0;
    
    int bit;
    int input;
    bool eind = false;
    io_verbonden = true;
    byte factor;
    char A;
    char L;
    // Stap 1 is uitzoeken hoeveel hardware er is aangesloten

    tft.println("  IO detectie");
    tft.setCursor(tft.getCursorX(), tft.getCursorY()+5);

    // Serial.println();
    // Serial.println(Serial.available());

    delay(100);
    
    unsigned long timeout = 5000;
    unsigned long laatste = millis();
    // Start maken in communiceren met de Seriele module door het versie nummer op te vragen.
    // De verwachte response is een 'V' gevolgd door de versie aanduiding welk niet gespecificeerd is als 1 println statement.'

    // Eerst even de serial leeg maken
    Serial.println("IOD");
    delay(50);
    while (Serial.available()) {
      A = Serial.read();
      delay(50);
    }
    tft.print("  ");
    
    while (!eind) {
      bit = 1;
      laatste = millis();
      tft.print(aparaten_cnt+1);
      tft.print(": ");
      input = 0;
      
      Serial.print("00000000");
      while (!Serial.available() && (millis() < laatste + timeout)) {
        delay(50);
      }
      
      while (Serial.available()) {
        A = Serial.read();
        if (A == '1') {
          int add_number = 1;
          for (int i = 1; i < bit; i++){
            add_number = add_number*2;
          }
          input += add_number;
        } 
        bit ++;
        while (bit < 9 && !Serial.available() && (millis() < laatste + timeout)) {
          delay(50);
        } 
      }

      if (bit != 9) {
        eind = true;
        tft.print("Geen 8 bit ontvangen");
      }
      
      if (input == 0) {
        eind = true;
      } else if (input == 255){
        eind = true;
      } else {
        aparaten[aparaten_cnt] = input;
        tft.print(" (");
        tft.print(input);
        tft.print(") ");
        aparaten_cnt ++;
        bit = 1;
        if (aparaten_cnt >= 30) {
          eind = true;
          tft.print("Te veel voor nu");
        }
      }
    }
    
    Serial.print('\n');
    delay(50);
    while (Serial.available()) {
      L = Serial.read();
    }

    if (aparaten_cnt == 0) {
      io_verbonden = false; // was false
      io_lost = millis();
      tft.print("Geen BKOSS module gevonden");
      delay(2500);
    } else {
      tft.println('-');
      tft.print("  ");
      tft.print(aparaten_cnt);
      if (aparaten_cnt == 1) {
        tft.println(" Module gevonden");
      } else {
        tft.println(" Modules gevonden");
      }
      if (aparaten_cnt <= 2) {

      }
      tft.print("  ");
      delay(100);
    }
  }

  void io() {
    // Serial.print("IO");
    char invoer;
    bool tmp_status;
    int i_uit;
    int i_in;

    while (Serial.available()){
      // Leeg maken van oude communicatie
      invoer = Serial.read();
      // tft.print(invoer);
    }
    
    unsigned long timeout = 5000;
    unsigned long laatste = millis();

    if (!io_verbonden &&  millis() > io_lost + timeout) {
      io_verbonden = true;
    }

    if (io_verbonden) {
      if (!io_actief){
        io_actief = true;
        Serial.println("IO");
        delay(10);

        while (Serial.available()){
          delay(50);
        }

        if (io_cnt >= 8) {
          // Een werkvoorraadje maken van 8 opdrachten zodat de complete overdracht (denk ik) sneller verloopt
          for (int i = 0 ; i < 8 ; i++) {
            i_uit = io_cnt - (i+1);
            if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
              Serial.print('1');
            } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
              Serial.print('0');
            }
          }
        } else {
          for (int i = 0 ; i < io_cnt ; i++) {
            i_uit = io_cnt - (i+1);
            if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
              Serial.print('1');
            } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
              Serial.print('0');
            }
          }
        }

        int i = 0;
        while (i < io_cnt && io_verbonden) {
          i_in = i;

          delay(25);
          invoer = ' ';
          int r = 1;
          bool draaien = true;
          bool antwoord = false;
          
          while (!Serial.available() && millis() < laatste + timeout){
            delay(50);
          }

          if (Serial.available()) {
            tft.setTextColor(tft.color565(0, 255, 0));
            invoer = Serial.read();
            if ((invoer == '0') || (invoer == '1')){
              if (invoer == '1') {
                tmp_status = true;
              } else if (invoer == '0') {
                tmp_status = false;
              }

              if (tmp_status != io_input[i_in]) {
                // Bij een wijziging vaststellen dat deze is gewijzigd en ook de aanpassing opslaan
                io_gewijzigd[i_in] = true;
                io_input[i_in] = tmp_status;
                // io_meeschakelen(i_in);
              }

            } else {
              delay(5);
            }
          } else {
            io_verbonden = false; // was false
            io_lost = millis();
            
          }
  
          if (io_cnt > i+8) {
            i_uit = io_cnt - (i+9);
            if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
              Serial.print('1');
            } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
              Serial.print('0');
            }
          } else if (io_cnt == i+8) {
            Serial.print('\n');
          }
          i++;
        }

        while (Serial.available()) {
          invoer = Serial.read();
        }

        io_actief = false;
        io_runned = true;
      }
    }
  }

  void io_reset_alert(){

  }

#else
  /* Wanneer er eigen hardware wordt gebruikt is mijn 10 pins seriele poort wel beschikbaar.
   * Dan kan er gelijk gebruik worden gemaakt van de standaard communicatie wat veel enorm vereenvoudigd.
   */
  
  void io_setup() {
    pinMode(HC_PCK, OUTPUT);
    pinMode(HC_SCK, OUTPUT);
    pinMode(HC_IN, INPUT);
    pinMode(HC_UIT, OUTPUT);
    pinMode(HC_ID, INPUT);
  }
  
  void io_detect() {
    /* Deze functie zorgt er vopor dat wordt gedetecteerd welke externe hardware overal is aangesloten.
     * Dit is een vrij complexe taak en zal zonder meer veel tweaken vragen.
     * Alle randapparatuur heeft een ID code ter grootte van 8 bit (1 byte) dat kan worden uitgelezen
     * Nog niet alles is uitgedacht en getest, maar op het moment is het handig uit te gaan van de volgende codes:
     *
     * ID  :   Naam                 |  Registratie  |   Omschrijving (primaire) doel
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   2 : 8 bit logische module  |   8 sensoren  | Ontvangst logisch voltage sensoren (kan ook voor aansturen logisch voltage aparaten worden gebruikt)
     *  50 : 8 bit hub              |       -       | Schakelt tussen verschillende uitgangen. Bij deze moduele moeten alle uitgangen worden bekeken wat hier achter zit.
     *  51 : Analoge hub            |       -       | Schakelt analoog 1 poort van de MCU analoog door zodat een analoge input of output van een andere bijvoorbeeld sensor kan worden geregistreerd.
     *  52 : uart hub               |       -       | Schakelt direct 2 poorten van de MCU analoog door zodat bijvoorbeeld de uart naar een ander aparaat kan linken.
     *  66 : 8 bit communicatie     |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen. Deze items overslaan in het overzicht.
     * 130 : 8 bit Schakel module   |   8 knoppen   | Aansturen aparaten (ook voor 12 volt sensoren te gebruiken)
     -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
     *   3 : 16 bit logische module |  16 sensoren
     *  83 : 16 bit communicatie    |       -       | Communicatie tussen 2 boordcomputers. Moet nog een protocol voor komen
     *  98 :  8 bit knoppen         |       -       | Aansturing van de boodcomputer met een 8 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 115 : 16 bit knoppen         |       -       | Aansturing van de boodcomputer met een 16 bit knoppen panneel... n.t.b. hoe dit gaat werken
     * 194 :  8 bit controller      |       -       | n.t.b. spel controller met 8 knoppen
     * 211 : 16 bit controller      |       -       | n.t.b. spel controller met 16 knoppen
     * 147 : 16 bit Schakel module  |  16 knoppen
     * 
     */
    

  }

  void io() {

    if (io_actief == false) {

      io_actief = true;
  
      bool tmp_status;
      int i_uit;
      int i_in;
      // int i_in_ = 0;
      // int i_in__ = -1;
    
      // Paralelle klokslag geven
      digitalWrite(HC_PCK, LOW);
      delay(10);
      digitalWrite(HC_PCK, HIGH);
      // delay(10);
        
      // digitalWrite(HC_ID, LOW);
    
      // Nu het paralelle gedeelte geklokt heeft de data serieel uitsturen en uitlezen
      for (int i = 0 ; i < io_cnt ; i++) {
    
        // Seriele klok laag
        digitalWrite(HC_SCK, LOW);
        delay(1);
        
        i_uit = io_cnt - (i+1);
        i_in = i;
        
        // Gewenste object zenden (in omgekeerde volgorde)
        if ((io_output[i_uit] == 1) || (io_output[i_uit] == 2) || (io_output[i_uit] == 5)) { // 1 = aan, 2 = inv. uit, 5 = inv geblokkeerd
          digitalWrite(HC_UIT, HIGH);
        } else { // 0 = uit, 3 = inv aan, 4 = geblokkeerd
          digitalWrite(HC_UIT, LOW);
        }
        
        // Invoer lezen
        tmp_status = digitalRead(HC_IN);
        // Controleren of de invoer is gewijzigd. Dit wel in de gewone volgorde omdat dit binnen komend signaal is.
        if (tmp_status != io_input[i_in]) {
          // Bij een wijziging vaststellen dat deze is gewijzigd en ook de aanpassing opslaan
          io_gewijzigd[i_in] = true;
          io_input[i_in] = tmp_status;
          // io_meeschakelen(i_in);
        }
    
        delay(1);
        // Seriele klok hoog
        digitalWrite(HC_SCK, HIGH);
        delay(1);
      
      }
    
      // Paralelle klokslag geven
      digitalWrite(HC_PCK, LOW);
      delay(10);
      digitalWrite(HC_PCK, HIGH);

      io_actief = false;
      io_gecheckt = millis();
    
    }
  }

  void io_reset_alert(){

  }

#endif

void io_schakel(int knop) {
  if (io_output[io_knoppen[knop]] == 0) {
    // uit wordt aan
    io_output[io_knoppen[knop]] = 1;
    knoppen_status[knop] = 1;
  } else if (io_output[io_knoppen[knop]] == 1) {
    // aan wordt uit
    io_output[io_knoppen[knop]] = 0;
    knoppen_status[knop] = 0;
  } else if (io_output[io_knoppen[knop]] == 2) {
    // uit wordt aan (maar dan voor de omgekeerde schakeling)
    io_output[io_knoppen[knop]] = 3;
    knoppen_status[knop] = 1;
  } else if (io_output[io_knoppen[knop]] == 3) {
    // aan wordt uit (maar dan voor de omgekeerde schakeling)
    io_output[io_knoppen[knop]] = 2;
    knoppen_status[knop] = 0;
  }
  knop_plaatsen(knop);
  // io();
  // delay(50);
  // io();
  io_now = true;
}

int io_output_status(byte output) {
  /* Deze functie zet de io_output (0 - 5) om in de weer te geven status (0 - 2) */
  
  if (output == 0) {
    return 0;
  } else if (output == 1) {
    return 1;
  } else if (output == 2) {
    return 0;
  } else if (output == 3) {
    return 1;
  } else if (output == 4) {
    return 2;
  } else if (output == 5) {
    return 2;
  }
  return 99;
}


void io_set_defaults(){
  // io_set_defaults(2);
  io_set_defaults(EEPROM.read(16));
  #ifdef ESP32
  io_state_laden(); // BKOS5a: herstel laatste IO-staat uit NVS
  #endif
  
}

void io_set_defaults(int standaard){
  /* Ik wil een aantal standaarden maken om de module voor het werken met de SD kaart toch flexiebel te maken in de testfase
   * Mijn streven is om op adres 616 van de EEPROM een cijfer op te slaan die refereert aan de gebruikte standaard
   * De standaarden die ik wil inbouwen zijn:
   * 
   1: De setup zoals die op mijn boot is, die kan aanveranderingen onderhevig zijn
   2: Enkel module test/demo setup
   3: Dubbele module test/demo setup
   *
   4: Escaperoom setup met 1 module
   5: Escaperoom setup met 2 modules
   *
   6: demo
   */

  //  int standaard = 2;//EEPROM.read(16);
   tft.print("IO_standaard: ");
   tft.print(' ');
   tft.println(standaard);
   if (standaard == 1){
    io_set_1();
   } else if (standaard == 2){
    io_set_2();
   } else if (standaard == 3){
    io_set_3();
   } else if (standaard == 4){
    io_set_4();
   } else if (standaard == 5){
    io_set_5();
   } else if (standaard == 6){
    io_set_6();
   } else {
    EEPROM.write(16, 2);
    tft.println("Poging dit voor volgende keer op 1 te zetten");
    io_set_2();
    delay(500);
   }
}

void io_set_1(){
  /* Setup voor mijn eigen boot... Mogelijk aan veranderingen onder hevig */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 24;
  io_knoppen_cnt = 11;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**USB    ";
    } else if (nr == 1) {
      io_namen[nr] = "**230    ";
    } else if (nr == 14){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek";
    } else if (nr == 16){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 17){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 18){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 19){
      // io_objecten[nr] = 3;
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 20){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 21){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**IL_rood";
    } else if (nr == 22){
      io_knoppen[9] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 23){
      io_knoppen[10] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**tv     ";
    }
  }
}

void io_set_2(){
  /* Basis setup voor de test module en demo met 1 module 
   * De volgende Dingen kunnen worden aangesloten:
   *
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 8;
  io_knoppen_cnt = 8;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 7){
      io_knoppen[nr] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    }
  }
}

void io_set_3(){
  /* Setup voor de test/demo met 2 modulen
   * Uitgangspunt is een uitbreiding op bovenstaande
   * De eerste module kan worden vervangen door een module met kapote lamp detectie
   * De tweede module niet omdat dit de mogelijkheid van input blokkeert.
   * De volgende Dingen kunnen worden aangesloten:
   *
   MODULE 1:
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   *
   MODULE 2:
   1: USB stopcontacten
   2: 12 volt stopcontacten
   3: 230 volt omvormer
   4: Stuurautomaat
   5: TV
   6: ?
   7: Motor actief (signaal lampje dynamo)
   8: Walstroom actief (kleine 12 volt voeding op walstroom)
   *
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 16;
  io_knoppen_cnt = 13;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 7){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 8){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**USB    ";
    } else if (nr == 9){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**12v    ";
    } else if (nr == 10){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**230    ";
    } else if (nr == 11){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**ST_AUTO";
    } else if (nr == 12){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**TV     ";
    } else if (nr == 13){
      io_knoppen[8] = nr;
      io_objecten[nr] = 0;
      io_namen[nr] = "?";
    } else if (nr == 14){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_MOTOR";
    } else if (nr == 15){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_WAL  ";
    }
  }
}

void io_set_4(){
  /* Basis setup voor de test module en demo met 1 module nog om te schrijven naar escaperoom
   * De volgende Dingen kunnen worden aangesloten:
   *
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 8;
  io_knoppen_cnt = 8;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 7){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    }
  }
}

void io_set_5(){
  /* Setup voor de test/demo met 2 modulen nog om te schrijven naar 2 modulen escaperoom
   * Uitgangspunt is een uitbreiding op bovenstaande
   * De eerste module kan worden vervangen door een module met kapote lamp detectie
   * De tweede module niet omdat dit de mogelijkheid van input blokkeert.
   * De volgende Dingen kunnen worden aangesloten:
   *
   MODULE 1:
   1: Witte verlichting
   2: Rode verlichting
   3: Heklicht (wit licht achterop de boot)
   4: Ankerlicht (wit licht bovenin de mast)
   5: Navigatie verlichting (rood licht links en groen licht rechts)
   6: 3 kleuren toplicht
   7: Deklicht (licht dat vanaf de mast het voordek verlicht)
   8: Stoomlicht (Witlicht in de mast dat naar voren schijnt)
   *
   MODULE 2:
   1: USB stopcontacten
   2: 12 volt stopcontacten
   3: 230 volt omvormer
   4: Stuurautomaat
   5: TV
   6: ?
   7: Motor actief (signaal lampje dynamo)
   8: Walstroom actief (kleine 12 volt voeding op walstroom)
   *
   */
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 16;
  io_knoppen_cnt = 13;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < cnt; nr++){
    io_objecten[nr] = 0;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_namen[nr] = "?         ";

    if (nr < 3) {
      io_objecten[nr] = 3;
      io_knoppen[nr] = nr;
      
    }
    if (nr == 0){
      io_namen[nr] = "**IL_wit ";
    } else if (nr == 1) {
      io_namen[nr] = "**IL_rood";
    } else if (nr == 2){
      io_knoppen[3] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_hek  ";
    } else if (nr == 3){
      io_knoppen[4] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_anker";
    } else if (nr == 4){
      io_knoppen[5] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_navi ";
    } else if (nr == 5){
      io_knoppen[6] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_3kl  ";
    } else if (nr == 6){
      io_knoppen[7] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**E_dek  ";
    } else if (nr == 7){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**L_stoom";
    } else if (nr == 8){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**USB    ";
    } else if (nr == 9){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**12v    ";
    } else if (nr == 10){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**230    ";
    } else if (nr == 11){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**ST_AUTO";
    } else if (nr == 12){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**TV     ";
    } else if (nr == 13){
      io_knoppen[8] = nr;
      io_objecten[nr] = 0;
      io_namen[nr] = "?";
    } else if (nr == 14){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_MOTOR";
    } else if (nr == 15){
      io_knoppen[8] = nr;
      io_objecten[nr] = 3;
      io_namen[nr] = "**S_WAL  ";
    }
  }
}

void io_set_6(){
  delete[]io_objecten;
  delete[]io_object_ruimte;
  delete[]io_output;
  delete[]io_input;
  delete[]io_gewijzigd;
  delete[]io_open_alert;
  delete[]io_namen;
  delete[]io_sd;
  delete[]io_events;
  
  int cnt = 8;
  io_knoppen_cnt = 8;
  io_cnt = cnt;
  io_objecten = new byte[cnt];
  io_object_ruimte = new byte[cnt];
  io_output = new byte[cnt];
  io_input = new bool[cnt];
  io_gewijzigd = new bool[cnt];
  io_open_alert = new bool[cnt];
  io_alert = new byte[cnt];
  io_namen = new char*[cnt];
  io_knoppen = new int[io_knoppen_cnt];
  
  for (int i = 0 ; i < io_cnt ; i++) {
    io_namen[i] = new char[11];
  }
  io_events = new byte***[io_cnt];
  for (int i = 0; i < io_cnt; i++) {
    io_object_ruimte[i] = 255;
    io_events[i] = new byte**[2];
    for (int j = 0; j < 2; j++) {
      io_events[i][j] = new byte*[2];
      for (int k = 0; k < 2; k++) {
        io_events[i][j][k] = new byte[10];
        for (int l = 0; l < 10; l++) {
          io_events[i][j][k][l] = 255;
        }
      }
    }
  }
  
  for (int nr = 0; nr < 8; nr++) {
    io_objecten[nr] = 3;
    io_output[nr] = 0;
    io_input[nr] = 0;
    io_alert[nr] = 3;
    io_knoppen[nr] = nr;
    // io_namen[nr] = "?         ";
  }
  io_namen[0] = "**TB_rood";
  io_namen[1] = "**TB_wit ";
  io_namen[2] = "**TB_blau";
  io_namen[3] = "**TB_groe";
  io_namen[4] = "**TB_rood";
  io_namen[5] = "**TB_geel";
  io_namen[6] = "**TB_wit ";
  io_namen[7] = "**TB_blau";

}
#ifdef ESP32
// BKOS5a: NVS IO state persistentie
Preferences _io_prefs;
bool io_state_dirty = false;
unsigned long io_state_last_write = 0;

void io_state_opslaan() {
  if (!io_state_dirty) return;
  _io_prefs.begin("io_state", false);
  _io_prefs.putBytes("out", io_output, io_cnt * sizeof(byte));
  _io_prefs.end();
  io_state_dirty = false;
  io_state_last_write = millis();
}

void io_state_laden() {
  _io_prefs.begin("io_state", true);
  size_t opgeslagen = _io_prefs.getBytesLength("out");
  if (opgeslagen == io_cnt * sizeof(byte)) {
    _io_prefs.getBytes("out", io_output, io_cnt * sizeof(byte));
  }
  _io_prefs.end();
}

void io_state_check() {
  if (io_state_dirty && (millis() - io_state_last_write >= IO_WRITE_DELAY_MS)) {
    io_state_opslaan();
  }
}
#endif
