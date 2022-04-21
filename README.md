# airpollutionrecording

### Processing

- Download [Processing](https://processing.org/download)
- Go to Sketch -> Import Library -> Add Library -> Type in "UDP" in the search bar
- Download the UDP library by Stephane Cousot and click Install below.
- Then, navigate to the folder Processing and copy the code within the folder and paste it within your Processing sketch.
- Save the file and keep it ready


### Arduino

- Download the [Arduino IDE](https://www.arduino.cc/en/software)
- Go to Sketch -> Include Library -> Manage Library -> Type in "SDS011"
- Install the 'Nova Fitness Sds dust sensors library' by Pawel Kolodziejczyk
- Install the [ESP32 libraries](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)


### Setup

To set up a multiple ESP32 + PM sensor setup, collect your ESP32's and PM Sensors, wire them up and keep them ready. 

Wiring config:

TXD to RX2
RXD to TX2
GND to GND
5V to VIN


Mark one of the ESP32's as the 'Main' with paper tape and flash the code from the folder Arduino/Main.ino to it.


#### Find MAC Address of the Main ESP32

This 'Main' ESP32 will collect data from other ESP's, hence we will need to find it's MAC Address.

Run this code below on the 'Main' ESP32 to find it's MAC address:

```
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){
}
```

Open the Serial Monitor to see the MAC address

For Data logging: 

Mark the other ESP32's numerically as '1', '2', '3' and so forth, and flash the code from the folder Arduino/Support. Change the ```BOARD_ID``` number on each file which corresponds to each ESP. Remember the numbers-they are important! Replace the MAC address with the one from the 'Main' one. Also, add the Wifi SSID and change the interval the data is sent to the main ESP. It's currently at half a second (500 milliseconds).


### See Values in Processing

Find the local IP address of the computer which will run the Processing code. For the Mac, System Preferences -> Network

Change the value in the Main-Data-Logger.ino file:

``` #define CONSOLE_IP "192.168.0.206" //enter local ip address of computer using Processing ```

Open Processing/Sensor_Readout.pde from Github, and run that code. Values should start changing on the screen.


### Things to test
- [ ] Does the ESP32 stop working after 10 mins or does the Google Sheet API stop?
- [ ] Does changing the Google Sheet logging time from 10s to 60s help? 

