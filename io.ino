void io_boot() {
  io_setup();  // Setup specifiek voor de computer
  io_detect(); // Detecteren hardware op basis van de computer
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