//import libraries
#include <WebServer.h>
#include <WiFi.h>
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

void setup() {
  //  Serial.begin(9600);
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    //    Serial.print("PM2.5 = ");
    twoFive = pm.pm25;
    //    Serial.print(pm.pm25);
    //    Serial.print(", PM10 = ");
    ten = pm.pm10;
    //    Serial.println(pm.pm10);
    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
  } else {
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
