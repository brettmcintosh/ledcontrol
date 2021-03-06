// #include "FastLED.h"

#include "xbee_comm.h"

#define NUM_LEDS 160

#define DATA_PIN_1 15
#define CLOCK_PIN_1 14
#define DATA_PIN_2 17
#define CLOCK_PIN_2 16
#define DATA_PIN_3 19
#define CLOCK_PIN_3 18
#define DATA_PIN_4 21
#define CLOCK_PIN_4 20


#define NONE 0
#define RAINBOW 1
#define THEATER_CHASE 2
#define SCANNER 3
#define FADE 4
#define BOUNCE 5
#define SPARKLE 6
#define JANKY_CHASE 7

#define FORWARD 1
#define REVERSE 0

class Strip {
	public:
	CRGB *strip;

	uint8_t current_animation;
	uint8_t direction = FORWARD;

	unsigned long interval;
	unsigned long last_update;

	uint8_t length;
	uint8_t brightness;
	uint8_t color1, color2;
	uint8_t range1, range2;
	uint8_t min1, min2, max1, max2;
	uint8_t fade_speed;
	uint8_t random_color;
	uint16_t total_steps;
	uint16_t current_step;

	uint16_t step_array[10];
	uint8_t color_array[10];
	unsigned long interval_array[10];
	unsigned long last_update_array[10];

	CHSV hsv1, hsv2;

	CRGB rgb1, rgb2;

	void (*on_complete)();

	// constructor
	Strip(CRGB led_strip[], void (*callback)()){
		strip = led_strip;
		on_complete = callback;
	}

	void update();
	void increment();
	void increment_array();
	void reverse();
	void rainbow(uint8_t rainbow_interval, uint8_t brightness, uint8_t dir);
	void rainbow_update();
	void theater_chase(CHSV c1, CHSV c2, uint8_t interval, uint8_t gap_length, uint8_t bright=255, uint8_t dir=FORWARD);
	void theater_chase_update();
	void scanner(CHSV c1, uint8_t interval, uint8_t bright, uint16_t start);
	void scanner_update();
	void fade(uint8_t c1, uint8_t c2, uint16_t steps, uint8_t fade_interval, uint8_t dir);
	void fade_update();
	void bounce(uint8_t bounce_interval, uint8_t bright, uint8_t f_speed);
	void bounce_update();
	void sparkle(uint8_t base_hue, uint8_t c_interval, uint8_t color_variety, uint8_t sat);
	void sparkle_update();
	void janky_chase(uint8_t j_interval, uint8_t l_min, uint8_t l_max, uint8_t b_min, uint8_t b_max, uint8_t hue, uint8_t random_color);
	void janky_chase_update();
};

void Strip::update(){
	receive_packet();
	// Serial.print("command ");
	// Serial.println(command);
	// Serial.print("current ");
	// Serial.println(current_animation);
	// Serial.println(millis());
	// Serial.println(last_update);
	// Serial.println(current_animation == RAINBOW);
	// Serial.println((millis() - last_update) > interval1);
	// Serial.println(".");

	// if(command != current_animation){
	if(check_for_new_data()){
	    // Serial.println(command);
		switch (command) {
			case RAINBOW:
	      		rainbow(packet[1], packet[2], packet[3]);
		        break;
	        case THEATER_CHASE:
	        	theater_chase(CHSV(packet[1], 255, 255), CHSV(packet[2],255, 255), packet[3], packet[4], packet[5], packet[6]);
	        	break;
	    	case SCANNER:
	        	scanner(CHSV(packet[1], 255, 255), packet[2], packet[3], packet[4]);
	        	break;
        	case FADE:
        		fade(packet[1], packet[2], packet[3], packet[4], packet[5]);
        		break;
    		case BOUNCE:
    			bounce(packet[1], packet[2], packet[3]);
    			break;	
    		case SPARKLE:
    			sparkle(packet[1], packet[2], packet[3], packet[4]);
    			break;
			case JANKY_CHASE:
				janky_chase(packet[1], packet[2], packet[3], packet[4], packet[5], packet[6], packet[7]);
				break;
		    default:
		    	Serial.println("?");
		        break;
		}
	}

	if((millis() - last_update) > interval){
		last_update = millis();
		switch (current_animation) {
		    case RAINBOW:
	      		rainbow_update();
		        break;
	        case THEATER_CHASE:
	        	theater_chase_update();
	        	break;
	    	case SCANNER:
	        	scanner_update();
	        	break;
        	case FADE:
        		fade_update();
        		break;
    		case BOUNCE:
    			bounce_update();
    			break;	
			case SPARKLE:
				sparkle_update();
				break;
			case JANKY_CHASE:
				janky_chase_update();
				break;
		    default:
		        break;
		}
	}
}

