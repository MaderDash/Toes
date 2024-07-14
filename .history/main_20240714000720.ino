#include <FastLED.h>

#define LED_PIN 6  // Pin for the LED data line
#define NUM_LEDS 49  // Number of LEDs in the matrix
#define DELAY_TIME 1000  // Delay time in milliseconds

enum DisplayState {
    FLAT = 0,
    SMILE,
    SLOW,
    STOP
};

// this is the grid of led's.
/*
42 43 44 45 46 47 48
41 40 39 38 37 36 35
28 29 30 31 32 33 34
27 26 25 24 23 22 21
14 15 16 17 18 19 20
13 12 11 10  9  8  7
 0  1  2  3  4  5  6
*/

/*
byte acceloration[2][16] = {
{34,22,23,24,25,26,28, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
{35,33,23,24,25,29,41, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
};

byte decloration[3][16] = {
{20,22,23,24,25,26,14, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
{ 7,19,23,24,25,13,15, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
};
*/




byte led_patterns[4][16] = {
    {21,22,23,24,25,26,27, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
    {40,42,30,31,32,36,48, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Smile
    { 0, 6, 8,12,16,17,18, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Slow
    { 2, 3, 4, 8,12,14,20,21,27,28,34,36,40,44,45,46}  // Stop
};

CRGB leds[NUM_LEDS];
DisplayState current_display_state = FLAT;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void display_pattern(DisplayState state) {
  for (int led_index = 0; led_index < NUM_LEDS; led_index++) {
    leds[led_index] = CRGB::Black;  // Clear all LEDs
  }

  // Activate LEDs based on the selected pattern
  byte* current_pattern = led_patterns[(int)state];
  bool lock = false;
  for (int pattern_index = 0; pattern_index < 16; pattern_index++) {
    if (current_pattern[pattern_index] > 0 || lock == false){
      lock = true;
      leds[current_pattern[pattern_index]] = CRGB::Red;
    }
  }
  FastLED.show();
}

void loop() {
  display_pattern(SMILE);
  delay(DELAY_TIME);

  display_pattern(SLOW);
  delay(DELAY_TIME);

  display_pattern(FLAT);
  delay(DELAY_TIME);

  display_pattern(STOP);
  delay(DELAY_TIME);
}

// # I would like a set of arrays that will allow me to fade from one image to another.
// # I would like to be able to fade from one image to another.

// # please make me some code that will do this.


