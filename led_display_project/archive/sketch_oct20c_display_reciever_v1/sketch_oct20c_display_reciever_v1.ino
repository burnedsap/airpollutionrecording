#include <Wire.h>                  // For I2C communication
#include <Adafruit_Protomatter.h>  // For RGB matrix
#include "ESP32_NOW.h"
#include "WiFi.h"
#include <vector>

#include "good_1.h"
#include "good_2.h"
#include "mid_1.h"
#include "mid_2.h"
#include "bad_1.h"
#include "bad_2.h"

int level_1 = 5; // 0 - level_1 GOOD
int level_2 = 10; // level_1 to level_2 MID
// anything above level_2 BAD

#define ESPNOW_WIFI_CHANNEL 6

#define HEIGHT 32   // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH 64    // Matrix width (pixels)
#define MAX_FPS 45  // Maximum redraw rate, frames/second

#if defined(_VARIANT_MATRIXPORTAL_M4_)  // MatrixPortal M4
uint8_t rgbPins[] = { 7, 8, 9, 10, 11, 12 };
uint8_t addrPins[] = { 17, 18, 19, 20, 21 };
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;
#else  // MatrixPortal ESP32-S3
uint8_t rgbPins[] = { 42, 41, 40, 38, 39, 37 };
uint8_t addrPins[] = { 45, 36, 48, 35, 21 };
uint8_t clockPin = 2;
uint8_t latchPin = 47;
uint8_t oePin = 14;
#endif


#if HEIGHT == 16
#define NUM_ADDR_PINS 3
#elif HEIGHT == 32
#define NUM_ADDR_PINS 4
#elif HEIGHT == 64
#define NUM_ADDR_PINS 5
#endif

// Structure to receive data
typedef struct struct_message {
  int id;
  float pm25;
  float pm10;
} struct_message;

struct_message myData;

Adafruit_Protomatter matrix(
  WIDTH, 4, 1, rgbPins, NUM_ADDR_PINS, addrPins,
  clockPin, latchPin, oePin, true);  //could change 4 to 6


#define N_COLORS 8
#define BOX_HEIGHT 8
#define N_GRAINS (BOX_HEIGHT * N_COLORS * 8)
uint16_t colors[N_COLORS];


uint32_t prevTime = 0;  // Used for frames-per-second throttle

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void err(int x) {
  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);        // Using onboard LED
  for (i = 1;; i++) {                  // Loop forever...
    digitalWrite(LED_BUILTIN, i & 1);  // LED on/off blink to alert user
    delay(x);
  }
}
const uint16_t *frames[] = { good_1, good_2, mid_1, mid_2, bad_1, bad_2 };

class ESP_NOW_Peer_Class : public ESP_NOW_Peer {
public:
  // Constructor of the class
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk)
    : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  // Destructor of the class
  ~ESP_NOW_Peer_Class() {}

  // Function to register the master peer
  bool add_peer() {
    if (!add()) {
      log_e("Failed to register the broadcast peer");
      return false;
    }
    return true;
  }

  // Function to print the received messages from the master
  void onReceive(const uint8_t *data, size_t len, bool broadcast) {
    Serial.printf("Received a message from master " MACSTR " (%s)\n", MAC2STR(addr()), broadcast ? "broadcast" : "unicast");
    memcpy(&myData, data, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("PM2.5: ");
    Serial.println(myData.pm25);
    Serial.print("PM10: ");
    Serial.println(myData.pm10);
    displayFrames(myData.pm25);
  }
};
// List of all the masters. It will be populated when a new master is registered
std::vector<ESP_NOW_Peer_Class> masters;

void register_new_master(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) {
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) {
    Serial.printf("Unknown peer " MACSTR " sent a broadcast message\n", MAC2STR(info->src_addr));
    Serial.println("Registering the peer as a master");

    ESP_NOW_Peer_Class new_master(info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);

    masters.push_back(new_master);
    if (!masters.back().add_peer()) {
      Serial.println("Failed to register the new master");
      return;
    }
  } else {
    // The slave will only receive broadcast messages
    log_v("Received a unicast message from " MACSTR, MAC2STR(info->src_addr));
    log_v("Igorning the message");
  }
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello, I'm alive");

  // Initialize the Wi-Fi module
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Slave");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  // Initialize the ESP-NOW protocol
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Reeboting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  // Register the new peer callback
  ESP_NOW.onNewPeer(register_new_master, NULL);

  Serial.println("Setup complete. Waiting for a master to broadcast a message...");


  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK) {
    for (;;)
      ;
  }
  matrix.fillScreen(matrix.color565(0, 0, 0));
  matrix.show();
}


void loop() {
  // Serial.println("Hello, I'm alive");
}

void displayFrames(float pm25) {
  int frameStart;
  if (pm25 <= level_1) {
    frameStart = 0;  // Index for frame1
  } else if (pm25 <= level_2) {
    frameStart = 2;  // Index for frame3
  } else {
    frameStart = 4;  // Index for frame5
  }

  // Display frames for about 2.5 seconds at 2 FPS
  for (int i = 0; i < 5; i++) {
    displayFrame(frameStart);
    delay(600);
    displayFrame(frameStart + 1);
    delay(600);
  }
}

void displayFrame(int frameIndex) {
  const uint16_t *frameData = frames[frameIndex];

  for (int y = 0; y < matrix.height(); y++) {
    for (int x = 0; x < matrix.width(); x++) {
      uint16_t color = pgm_read_word(&frameData[y * matrix.width() + x]);
      matrix.drawPixel(x, y, color);
    }
  }
  matrix.show();
}