void Strip::increment(){
	if(direction == FORWARD){
		current_step++;
		if(current_step >= total_steps){
			current_step = 0;
		}
	}else{ //direction == REVERSE
		--current_step;
		if(current_step <= 0){
			current_step = total_steps - 1;
		}
	}
	if(on_complete != NULL){
		on_complete();
	}
}

void Strip::increment_array(){
	

	if(direction == FORWARD){
		for(int i = 0; i < 10; i++){
			if((millis() - last_update_array[i]) > interval_array[i]){
				last_update_array[i] = millis();
				step_array[i]++;
				if(step_array[i] >= total_steps){
					step_array[i] = 0;
					color_array[i] = random8();
					interval_array[i] = random(5, 50);
				}
			}
		}
	}else{
		for(int i = 0; i < 10; i++){
			if((millis() - last_update_array[i]) > interval_array[i]){
				last_update_array[i] = millis();
				--step_array[i];
				if(step_array[i] <= 0){
					step_array[i] = total_steps - 1;
					color_array[i] = random8();
					interval_array[i] = random(25, 50);
				}
			}
		}
	}
	if(on_complete != NULL){
		on_complete();
	}
}

void Strip::reverse(){
	if(direction == FORWARD){
		direction = REVERSE;
		current_step = total_steps - 1;
	}else{ //direction == REVERSE
		direction = FORWARD;
		current_step = 0;
	}
}

void Strip::rainbow(uint8_t rainbow_interval, uint8_t bright=255, uint8_t dir=FORWARD){
	// Serial.println('RAINBOWING');
	// Serial.println(current_animation);
	current_animation = RAINBOW;
	interval = rainbow_interval;
	total_steps = 255;
	current_step = 0;
	direction = dir;
	brightness = bright;
}

void Strip::rainbow_update(){
	// Serial.println("In rainbow update");
	for(int i = 0; i < NUM_LEDS; i++){
		int color = ((i * 255) / NUM_LEDS) + current_step;
		// strip[i].setHue(color);
		strip[i] = CHSV(color, 255, brightness);
	}
	// FastLED.show();
	// Serial.println("showed");
	increment();
	// Serial.println('incremented!');
}

void Strip::theater_chase(CHSV background, CHSV foreground, uint8_t theater_interval, uint8_t gap_length, uint8_t bright, uint8_t dir){
	current_animation = THEATER_CHASE;
	interval = theater_interval;
	total_steps = NUM_LEDS;
	hsv1 = foreground;
	// hsv2 = background;
	hsv2 = CHSV(0, 0, 0);
	current_step = 0;
	direction = dir;
	brightness = bright;
	length = gap_length;
}

void Strip::theater_chase_update(){
	for(int i = 0; i < NUM_LEDS; i++){
		CHSV color;
		if((i + current_step) % length == 0){
			color = hsv1;
		}else{
			color = hsv2;
		}
		strip[i] = color;
	}
	// FastLED.show();
	increment();
	// Serial.println("TheATER UPDATE");
}

void Strip::scanner(CHSV color, uint8_t scanner_interval, uint8_t bright, uint16_t start){
	current_animation = SCANNER;
	interval = scanner_interval;
	total_steps = NUM_LEDS;
	hsv1 = color;
	current_step = start;
	brightness = bright;
}

void Strip::scanner_update(){
	for(int i = 0; i < NUM_LEDS; i++){
		if(i == current_step || i == total_steps - current_step){
			strip[i] = hsv1;

		}else if(i > current_step - 8 || i < current_step + 8){
			strip[i].fadeToBlackBy(64);
		}
	}
	// FastLED.show();
	increment();
}

void Strip::fade(uint8_t c1, uint8_t c2, uint16_t steps, uint8_t fade_interval, uint8_t dir = FORWARD){
	current_animation = FADE;
	interval = fade_interval;
	total_steps = steps;
	current_step = 0;
	color1 = c1;
	color2 = c2;
	direction = dir;
}

