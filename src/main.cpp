#include <Servo.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// wifi connection vars definition
WiFiUDP Udp;
int incomingByte;

// servo vars definition
const int SERVO_ROTATION_LIMIT = 120;
Servo servoLeft;
Servo servoRight;
int pos = 0;
int direction = 1;
int speed_level = 0;
// int lpos = 0;
// int ldirection = 1;
// int rpos = 0;
// int rdirection = 1;
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
void moveServosSimplified();


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
    moveServosSimplified();
  }
}



// setup functions
void wifiConnectionInit() {
  #if defined(WIFI_SSID) && defined(WIFI_PASSWD)
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  #else
    #error "Missing defines WIFI_SSID and WIFI_PASSWD"
  #endif
 
  // Wait for connection  
  Serial.printf("[WIFI connection init] Connecting to %s ", WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {   
    digitalWrite(INTERNAL_LED, LOW);
    Serial.print(".");
    delay(500);
    digitalWrite(INTERNAL_LED, HIGH);
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
  MDNS.addService("http", "tcp", 80);
  Serial.println("[MDNS init] mDNS responder started");
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

void moveServosSimplified () {
  // move servos with the same spped
  // speed - max speed from l and r speeds
  speed_level = max(left_speed_level, right_speed_level);
  for (int i = 10; i<= SERVO_ROTATION_LIMIT; i+= 10) {
    pos = (SERVO_ROTATION_LIMIT * (1-direction)/2) + i * direction;
    servoLeft.write(pos);
    servoRight.write(pos);
    delay(map(speed_level, 0, 10, 55, 15));
  }
  direction *= (-1);
}
