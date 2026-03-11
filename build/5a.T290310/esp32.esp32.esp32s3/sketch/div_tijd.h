#line 1 "/a0/usr/workdir/BKOS5a/div_tijd.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

String tijd();
String tijd(String wat);
long millis_wekker(int H, int M);
long millis_timer(int H, int M, int S);
String tijd_tot_millis(long invoer);

String dagnm;
int dag;
String maand;
int jaar;
int uur;
int minuut;
int seconde;
String tijdzone;
long tijd_millis;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "nl.pool.ntp.org", 3600, 3600);