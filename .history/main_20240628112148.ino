#include <FastLED.h>

#define LED_PIN 6  // Pin for the LED data line
#define NUM_LEDS 49  // Number of LEDs in the matrix

enum DisplayState {
  FLAT,
  SMILE,
  SLOW,
  STOP
};
DisplayState displayState = STOP;


CRGB leds[NUM_LEDS];
byte matrex[4][16] = {
  {21,22,23,24,25,26,27, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
  {40,42,30,31,32,36,48, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Smile
  { 0, 6, 8,12,16,17,18, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Slow
  { 2, 3, 4, 8,12,14,20,21,27,28,34,36,40,44,45,46} // Stop  
 
};
void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void test() {
  for (int counter = 0; counter < NUM_LEDS; counter++) {
    leds[counter] = CRGB::Red;  // Set the first LED to red
    FastLED.show();
    delay(100);
    leds[counter] = CRGB::Black;  // Set the first LED to black
  }
}

void clear() {
  for (int counter = 0; counter < NUM_LEDS; counter++) {
    leds[counter] = CRGB::Black;  // Set the first LED to black
  }
  FastLED.show();
}





void lights() {
  byte* currentState = matrex[(int)displayState];
  for (int counter = 0; counter < 16; counter++) {
    if (counter < 1 || currentState[counter] > 0){
      leds[currentState[counter]] = CRGB::Red;  // Set the LED to red
    }
  }
  FastLED.show();
}

void loop() {
  lights(); // Show the flat line
  delay(1000); // Hold the flat line for 1 second
  
  clear();
  displayState = SMILE;
  lights(); // Show the flat line
  delay(1000); // Hold the flat line for 1 second
  clear();
  displayState = SLOW;
  lights(); // Show the flat line
  delay(1000); // Hold the flat line for 1 second
  clear();
  displayState = FLAT;
  lights(); // Show the flat line
  delay(1000); // Hold the flat line for 1 second
  clear();
  
}