//import libraries
#include <esp_now.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>


//EDIT THESE VALUES

const char* ssid = "SAP 2.4";
const char* password = "37928629";

float timeBetweenDataLog = 12;


#define CONSOLE_IP "192.168.4.2"
#define CONSOLE_PORT 4210
WiFiUDP Udp;
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);
#include <WiFiClientSecure.h>

//https://script.google.com/macros/s/AKfycbzRJZi91wcq7vCb6Pgjyq-K9KW_Ms0ZCNEVs8RjEr3AY2LOAJyYogu3F6U7b-UtEjQN/exec

const char* host = "script.google.com";
const int httpsPort = 443;
//WiFiClientSecure client;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbzRJZi91wcq7vCb6Pgjyq-K9KW_Ms0ZCNEVs8RjEr3AY2LOAJyYogu3F6U7b-UtEjQN";  // Replace by your GAS service id

float twoFive;
float ten;
float s1p25 = 0;
float s1p10 = 0;
float s2p25 = 0;
float s2p10 = 0;
float s3p25 = 0;
float s3p10 = 0;
float s4p25 = 0;
float s4p10 = 0;
float s5p25 = 0;
float s5p10 = 0;
float s6p25 = 0;
float s6p10 = 0;

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
typedef struct test_struct2 { ///COMPANION INPUT2
  float a;
  float b;
} test_struct2;
test_struct2 myData2;
typedef struct test_struct3 { ///COMPANION INPUT3
  float a;
  float b;
} test_struct3;
test_struct3 myData3;
typedef struct test_struct4 { ///COMPANION INPUT4
  float a;
  float b;
} test_struct4;
test_struct4 myData4;
typedef struct test_struct5 { ///COMPANION INPUT5
  float a;
  float b;
} test_struct5;
test_struct5 myData5;
typedef struct test_struct6 { ///COMPANION INPUT6
  float a;
  float b;
} test_struct6;
test_struct6 myData6;


void setup() {
  //  Serial.begin(9600);
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  initWiFi();

  if (esp_now_init() != ESP_OK) {
    //    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  server.begin();

  //  makeIFTTTRequest();
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

unsigned long balls = 0;
unsigned long lastBall = 0;

void loop() {
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    twoFive = pm.pm25;
    ten = pm.pm10;
    //    Serial.println(pm.toString());
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

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S1-PM2.5: ");
  Udp.printf(String(s1p25).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S1-PM10: ");
  Udp.printf(String(s1p10).c_str());
  Udp.endPacket();

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S2-PM2.5: ");
  Udp.printf(String(s2p25).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S2-PM10: ");
  Udp.printf(String(s2p10).c_str());
  Udp.endPacket();

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S3-PM2.5: ");
  Udp.printf(String(s3p25).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S3-PM10: ");
  Udp.printf(String(s3p10).c_str());
  Udp.endPacket();

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S4-PM2.5: ");
  Udp.printf(String(s4p25).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S4-PM10: ");
  Udp.printf(String(s4p10).c_str());
  Udp.endPacket();

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S5-PM2.5: ");
  Udp.printf(String(s5p25).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S5-PM10: ");
  Udp.printf(String(s5p10).c_str());
  Udp.endPacket();

  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S6-PM2.5: ");
  Udp.printf(String(s6p25).c_str());
  Udp.endPacket();
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.printf("S6-PM10: ");
  Udp.printf(String(s6p10).c_str());
  Udp.endPacket();



  if ( millis() - lastBall > timeBetweenDataLog * 1000)
  {

    lastBall = millis();
    sendData(twoFive, ten, s1p25, s1p10, s2p25, s2p10, s3p25, s3p10, s4p25, s4p10, s5p25, s5p10, s6p25, s6p10);
    //    makeIFTTTRequest();
  }

  delay(500);
}


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData1, incomingData, sizeof(myData1));
  s1p25 = myData1.a;
  s1p10 = myData1.b;
  memcpy(&myData2, incomingData, sizeof(myData2));
  s2p25 = myData2.a;
  s2p10 = myData2.b;
  memcpy(&myData3, incomingData, sizeof(myData3));
  s3p25 = myData3.a;
  s3p10 = myData3.b;
  memcpy(&myData4, incomingData, sizeof(myData4));
  s4p25 = myData4.a;
  s4p10 = myData4.b;
  memcpy(&myData5, incomingData, sizeof(myData5));
  s5p25 = myData5.a;
  s5p10 = myData5.b;
  memcpy(&myData6, incomingData, sizeof(myData6));
  s6p25 = myData6.a;
  s6p10 = myData6.b;
}


void sendData(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6 )
{
  WiFiClientSecure client;
  client.setInsecure();
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
  String string_x0     =  String(x0, DEC);
  String string_y0     =  String(y0, DEC);
  String string_x1     =  String(x1, DEC);
  String string_y1     =  String(y1, DEC);
  String string_x2     =  String(x2, DEC);
  String string_y2     =  String(y2, DEC);
  String string_x3     =  String(x3, DEC);
  String string_y3     =  String(y3, DEC);
  String string_x4     =  String(x4, DEC);
  String string_y4     =  String(y4, DEC);
  String string_x5     =  String(x5, DEC);
  String string_y5     =  String(y5, DEC);
  String string_x6     =  String(x6, DEC);
  String string_y6     =  String(y6, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?mp25=" + string_x0 + "&mp10=" + string_y0 +
               "/exec?s1p25=" + string_x1 + "&s1p10=" + string_y1 +
               "/exec?s2p25=" + string_x2 + "&s2p10=" + string_y2 +
               "/exec?s3p25=" + string_x3 + "&s3p10=" + string_y3 +
               "/exec?s4p25=" + string_x4 + "&s4p10=" + string_y4 +
               "/exec?s5p25=" + string_x5 + "&s5p10=" + string_y5 +
               "/exec?s6p25=" + string_x6 + "&s6p10=" + string_y6;
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}


