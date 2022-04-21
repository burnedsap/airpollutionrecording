#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    100
#define BRIGHTNESS  120
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  //  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  currentPalette[0] = CRGB(253, 231, 37);
  currentPalette[1] = CRGB(210, 226, 27);
  currentPalette[2] = CRGB(165, 219, 54);
  currentPalette[3] = CRGB(122, 209, 81);

  currentPalette[4] = CRGB(84, 197, 104);
  currentPalette[5] = CRGB(53, 183, 121);
  currentPalette[6] = CRGB(34, 168, 132);
  currentPalette[7] = CRGB(31, 152, 139);

  currentPalette[8] = CRGB(35, 136, 142);
  currentPalette[9] = CRGB(42, 120, 142);
  currentPalette[10] = CRGB(49, 104, 142);
  currentPalette[11] = CRGB(57, 86, 140);
  currentPalette[12] = CRGB(65, 68, 135);
  currentPalette[13] = CRGB(71, 47, 125);
  currentPalette[14] = CRGB(72, 26, 108);
  currentPalette[15] = CRGB(68, 1, 84);

}


void loop()
{
  for (int i = 0; i < 16; i++) {
    fill_solid(leds, NUM_LEDS, currentPalette[i]);
    FastLED.show();
    delay(1000);
  }
}

