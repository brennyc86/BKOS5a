#line 1 "/a0/usr/workdir/BKOS5a/KNOPPEN.h"
int aantal_knoppen = 0;

int ** knoppen_positie;
byte * knoppen_status;
int ** knoppen_actie;
int ** knoppen_teken_positie;
uint16_t ** knoppen_basiskleur;
int ** knoppen_tekst_positie;
uint16_t ** knoppen_tekst_kleur;
char ** knoppen_tekst;
char ** knoppen_tekst2;



void knop_plaatsen(int knop_nummer);
void knop_plaatsen(int knop_nummer, bool tekst2);
void knop_plaatsen(int knop_nummer, bool tekst, uint16_t basis_kleur);

void homeknop_plaatsen();
void halve_homeknop_plaatsen();
void halve_2eknop_plaatsen(String tekst);
void homeknop_plaatsen(bool invert);

void alle_knoppen_plaatsen();
void alle_knoppen_plaatsen(bool tekst2);
int klik(int x, int y);
void knop_feedback(int knop_nummer); // BKOS5a: visuele touch feedback