void Strip::fade_update(){
	// uint8_t red = ((hsv1.r * (total_steps - current_step)) + (hsv2.r * current_step)) / total_steps;
	// uint8_t green = ((hsv1.g * (total_steps - current_step)) + (hsv2.g * current_step)) / total_steps;
	// uint8_t blue = ((hsv1.b * (total_steps - current_step)) + (hsv2.b * current_step)) / total_steps;

	uint8_t c = ((color1 * (total_steps - current_step)) + (color2 * current_step)) / total_steps;

	fill_solid( &(strip[0]), NUM_LEDS, CHSV(c, 255, 255));

	// Serial.println(current_step);
	if(current_step == total_steps - 1){
		Serial.println("CHANGING");
		color1 = color2;
		color2 = random8();
	}

	// FastLED.show();
	increment();
}

void Strip::bounce(uint8_t bounce_interval, uint8_t bright, uint8_t f_speed){
	current_animation = BOUNCE;
	interval = bounce_interval;
	total_steps = NUM_LEDS + 10;
	current_step = 0;
	fade_speed = f_speed;
	brightness = bright;
	direction = FORWARD;
	for(int i = 0; i < 10; i++){
		step_array[i] = random(0, total_steps);
		color_array[i] = random8();
		interval_array[i] = random(25, 50);
		last_update_array[i] = 0;
	}
}

void Strip::bounce_update(){
	for(int i = 0; i < NUM_LEDS; i++){
			strip[i].fadeToBlackBy(fade_speed);
	}

	for(int j = 0; j < 10; j++){
		// Serial.print(j);
		// Serial.print(" -> ");
		// Serial.println(step_array[j]);

		uint16_t s = step_array[j];
		if(0 <= s && s < NUM_LEDS){
			strip[s] = CHSV(color_array[j], 255, 255);
		}

		// for(int t = 1; t <= 24; t++){
		// 	if(direction == FORWARD){
		// 		int d = s - t;
		// 		if(0 <= d && d < NUM_LEDS){
		// 			// strip[d] += CHSV(20, 0, 0);
		// 			strip[d].fadeToBlackBy(32);
		// 		}
		// 	}else{
		// 		int d = s + t;
		// 		if(0 <= d && d < NUM_LEDS){
		// 			// strip[d] += CHSV(20, 0, 0);
		// 			strip[d].fadeToBlackBy(32);
		// 		}
		// 	}
		// }
	}
	// FastLED.show();
	increment_array();
	// Serial.println("");
}

void Strip::sparkle(uint8_t base_hue, uint8_t c_interval, uint8_t color_variety, uint8_t sat){
	current_animation = SPARKLE;
	interval = c_interval;
	total_steps = 2000;
	current_step = 0;
	color1 = base_hue;
	range1 = color_variety;
	range2 = sat;
}

void Strip::sparkle_update(){
	fadeToBlackBy(strip, NUM_LEDS, 10);
	int pos = random16(NUM_LEDS);
	strip[pos] += CHSV( color1 + random8(range1), range2, 255);

	// FastLED.show();
	increment();
	// Serial.println("sparkle");
}

void Strip::janky_chase(uint8_t j_interval, uint8_t l_min, uint8_t l_max, uint8_t b_min, uint8_t b_max, uint8_t hue, uint8_t rand_color){
	current_animation = JANKY_CHASE;
	interval = j_interval;
	min1 = l_min;
	max1 = l_max;
	min2 = b_min;
	max2 = b_max;
	range1 = random8(l_min, l_max);
	range2 = random8(b_min, b_max);
	random_color = rand_color;
	hsv1 = CHSV(hue, 255, 255);
	if(random_color){
		hsv1 = CHSV(random8(), 255, 255);
	}
}

void Strip::janky_chase_update(){
	Serial.println('.');
	Serial.println(range1);
	Serial.println(range2);
	if(range1 > 0){
		strip[0] = hsv1;
		range1--;
	}else{
		if(range2 > 0){
			// strip[0] = CRGB::Black;
			strip[0].fadeToBlackBy(200);
			range2--;
		}else{
			range1 = random8(min1, max1);
			range2 = random8(min2, max2);
			if(random_color){
				hsv1 = CHSV(random8(), 255, 255);
			}
		}
	}

	for(int i = NUM_LEDS - 1; i > 0; i--){
		strip[i] = strip[i - 1];
	}
}