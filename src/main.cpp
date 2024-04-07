#include <Servo.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// wifi connection vars definition
WiFiUDP Udp;
#define SERVER_PORT = 4210
char incomingPacket;
int incomingByte;

// servo vars definition
Servo servoLeft;
Servo servoRight;
int pos = 0;
static unsigned int left_speed_level = 0;
static unsigned int right_speed_level = 0;


// indicators definition
#define INTERNAL_LED D0 

// function declarations:
//   setup functions
void wifiConnectionInit();
void mdnsInit();
void servoInit();
//  loop functions
void blink();
int readIncomingData();
int getLeftSpeedLevel(int);
int getRightSpeedLevel(int);
void moveServos();

void setup() {
  Serial.begin(9600);
  wifiConnectionInit();
  Udp.begin(4210);
  mdnsInit();
  servoInit();
  blink();
}

void loop() {
  MDNS.update();
  incomingByte = readIncomingData();
  right_speed_level = getRightSpeedLevel(incomingByte);
  left_speed_level = getLeftSpeedLevel(incomingByte);
  if (right_speed_level || left_speed_level) {
    moveServos();
  }
}


// setup functions
void wifiConnectionInit() {
  #if defined(WIFI_CREDS_SSID) && defined(WIFI_CREDS_PASSWD)
    WiFi.begin(WIFI_CREDS_SSID, WIFI_CREDS_PASSWD);
  #else
    #error "Missing defines WIFI_CREDS_SSID and WIFI_CREDS_PASSWD"
  #endif
 
  // Wait for connection  
  Serial.printf("Connecting to %s ", WIFI_CREDS_SSID);
  while (WiFi.status() != WL_CONNECTED) {   
    digitalWrite(INTERNAL_LED, LOW);
    Serial.print(".");
    delay(500);
    digitalWrite(INTERNAL_LED, HIGH);
  }
  Serial.println(" connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void mdnsInit() {
  // Start the mDNS responder for patbandvr.local
  if (!MDNS.begin("patbandvr")) {
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started");
}

void servoInit(){
  servoLeft.attach(D4);
  servoRight.attach(D5);
  servoLeft.write(0);
  servoRight.write(0);
  delay(500);
}

// loop functions
void blink(){
  delay(500);
  digitalWrite(INTERNAL_LED, HIGH);
  delay(500);
  digitalWrite(INTERNAL_LED, LOW);
}

int readIncomingData () {
  if (Udp.parsePacket()) {
    // receive incoming UDP packets
    incomingByte = Udp.read();
    if (incomingByte) {
      Serial.println(incomingByte);
      return incomingByte;
    }
  }
  return 0;
}

int getLeftSpeedLevel(int incomingByte) {
  return incomingByte >> 4;
}

int getRightSpeedLevel (int incomingByte) {
  return incomingByte & 0x0F;
}

void moveServos() {
  for (pos = 0; pos <= 180; pos += 10) {
    if (left_speed_level > 0) {
      Serial.print("move left f: ");
      Serial.println(left_speed_level);
      servoLeft.write(pos);
    }
    if (right_speed_level > 0) {
      Serial.print("move righ f: ");
      Serial.println(right_speed_level);
      servoRight.write(pos);
    }
    // max delay - 55
    // min delay - 15
    delay(map(left_speed_level, 0, 10, 55, 15));
  }

  for (pos = 180; pos >= 0; pos -= 10) {
    if (left_speed_level > 0) {
      Serial.print("move left b: ");
      Serial.println(left_speed_level);
      servoLeft.write(pos);
    }
    if (right_speed_level > 0) {
      Serial.print("move righ b: ");
      Serial.println(right_speed_level);
      servoRight.write(pos);
    }
    delay(map(left_speed_level, 0, 10, 55, 15));
  }
}
