#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "SdsDustSensor.h"
#include "FastLED.h"

//EDIT DETAILS
#define BOARD_ID 1

// Insert your SSID
constexpr char WIFI_SSID[] = "SSID";

const long interval = 500;  // Interval at which to publish sensor readings

//MAC Address of the receiver
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x8E, 0xF2, 0x94};
#define LED_PIN     14
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    300
#define BRIGHTNESS  100
CRGB leds[NUM_LEDS];
HardwareSerial port(2);

SdsDustSensor sds(Serial2);
#define RXD2 16
#define TXD2 17

float twoFive;
float ten;
float val = 0;
float x;

uint8_t blendRate = 50;  // How fast to blend.  Higher is slower.  [milliseconds]

CHSV colorStart = CHSV(0, 255, 255); // starting color
CHSV colorTarget = CHSV(160, 255, 255); // target color
CHSV colorCurrent = colorStart;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  int id;
  float x;
  float y;
} struct_message;

//Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0;   // Stores last time temperature was published

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  //Init Serial Monitor
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(3000); // 3 second delay for recovery

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

  sds.begin();

  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended

  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  //Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    //Set values to send
    myData.id = BOARD_ID;

    PmResult pm = sds.readPm();
    if (pm.isOk()) {
      twoFive = pm.pm25;
      ten = pm.pm10;
      Serial.println(pm.toString());
    } else {
    }

    myData.x = twoFive;
    myData.y = ten;

    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
  x = abs(sin(val));
  EVERY_N_MILLISECONDS(blendRate) {
    static uint8_t k;  // the amount to blend [0-255]
    colorCurrent = blend(colorStart, colorTarget, k, SHORTEST_HUES);
    fill_solid( leds, NUM_LEDS, colorCurrent );
    leds[0] = colorTarget;  // set first pixel to always show target color
//    Serial.print("colorCurrent:\t"); Serial.print(colorCurrent.h); Serial.print("\t");
//    Serial.print("colorTarget:\t"); Serial.print(colorTarget.h);
//    Serial.print("\tk: "); Serial.print(k);
//    Serial.print("\tx: "); Serial.print(x);
//    Serial.print("\tb: "); Serial.println(FastLED.getBrightness());
    k  = ten;
  }
  val += 0.01;
  FastLED.setBrightness(20+x*235);
  FastLED.show();
}
