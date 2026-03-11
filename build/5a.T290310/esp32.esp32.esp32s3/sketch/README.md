#line 1 "/a0/usr/workdir/BKOS5a/README.md"
Englisch version below

# NL

## LETOP ! ! ! Deze versie van BKOS is nog lang niet af en zal daarom slechts beperkt functioneren.

Doel van deze nieuwe versie is naast de Raspberry Pi Pico controllers ook de ESP32 als microcontroller te kunnen gebruiken, en ook de CYD (ESP32-2432S028R) of de grotere versies daarvan de ESP32-8048S043C en ESP34-8048S070C

De software is nog vrij bassal en draait enkel als stand alone. Functionaliteit dat ik nog moet toevoegen (of bezig ben met toevoegen) zijn:
- IO aansturing
- SD kaart
- WiFi

## Instructies
Kies uw Microcontroller met een cijfer bij HARDWARE
0 & 1 (RP Pico) geschikt voor PCB versies 0.2 en hoger
3     (ESP 32)  geschikt voor PCB versies 0.4 en hoger, kiezen voor ESP32 Dev Module
2     (CYD)     Kiezen voor ESP32_WROOM DA Module
4 & 5           Kiezen voor ESP32S3 Dev Module, PSRAM -> OPI PSRAM, Flash Size -> 16 MB (128 Mb)

Kies een resulutie bij RESULUTIE voor liggend in 10 pixels BBHH
Dit moet de resolutie van het gebruikte scherm zijn, hier wordt naartoe geschaald

Kies een orientie bij ORIENTATIE
1 = Staand
0 = Liggend
Liggend werkt nog niet op alle schermen en is ook nu nog met name een uitgerekte versie van staand, maar hier wordt aan gewerkt


ESP-eSPI  (met aanpassingen afhankelijk van de MCU en scherm)




# EN

## ATENTION ! ! ! This version of BKOS is not compleet and functionated partely

Goal of this new version is to let it not only function at a Raspberry Pi Pico MCU, but also at the ESP32 MCU's. For now it is (with version 0.4 of my PCB) the 38 pins ESP32 dev board, but also the CYD (ESP32-2432S028R) and the bigger versions of it, the ESP32-8048S043C and the ESP32-8048S070C

The software functionated only basicly for now as stand alone play and test with your screen. Functionality I have to add (ore I'm working right now to add) are:
- IO capability
- SD card
- WiFi


## Instructions
Select a MCU with a number by HARDWARE
0 & 1 (RP Pico) Works with PCB version 0.2 and higher
3     (ESP 32)  Works with PCB version 0.4 and higher, Select ESP32 Dev Module
2     (CYD)     Select ESP32_WROOM DA Module
4 & 5           Select ESP32S3 Dev Module, PSRAM -> OPI PSRAM, Flash Size -> 16 MB (128 Mb)

Choice a resulotion by RESULUTIE for landscape in 10 pixels WWHH
This have to be the resolution of the screen. Everything will scale to this resolution

Choice a orientation by ORIENTATIE
1 = Portret
0 = Landscape
Landscape will not work for everything


ESP-eSPI  (met aanpassingen afhankelijk van de MCU en scherm)
