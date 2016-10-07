#include "FastLED.h"

#define NUM_LEDS 10
#define DATA_PIN 2
#define CHIPSET WS2812B
#define COLOR_ORDER RGB


#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

#define DISPLAYTIME 5
#define BLACKTIME   1


int BRIGHTNESS = 50;
CRGB leds[NUM_LEDS];

void setup() {
  delay(500);
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  static uint8_t starthue = 0;
  fill_rainbow(leds, NUM_LEDS, --starthue, 20);

  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);

  if (BRIGHTNESS > 255) {
    BRIGHTNESS = 0;
  } else {
    BRIGHTNESS += 50;
  }
  FastLED.setBrightness(BRIGHTNESS);

  if (secs < DISPLAYTIME) {
    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
    leds[0] = TEMPERATURE_1; // show indicator pixel
  } else {
    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
    leds[0] = TEMPERATURE_2; // show indicator pixel
  }

  if ((secs % DISPLAYTIME) < BLACKTIME) {
    memset8(leds, 0, NUM_LEDS * sizeof(CRGB));
  }

  FastLED.show();
  FastLED.delay(8);

}
