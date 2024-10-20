#include "ESP32_NOW.h"
#include "WiFi.h"
#include "SdsDustSensor.h"

// EDIT DETAILS
#define BOARD_ID 1
const long INTERVAL = 500;  // Interval at which to publish sensor readings

#define ESPNOW_WIFI_CHANNEL 6

#define RXD2 16
#define TXD2 17

SdsDustSensor sds(Serial2);

float twoFive;
float ten;

class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer {
public:
  // Constructor of the class using the broadcast address
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk)
    : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Broadcast_Peer() {
    remove();
  }

  // Function to properly initialize the ESP-NOW and register the broadcast peer
  bool begin() {
    if (!ESP_NOW.begin() || !add()) {
      log_e("Failed to initialize ESP-NOW or register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to send a message to all devices within the network
  bool send_message(const uint8_t *data, size_t len) {
    if (!send(data, len)) {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};

// Structure to send data
// Must match the receiver structure
typedef struct struct_message {
  int id;
  float pm25;
  float pm10;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0;  // Stores last time data was published

uint32_t msg_count = 0;

// Create a broadcast peer object
ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.println("Starting setup...");

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial2 initialized");

  sds.begin();
  Serial.println("SDS sensor initialized");
  Serial.println(sds.setActiveReportingMode().toString());
  Serial.println(sds.setContinuousWorkingPeriod().toString());

  /// Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Register the broadcast peer
  if (!broadcast_peer.begin()) {
    Serial.println("Failed to initialize broadcast peer");
    Serial.println("Reebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Setup complete. Broadcasting messages every 5 seconds.");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;

    myData.id = BOARD_ID;
    PmResult pm = sds.readPm();
    if (pm.isOk()) {
      myData.pm25 = pm.pm25;
      myData.pm10 = pm.pm10;
      Serial.println(pm.toString());
    } else {
      Serial.println("Could not read values from sensor");
      myData.pm25 = 0;  // Use a sentinel value to indicate read error
      myData.pm10 = 0;
    }


    Serial.printf("Broadcasting message: %s\n", (uint8_t *)&myData);

    if (!broadcast_peer.send_message((uint8_t *)&myData, sizeof(myData))) {
      Serial.println("Failed to broadcast message");
    }

  }
}