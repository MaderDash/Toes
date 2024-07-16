#include <FastLED.h>

#define LED_PIN 6  				// Pin for the LED data line
#define NUM_LEDS 49  			// Number of LEDs in the matrix
#define DELAY_TIME 1000  	// Delay time in milliseconds
#define FADE_DELAY 20 		// Delay for fading in milliseconds

const int NUM_PATTERN_LEDS 		= 16;
const int ACCELERATION_STEPS 	= 5;
const int DECELERATION_STEPS 	= 5;
const int DEFAULT_FADE_STEPS 	= 10;

enum DisplayState {
  FLAT = 0,
  SMILE,
  SLOW,
  STOP
};

// LED matrix layout
//  https://wokwi.com/projects/401952384875216897


/*
42 43 44 45 46 47 48
41 40 39 38 37 36 35
28 29 30 31 32 33 34
27 26 25 24 23 22 21
14 15 16 17 18 19 20
13 12 11 10  9  8  7
 0  1  2  3  4  5  6
*/

// LED patterns for each state
byte led_patterns[4][NUM_PATTERN_LEDS] = {
  {21, 22, 23, 24, 25, 26, 27,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // Flat
  {40, 42, 30, 31, 32, 36, 48,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // Smile
  { 0,  6,  8, 12, 16, 17, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // Slow
  { 2,  3,  4,  8, 12, 14, 20, 21, 27, 28, 34, 36, 40, 44, 45, 46}  // Stop
};

// Acceleration patterns (from FLAT to SMILE)
byte acceleration[2][16] = {
  {34, 22, 23, 24, 25, 26, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {35, 33, 23, 24, 25, 29, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Deceleration patterns (from FLAT to SLOW/STOP)
byte deceleration[2][16] = {
  {20, 22, 23, 24, 25, 26, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  { 7, 19, 23, 24, 25, 13, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

CRGB leds[NUM_LEDS];
DisplayState current_display_state = FLAT;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void display_pattern(const byte* pattern) {
  for (int led_index = 0; led_index < NUM_LEDS; led_index++) {
    leds[led_index] = CRGB::Black;
  }
  for (int pattern_index = 0; pattern_index < 16; pattern_index++) {
    if (pattern[pattern_index] > 0) {
      leds[pattern[pattern_index]] = CRGB::Red;
    }
  }
  FastLED.show();
}


void fade_to_pattern(const byte* target_pattern, int fade_steps) {
  byte current_brightness[NUM_LEDS];

  // Get current brightness of each LED
  for (int i = 0; i < NUM_LEDS; i++) {
    current_brightness[i] = leds[i].red;
  }

  // Calculate brightness steps for each LED
  int brightness_step[NUM_LEDS];
  for (int i = 0; i < 16; i++) {
    if (target_pattern[i] > 0) {
      brightness_step[target_pattern[i]] = (255 - current_brightness[target_pattern[i]]) / fade_steps;
    }
  }

  // Fade in the target pattern
  for (int step = 0; step <= fade_steps; step++) {
    for (int i = 0; i < 16; i++) {
      if (target_pattern[i] > 0) {
        if (current_brightness[target_pattern[i]] < 255) {
          current_brightness[target_pattern[i]] += brightness_step[target_pattern[i]];
          leds[target_pattern[i]] = CRGB(current_brightness[target_pattern[i]], 0, 0);
        }
      }
    }
    FastLED.show();
    delay(FADE_DELAY);
  }
}

void loop() {
  // Example usage:

  // 1. Display FLAT
  display_pattern(led_patterns[FLAT]);
  delay(DELAY_TIME);

  // 2. Fade from FLAT to SMILE (using acceleration patterns)
  for (int i = 0; i < sizeof(acceleration) / sizeof(acceleration[0]); i++) {
    fade_to_pattern(acceleration[i], ACCELERATION_STEPS); // 5 fade steps
    delay(DELAY_TIME / 2); // Shorter delay between acceleration steps
  }
  display_pattern(led_patterns[SMILE]); // Ensure SMILE is fully displayed
  delay(DELAY_TIME);

  // 3. Fade from SMILE to FLAT
  fade_to_pattern(led_patterns[FLAT], DEFAULT_FADE_STEPS); // 10 fade steps
  delay(DELAY_TIME);

  // 4. Fade from FLAT to SLOW (using deceleration patterns)
  for (int i = 0; i < sizeof(deceleration) / sizeof(deceleration[0]); i++) {
    fade_to_pattern(deceleration[i], DECELERATION_STEPS); // 5 fade steps
    delay(DELAY_TIME / 2); // Shorter delay between deceleration steps
  }
  display_pattern(led_patterns[SLOW]); // Ensure SLOW is fully displayed
  delay(DELAY_TIME);

  // 5. Fade from SLOW to FLAT
  fade_to_pattern(led_patterns[FLAT], DEFAULT_FADE_STEPS); // 10 fade steps
  delay(DELAY_TIME);

  // 6. Display STOP
  display_pattern(led_patterns[STOP]);
  delay(DELAY_TIME);
}

