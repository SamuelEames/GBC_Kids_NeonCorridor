/* GBC_Kids_NeonCorridor

Controller to sequence lights down a corridor for a series

Electronics
 * WS2811 ICs fed into RGB amplifier, driving 12V analog LED strips (one strip per module)
 * ATTINY Microcontroller because it's small and I have a lot of them not being used

 */

#include <Adafruit_NeoPixel.h>

// IO Pins
#define PIN_PIXEL   2   // Data pin for LEDs

// Pixel Setup
#define NUM_LEDS    4             // Note: I'm using single colour LEDs, so '1 pixel' = 3 LEDs
uint8_t modules[NUM_LEDS*3];      // Stores intensity values for each module - Ideally I'd use the leds arrays to do that, but I don't know where the raw values of that is stored :(
Adafruit_NeoPixel leds(NUM_LEDS, PIN_PIXEL, NEO_RGB + NEO_KHZ800);

// Global Variables

// Parameters
#define STEP_INTERVAL   100      // (ms) interval for step effect
#define LED_GROUPS      6         // Every x light lit up in sequence



void setup() 
{
  leds.begin();                               // Initialize Pixels
  leds.show();                                // Initialize all pixels to 'off'
}


void loop() 
{
  static uint32_t stepTimer = 0;

  if (millis() < stepTimer)            // (handle timer overflow)
  {
    stepTimer = millis();
    return;
  }

  if (millis() - stepTimer >= STEP_INTERVAL)
  {
    updateLEDs();
    stepTimer = millis();                     // Reset timer
  }

  
}



void updateLEDs()
{
  // Updates LED state in modules array, then updates pixel output

  static uint8_t stepNum = 0;                 // Step number of sequence

  // modules[0] = 255;

  // Black output
  for (uint8_t i = 0; i < sizeof(modules); ++i)
    modules[i] = 0;

  // Light the ones we want
  for (uint8_t i = stepNum; i < sizeof(modules); i += LED_GROUPS)
  {
    if (i < sizeof(modules))
      modules[i] = 0xFF; 
  }



  if (++stepNum >= LED_GROUPS)
    stepNum = 0;

  writeLEDs();                                // Display LEDs state
  
  return;
}


void writeLEDs()
{
  // Writes modules buffer into leds
  static uint8_t modules_last[sizeof(modules)];
  bool newData = false;                   

  for (uint8_t i = 0; i < sizeof(modules); ++i)
  {
    if (modules[i] != modules_last[i])        // Check for changes in data
      newData = true;

    modules_last[i] = modules[i];             // Record for next loop
  }

  if (newData)                                // Only refresh LEDs if we actually have a new state to show
  {
    for (uint8_t i = 0; i < NUM_LEDS; ++i)
      leds.setPixelColor(i, modules[0+3*i], modules[1+3*i], modules[2+3*i]);

    leds.show();
  }

  return;
}
