#include <esp_now.h>
#include <WiFi.h>
#include "SdsDustSensor.h"

HardwareSerial port(2);

SdsDustSensor sds(Serial2);
#define RXD2 16
#define TXD2 17

float twoFive;
float ten;

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress[] = {0x7C, 0x9E, 0xBD, 0x61, 0x91, 0xC0};

typedef struct test_struct6 {
  float a;
  float b;
} test_struct6;

test_struct6 test;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {

  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    twoFive = pm.pm25;
    ten = pm.pm10;
    Serial.println(pm.toString());
  } else {
  }


  test.a = twoFive;
  test.b = ten;

  esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct6));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(500);

}
