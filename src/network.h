#ifndef HEADPATVR_NETWORK_MANAGER
#define HEADPATVR_NETWORK_MANAGER

#include <ESPmDNS.h> 
#include <WebServer.h>
#include <WiFiUdp.h>


extern WiFiUDP Udp;

void wifiConnectionInit();
void mdnsInit();
int readIncomingData();

#endif  // HEADPATVR_NETWORK_MANAGER
