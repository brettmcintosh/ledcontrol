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

	// FastLED.addLeds<LPD8806, DATA_PIN_1, CLOCK_PIN_1, GRB, DATA_RATE_MHZ(12)>(leds1, NUM_LEDS);
	FastLED.addLeds<LPD8806, DATA_PIN_1, CLOCK_PIN_1, GRB>(leds1, NUM_LEDS);
	// FastLED.addLeds<LPD8806, DATA_PIN_1, CLOCK_PIN_1, GRB>(leds2, NUM_LEDS);

	strip1.rainbow(10, 128);
	// CHSV c1 = CHSV(0, 255, 255);
	// CHSV c2 = CHSV(80, 100, 128);
	// strip1.theater_chase(CHSV(160, 255, 0), CHSV(120, 255, 255), 75, 4, 255, REVERSE);
	// strip1.scanner(CHSV(random8(), 255, 255), 50, 255, 0);
	// strip1.fade(random8(), random8(), 100, 50);
	// strip1.bounce(5, 255, 0);
}

void loop() {
	strip1.update();
	// Serial.println(".");
}

// void serialEvent() {
//   while (Serial.available()) {
//     char pat = Serial.read();
//     // if(pat != '\r' || pat != '\n' || pat != NULL){
//     	Serial.println(pat);
// 	    switch (pat) {
// 	        case '1':
// 	            strip1.theater_chase(CHSV(160, 255, 0), CHSV(120, 255, 255), 50, 5, 255, REVERSE);
// 	            Serial.println("theater");
// 	            break;
// 	        case '2':
// 	            strip1.scanner(CHSV(random8(), 255, 255), 50, 255, 0);
// 	            Serial.println("scanner");
// 	            break;
// 	    	case '3':
// 	    		strip1.fade(random8(), random8(), 100, 50);
// 	    		Serial.println("fade");
// 	    		break;
// 	    	case '4':
// 	    		strip1.bounce(5, 255, 0);
// 	    		Serial.println("bounce");
// 	    		break;
//     		case 'a':
//     			strip1.rainbow(10, 128);
//     			break;
// 			case 'r':
// 				strip1.reverse();
// 				break;
// 	        default:
// 	            // strip1.bounce(5, 255, 0);
// 	            Serial.println("other");
// 	    }
//     // }
//   }
// }



void fill_black(){
	fill_solid( &(leds1[0]), NUM_LEDS, CRGB::Black);
}

void fill_purple(){
	fill_solid( &(leds1[0]), NUM_LEDS, CRGB::Purple);
}