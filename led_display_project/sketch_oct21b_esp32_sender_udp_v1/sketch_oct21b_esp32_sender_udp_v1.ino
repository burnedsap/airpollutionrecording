#include <WiFi.h>
#include <WiFiUdp.h>
#include "SdsDustSensor.h"

// WiFi Settings
const char* ssid = "ESP32_Matrix";
const char* password = "matrix123";
const int UDP_PORT = 4210;
const IPAddress broadcastIP(192, 168, 4, 255);  // Broadcast address for subnet

// Board and Sensor Settings
#define BOARD_ID 1
const long INTERVAL = 500;  // Interval at which to publish sensor readings
#define RXD2 16
#define TXD2 17

// Initialize objects
SdsDustSensor sds(Serial2);
WiFiUDP udp;

// Structure to send data (matching receiver structure)
typedef struct struct_message {
    int id;
    float pm25;
    float pm10;
} struct_message;

struct_message myData;
unsigned long previousMillis = 0;

void setup() {
    // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("Starting setup...");

    // Initialize SDS sensor
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    Serial.println("Serial2 initialized");
    
    sds.begin();
    Serial.println("SDS sensor initialized");
    Serial.println(sds.setActiveReportingMode().toString());
    Serial.println(sds.setContinuousWorkingPeriod().toString());

    // Setup WiFi Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    
    Serial.println("UDP Broadcast Sender");
    Serial.println("WiFi Access Point Created");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("SSID: ");
    Serial.println(ssid);
    
    // Start UDP
    udp.begin(UDP_PORT);
}

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= INTERVAL) {
        previousMillis = currentMillis;
        
        // Read sensor data
        myData.id = BOARD_ID;
        PmResult pm = sds.readPm();
        
        if (pm.isOk()) {
            myData.pm25 = pm.pm25;
            myData.pm10 = pm.pm10;
            Serial.println(pm.toString());
        } else {
            Serial.println("Could not read values from sensor");
            myData.pm25 = 40;  // Use sentinel value to indicate read error
            myData.pm10 = 0;
        }

        // Send UDP broadcast
        udp.beginPacket(broadcastIP, UDP_PORT);
        udp.write((uint8_t*)&myData, sizeof(myData));
        bool success = udp.endPacket();
        
        if (success) {
            Serial.printf("Broadcast sent - PM2.5: %.2f, PM10: %.2f\n", 
                        myData.pm25, myData.pm10);
        } else {
            Serial.println("Failed to broadcast message");
        }
    }

    // Optional: Check if any clients are connected
    if (WiFi.softAPgetStationNum() == 0) {
        Serial.println("No clients connected to AP");
    }
}
