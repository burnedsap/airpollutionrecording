#include <esp_now.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "SdsDustSensor.h"
#include <WebServer.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include "FastLED.h"
HardwareSerial port(2);
SdsDustSensor sds(Serial2);



// EDIT DETAILS
const char* ssid = "SSID";
const char* password = "PASSWORD";
float timeBetweenDataLog = 10;
#define CONSOLE_IP "192.168.0.206" //enter local ip address of computer using Processing

#define LED_PIN     14
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    300
#define BRIGHTNESS  255
CRGB leds[NUM_LEDS];

short bright = 255;
int ledK;
int fadeAmount = -1;
uint8_t blendRate = 50 * 2; // How fast to blend.  Higher is slower.  [milliseconds]
const long interval = 500;
unsigned long previousMillis = 0;   // Stores last time temperature was published
unsigned long balls = 0;
unsigned long lastBall = 0;
CHSV colorStart = CHSV(160, 255, 255); // starting color
CHSV colorTarget = CHSV(0, 255, 255); // target color
CHSV colorCurrent = colorStart;
#define CONSOLE_PORT 4210
WiFiUDP Udp;
WebServer server(80);
#define RXD2 16
#define TXD2 17
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float x;
  float y;
} struct_message;
struct_message myData;

struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;
struct_message board5;
struct_message board6;

struct_message boardsStruct[6] = {board1, board2, board3, board4, board5, board6};


void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  //  boardsStruct[myData.id - 1].id = myData.id;
  boardsStruct[myData.id - 1].x = myData.x;
  boardsStruct[myData.id - 1].y = myData.y;
  Serial.printf("PM 2.5 value: %4.2f \n", boardsStruct[myData.id - 1].x);
  Serial.printf("PM 10 value: %4.2f \n", boardsStruct[myData.id - 1].y);
  Serial.println();
}


//https://script.google.com/macros/s/AKfycbxwUHKYi-M8Gxr_2bpFzNQemrFGa4uMcBiqfrL68SWPsEEfW3jNVoxgbKkzNpJ0IFjb/exec

const char* host = "script.google.com";
const int httpsPort = 443;
//WiFiClientSecure client;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "AKfycbxwUHKYi-M8Gxr_2bpFzNQemrFGa4uMcBiqfrL68SWPsEEfW3jNVoxgbKkzNpJ0IFjb";  // Replace by your GAS service id

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


void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  sds.begin();

  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);

  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  server.begin();
}

void loop() {

  float s1p25 = boardsStruct[0].x;
  float s1p10 = boardsStruct[0].y;
  float s2p25 = boardsStruct[1].x;
  float s2p10 = boardsStruct[1].y;
  float s3p25 = boardsStruct[2].x;
  float s3p10 = boardsStruct[2].y;
  float s4p25 = boardsStruct[3].x;
  float s4p10 = boardsStruct[3].y;
  float s5p25 = boardsStruct[4].x;
  float s5p10 = boardsStruct[4].y;
  float s6p25 = boardsStruct[5].x;
  float s6p10 = boardsStruct[5].y;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    PmResult pm = sds.readPm();
    if (pm.isOk()) {
      twoFive = pm.pm25;
      ten = pm.pm10;
      //    Serial.println(pm.toString());
    } else {
      //    twoFive = random(100);
      //    ten = random(100);
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
  }
  if ( millis() - lastBall > timeBetweenDataLog * 1000)
  {
    lastBall = millis();
    Serial.println("PM 2.5: " + String(s1p25));
    Serial.println("PM 10: " + String(s1p10));
    sendData(twoFive, ten, s1p25, s1p10, s2p25, s2p10, s3p25, s3p10, s4p25, s4p10, s5p25, s5p10, s6p25, s6p10);
  }

  if (bright < 100) {
    fadeAmount = 2;
  }
  if (bright > 254) {
    fadeAmount = -2;
  }

  bright += fadeAmount;

  ledK = map(ten, 100, 200, 0, 255);

  EVERY_N_MILLISECONDS(blendRate) {
    static uint8_t k;  // the amount to blend [0-255]
    colorCurrent = blend(colorStart, colorTarget, k, SHORTEST_HUES);
    fill_solid( leds, NUM_LEDS, colorCurrent );
    leds[0] = colorTarget;  // set first pixel to always show target color
    //    Serial.print("colorCurrent:\t"); Serial.print(colorCurrent.h); Serial.print("\t");
    //    Serial.print("colorTarget:\t"); Serial.print(colorTarget.h);
    Serial.print("\tk: "); Serial.print(k);
    Serial.print("\tbright: "); Serial.print(bright);
    //    Serial.print("\tb: "); Serial.println(FastLED.getBrightness());
    k  = ten;
  }
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();

  delay(500);
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
               "&s1p25=" + string_x1 + "&s1p10=" + string_y1 +
               "&s2p25=" + string_x2 + "&s2p10=" + string_y2 +
               "&s3p25=" + string_x3 + "&s3p10=" + string_y3 +
               "&s4p25=" + string_x4 + "&s4p10=" + string_y4 +
               "&s5p25=" + string_x5 + "&s5p10=" + string_y5 +
               "&s6p25=" + string_x6 + "&s6p10=" + string_y6;
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
