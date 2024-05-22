
#include "network.h"

WiFiUDP Udp;

void wifiConnectionInit() {
  #if defined(WIFI_SSID) && defined(WIFI_PASSWD)
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  #else
    #error "Missing defines WIFI_SSID and WIFI_PASSWD"
  #endif
 
  // Wait for connection  
  Serial.printf("[WIFI connection init] Connecting to %s ", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {   
    digitalWrite(2, LOW);
    Serial.print(".");
    delay(500);
    digitalWrite(2, HIGH);
  }
  Serial.println();
  Serial.println("[WIFI connection init] connected");
  Serial.print("[WIFI connection init] IP address: ");
  Serial.println(WiFi.localIP());
}

void mdnsInit() {
  // Start the mDNS responder for patbandvr.local
  if (!MDNS.begin("patbandvr")) {
    Serial.println("[MDNS init] Error setting up MDNS responder!");
  }
  // MDNS.addService("http", "udp", 4210);
  Serial.println("[MDNS init] mDNS responder started");
}

int readIncomingData () {
  if (Udp.parsePacket()) {
    // receive incoming UDP packets
    int incomingByte = Udp.read();
    if (incomingByte) {
      return incomingByte;
    }
  }
  return 0;
}

