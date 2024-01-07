// Circuit Playground Color Sensing Example
// Use a NeoPixel RGB LED and light sensor on the Circuit Playground board to
// do basic color detection.  By quickly flashing full red, green, and blue color
// light from the NeoPixel the light sensor can read the intensity of the
// reflected light and roughly approximate the color of the object.
//
// After uploading the sketch to Circuit Playground you can press and release the
// left or right button to do a color sense and print the red, green, blue component
// values to the serial monitor (115200 baud).  In addition all the NeoPixels on
// the board will be lit up to the detected color.  You should hold a brightly
// colored object right above the light sensor and NeoPixel #1 (upper
// left part of board, look for the eye symbol next to the color sensor) when
// performing the color sense.
//
// Author: Limor Fried & Tony DiCola
// License: MIT License (https://opensource.org/licenses/MIT)
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

#define NUMPIXELS 10  // Number of NeoPixels on the Circuit Playground Express

void setup() {
  Serial.begin(115200);
  // Initialize Circuit Playground library.
  CircuitPlayground.begin();
  CircuitPlayground.strip.setBrightness(50);
  rainbowCycle(2);  // Adjust the delay (in milliseconds) for the speed of the rainbow effect
  CircuitPlayground.clearPixels();
  CircuitPlayground.playTone(1600, 200);
  delay(50);
}

void loop() {
  // Detect if the left or right button is pressed by taking two readings with
  // a small delay inbetween.  If the button changes state (like it was pressed
  // or released) then the two readings will be different.
  bool left_first = CircuitPlayground.leftButton();
  bool right_first = CircuitPlayground.rightButton();
  delay(20);
  bool left_second = CircuitPlayground.leftButton();
  bool right_second = CircuitPlayground.rightButton();
  // Now check if either button was released, i.e. changed from a true (pressed)
  // state to a false (non-pressed) state.
  if ((left_first && !left_second) || (right_first && !right_second)) {
    // Button was pressed, perform a color sense.
    // First turn off all the pixels to make sure they don't interfere with the
    // reading.
    CircuitPlayground.clearPixels();
    // Now take a color reading (the red, green, blue color components will be
    // returned in the parameters passed in).
    uint8_t red, green, blue;
    CircuitPlayground.senseColor(red, green, blue);
    // Print out the color components.
    Serial.print("Color: red=");
    Serial.print(red, DEC);
    Serial.print(" green=");
    Serial.print(green, DEC);
    Serial.print(" blue=");
    Serial.println(blue, DEC);
    // Gamma correction makes LED brightness appear more linear
    red   = CircuitPlayground.gamma8(red);
    green = CircuitPlayground.gamma8(green);
    blue  = CircuitPlayground.gamma8(blue);
    // Finally set all the pixels to the detected color.
    for (int i=0; i<10; ++i) {
      CircuitPlayground.strip.setPixelColor(i, red, green, blue);
    }
    CircuitPlayground.strip.show();
    CircuitPlayground.playTone(1600, 150);
    delay(3000);
    CircuitPlayground.clearPixels();
    delay(50);
  }
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