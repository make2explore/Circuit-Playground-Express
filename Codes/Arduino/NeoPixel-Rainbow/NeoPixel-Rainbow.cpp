// ---------------------------------- make2explore.com -------------------------------------------------------//
// Project           - Create a rainbow effect on an Adafruit Circuit Playground Express
// Created By        - info@make2explore.com
// Last Modified     - 14/12/2023 01:26:00 @admin
// Software          - C/C++, PlatformIO IDE, Visual Studio Code Editor, Libraries
// Hardware          - Adafruit Circuit Playground Express        
// Sensors Used      - OnBoard NeoPixels
// Source Repo       - github.com/make2explore
// -----------------------------------------------------------------------------------------------------------//

#include <Adafruit_CircuitPlayground.h>

#define NUMPIXELS 10  // Number of NeoPixels on the Circuit Playground Express

void setup() {
  CircuitPlayground.begin();
}

void loop() {
  rainbowCycle(20);  // Adjust the delay (in milliseconds) for the speed of the rainbow effect
}

// Function to create a rainbow cycle effect
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< CircuitPlayground.strip.numPixels(); i++) {
      CircuitPlayground.strip.setPixelColor(i, Wheel(((i * 256 / CircuitPlayground.strip.numPixels()) + j) & 255));
    }
    CircuitPlayground.strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return CircuitPlayground.strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return CircuitPlayground.strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return CircuitPlayground.strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// ---------------------------------- make2explore.com ----------------------------------------------------//