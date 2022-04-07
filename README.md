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


### Setup

To set up a multiple ESP32 + PM sensor setup, collect your ESP32's and PM Sensors, wire them up and keep them ready. 

Wiring config:

TXD to RX2
RXD to TX2
GND to GND
5V to VIN

Mark one of the ESP32's as the 'Main' with paper tape and flash the code from the folder Arduino/Main.ino to it.

Mark the other ESP32's numerically as '1', '2', '3' and so forth, and flash the code from the folder Arduino/Support-1.ino, Arduino/Support-2.ino, Arduino/Support-3.ino, and so on. Remember the numbers-they are important!


Now that all the code has been flashed, set up the ESP32 + PM sensor and power them up. Open your computer and connect to the WiFi network with the name 'ESP32' and the password is '12345678'. Open the Processing sketch and hit the play button. The resulting visuals should show the PM data on a line graph.

