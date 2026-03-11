#line 1 "/a0/usr/workdir/BKOS5a/io_instellingen.h"
void naar_io_instellingen();
void start_io_instellingen();
void bouw_io_instellingen ();
void bouw_io_instellingen_knoppen();
void run_io_instellingen();


int io_sd_cnt;
String* io_sd;

int instellingen_io_knoppen_cnt = 15;
int instellingen_io_knoppen_reeks = 0;
int instellingen_io_knoppen_reeks_cnt;

                                           // Aantal pinnen         reserve
int instellingen_io_knoppen_positie[15][4] = {{10, 30, 105, 40}, {125, 30, 105, 40},

                          // knopjes voor 9 pinnen. Nog even bedenken of knop 9 passief is, of voor de 9e io
                                      {10, 80, 70, 40}, {85, 80, 70, 40}, {160, 80, 70, 40},
                                    {10, 130, 70, 40}, {85, 130, 70, 40}, {160, 130, 70, 40},
                                    {10, 180, 70, 40}, {85, 180, 70, 40}, {160, 180, 70, 40},

                          //  esc                   <                    >                enter
                          {10, 230, 50, 40}, {68, 230, 50, 40}, {123, 230, 50, 40}, {180, 230, 50, 40},
};

uint16_t instellingen_io_knoppen_kleur[6]      = {tft.color565(120, 120, 120), tft.color565(0, 255, 0), tft.color565(0, 255, 255), tft.color565(255, 0, 0), tft.color565(255, 255, 255), tft.color565(30, 30, 30)};
uint16_t instellingen_io_knoppen_tekstkleur[6] = {tft.color565(255, 255, 255), tft.color565(0, 0, 0), tft.color565(0, 0, 0), tft.color565(255, 255, 255), tft.color565(0, 0, 0), tft.color565(45, 45, 45)};

char instellingen_io_knoppen_tekst[15][10] = {"", "",
                                            "", "", "",
                                            "", "", "",
                                            "", "", "",
                                        "esc", "<", ">", "opslaan"
};

char instellingen_io_knoppen_tekst2[15][10] = {"poorten","",
                                                "","","",
                                                "","","",
                                                "","","",
                                               "","","",""
};

byte io_instellingen_knoppen_status[15] = {4, 5,
                                         5, 5, 5,
                                         5, 5, 5,
                                         5, 5, 5,
                                        3, 5, 5, 1
};
