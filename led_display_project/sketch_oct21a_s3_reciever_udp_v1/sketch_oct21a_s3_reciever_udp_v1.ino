#include <Wire.h>
#include <Adafruit_Protomatter.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "good_1.h"
#include "good_2.h"
#include "mid_1.h"
#include "mid_2.h"
#include "bad_1.h"
#include "bad_2.h"

// WiFi credentials - must match ESP32 sender
const char* ssid = "ESP32_Matrix";
const char* password = "matrix123";
const int UDP_PORT = 4210;

// Matrix configuration
#define HEIGHT 32
#define WIDTH 64
#define MAX_FPS 45

// Pin configurations for Matrix Portal S3
uint8_t rgbPins[] = { 42, 41, 40, 38, 39, 37 };
uint8_t addrPins[] = { 45, 36, 48, 35, 21 };
uint8_t clockPin = 2;
uint8_t latchPin = 47;
uint8_t oePin = 14;

#if HEIGHT == 16
#define NUM_ADDR_PINS 3
#elif HEIGHT == 32
#define NUM_ADDR_PINS 4
#elif HEIGHT == 64
#define NUM_ADDR_PINS 5
#endif

// Thresholds for display
int level_1 = 5;  // 0 - level_1 GOOD
int level_2 = 10; // level_1 to level_2 MID
                  // anything above level_2 BAD

// Data structure - must match ESP32 sender
struct DataPacket {
  int id;
  float pm25;
  float pm10;
};

DataPacket receivedData;
WiFiUDP udp;
uint8_t packetBuffer[255];

// Matrix initialization
Adafruit_Protomatter matrix(
  WIDTH, 4, 1, rgbPins, NUM_ADDR_PINS, addrPins,
  clockPin, latchPin, oePin, true);

// Your frame data
extern const uint16_t *frames[];  // Make sure these are defined in your other header files
const uint16_t *frames[] = { good_1, good_2, mid_1, mid_2, bad_1, bad_2 };

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for serial for up to 3 seconds
  
  Serial.println("Matrix Portal S3 Starting...");

  // Initialize matrix
  ProtomatterStatus status = matrix.begin();
  Serial.print("Matrix initialization status: ");
  Serial.println((int)status);
  if (status != PROTOMATTER_OK) {
    Serial.println("Matrix initialization failed!");
    while(1) {
      delay(100);
    }
  }
  
  // Clear the matrix
  matrix.fillScreen(matrix.color565(0, 0, 0));
  matrix.show();
  
  // Connect to WiFi
  connectToWiFi();
  
  // Start UDP
  udp.begin(UDP_PORT);
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Try to connect for 30 seconds
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect. Restarting...");
    delay(3000);
    ESP.restart();
  }
  
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectToWiFi();
  }
  
  // Check for UDP packets
  int packetSize = udp.parsePacket();
  if (packetSize) {
    // We've received a packet
    if (packetSize == sizeof(DataPacket)) {
      // Read the packet into our data structure
      udp.read((uint8_t*)&receivedData, sizeof(DataPacket));
      
      // Print received data
      Serial.print("Received from IP: ");
      Serial.println(udp.remoteIP());
      Serial.print("PM2.5: ");
      Serial.println(receivedData.pm25);
      Serial.print("PM10: ");
      Serial.println(receivedData.pm10);
      
      // Update display
      displayFrames(receivedData.pm25);
    }
  }
  
  // Optional: Add a small delay to prevent tight looping
  delay(10);
}

void displayFrames(float pm25) {
  int frameStart;
  if (pm25 <= level_1) {
    frameStart = 0;  // Index for good frames
  } else if (pm25 <= level_2) {
    frameStart = 2;  // Index for mid frames
  } else {
    frameStart = 4;  // Index for bad frames
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