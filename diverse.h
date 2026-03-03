// BKOS5a: Touch debounce constante
#define TOUCH_DEBOUNCE_MS 200UL

// BKOS5a: Schermtimer waarschuwing variabele
bool scherm_waarschuwing_actief = false;

// BKOS5a: Laatste touch tijdstip voor debounce
unsigned long laatste_touch_tijd = 0;

#include "div_ellipse.h"
#include "div_icon.h"
#include "div_teken.h"
#include "div_schermacties.h"
#include "div_tijd.h"
#include "div_status.h"
#include "div_QR.h"