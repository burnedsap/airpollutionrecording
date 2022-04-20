#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "SdsDustSensor.h"
#include <FastLED.h>
#define NUM_LEDS 300
#define DATA_PIN 14
#define BRIGHTNESS  128
CRGB leds[NUM_LEDS];

//EDIT DETAILS
#define BOARD_ID 1

// Insert your SSID
constexpr char WIFI_SSID[] = "SSID";

const long interval = 500;  // Interval at which to publish sensor readings

//MAC Address of the receiver
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x8E, 0xF2, 0x94};

HardwareSerial port(2);

SdsDustSensor sds(Serial2);
#define RXD2 16
#define TXD2 17

float twoFive;
float ten;


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
  delay(3000); // startup delay to protect LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(  BRIGHTNESS );
  //Init Serial Monitor
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
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
    CHSV color1 = CHSV(map(ten, 30, 300, 1, 255), map(twoFive, 30, 300, 1, 255), 150);
    fill_solid(leds, NUM_LEDS, color1);
    FastLED.show();

    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
}
