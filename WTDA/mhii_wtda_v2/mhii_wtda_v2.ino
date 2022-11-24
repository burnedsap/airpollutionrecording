//import libraries
#include <esp_now.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>

#define CONSOLE_IP "192.168.4.2"
#define CONSOLE_PORT 4210
const char* ssid = "ESP32";
const char* password = "12345678";
WiFiUDP Udp;
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

float twoFive;
float ten;


#include "SdsDustSensor.h"
HardwareSerial port(2);

SdsDustSensor sds(Serial2);
#define RXD2 16
#define TXD2 17

typedef struct test_struct1 { ///COMPANION INPUT1
  float a;
  float b;
} test_struct1;
test_struct1 myData1;
typedef struct test_struct2 { ///COMPANION INPUT1
  float a;
  float b;
} test_struct2;
test_struct2 myData2;

void setup() {
  //  Serial.begin(9600);
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended

  WiFi.mode(WIFI_AP_STA);

  if (esp_now_init() != ESP_OK) {
    //    Serial.println("Error initializing ESP-NOW");
    return;
  }

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    twoFive = pm.pm25;
    ten = pm.pm10;
    Serial.println(pm.toString());
  } else {
//    twoFive = random(100);
//    ten = random(100);
//    Serial.println(twoFive);
//    Serial.println(" ");
//    Serial.println(ten);
    //    Serial.print("Could not read values from sensor, reason: ");
//    Serial.println(pm.statusToString());
  }


  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("M-PM2.5: ");
  Udp.printf(String(twoFive).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("M-PM10: ");
  Udp.printf(String(ten).c_str());
  Udp.endPacket();

  delay(500);
}
