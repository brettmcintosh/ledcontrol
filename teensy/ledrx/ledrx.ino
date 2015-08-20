// animations.ino
#include "FastLED.h"
#include <XBee.h>
#include "lib8tion.h"  // for the fancy RNG
// #include "xbee_comm.h"
#include "led.h"

// XBee stuff
//#############################################################################################

// XBee xbee = XBee();

// LED stuff
//#############################################################################################

CRGB leds1[NUM_LEDS];
Strip strip1 = Strip(leds1, NULL);

// Arduino stuff
//#############################################################################################

void setup() {

	// XBee
	Serial.begin(115200);
	Serial3.begin(115200);
	xbee.setSerial(Serial3);
	delay(1500);

	FastLED.addLeds<LPD8806, DATA_PIN_1, CLOCK_PIN_1, GRB>(leds1, NUM_LEDS);
	FastLED.addLeds<LPD8806, DATA_PIN_2, CLOCK_PIN_2, GRB>(leds1, NUM_LEDS);
	FastLED.addLeds<LPD8806, DATA_PIN_3, CLOCK_PIN_3, GRB>(leds1, NUM_LEDS);
	FastLED.addLeds<LPD8806, DATA_PIN_4, CLOCK_PIN_4, GRB>(leds1, NUM_LEDS);

	strip1.rainbow(10, 128);
	// strip1.bounce(1, 255, 0);
	// strip1.confetti(100, 2);
}

void loop() {
	strip1.update();
	// Serial.println(".");
}
