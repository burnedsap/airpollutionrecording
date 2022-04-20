#include <FastLED.h>
#define NUM_LEDS 300
#define DATA_PIN 14
#define BRIGHTNESS  128
CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // startup delay to protect LEDs
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() {
  CHSV color1 = CHSV(255, 100, 150);
  fill_solid(leds, NUM_LEDS, color1);
  FastLED.show();
  delay(100);
}
