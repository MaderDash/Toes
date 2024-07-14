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

byte matrex[4][16] = {
    {21,22,23,24,25,26,27, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Flat
    {40,42,30,31,32,36,48, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Smile
    { 0, 6, 8,12,16,17,18, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Slow
    { 2, 3, 4, 8,12,14,20,21,27,28,34,36,40,44,45,46}  // Stop
};

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void clear() {
    for (int counter = 0; counter < NUM_LEDS; counter++) {
        leds[counter] = CRGB::Black;  // Set the first LED to black
    }
    FastLED.show();
}

void lights(DisplayState state) {
    clear();
    byte* currentState = matrex[(int)state];
    for (int counter = 0; counter < 16; counter++) {
        if (counter < 1 || currentState[counter] > 0){
            leds[currentState[counter]] = CRGB::Red;  // Set the LED to red
        }
    }
    FastLED.show();
}

void changeState(DisplayState newState) {
    lights(displayState);
    delay(DELAY_TIME);
    clear();
    displayState = newState;
}

void loop() {
    changeState(SMILE);
    changeState(SLOW);
    changeState(FLAT);
    changeState(STOP);
}