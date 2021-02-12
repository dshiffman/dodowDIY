// Inspiration from gary909 instructable:
// https://www.instructables.com/DIY-Dodow-Clone-Arduino-Sleep-Meditation-Machine

#include "Arduino.h"
#include <EEPROM.h>
#include <avr/sleep.h>
#include "debug.h"

const int diag = PB1;           	// u/d indicator is attached  pin 6
const int led = PB0;           		// LED is attached to pwm pin 5
const int modeSwitch = PB2;			  // 8 or 20 on on pin 7
const int rx = PB3;
const int tx = PB4;
const float pwmIntervals = 254;
const float minpwm = 50;			    // minimum brightness
const float msPerMin = 60000;

diagPins(rx, tx);

// Fade a LED to perceived brightness rather than linearly
// Thanks to Diarmuid Mac Namara's blog for the calculation:
// https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms
// Use lookup table in EEPROM rather than RAM, don't re-write each time

void populateLookup() {
	const float r = (pwmIntervals * log10(2))/(log10(255));
	diagPrintln ("Populate");
	byte PWMval, EEPval;
	for (int i = 0; i <= pwmIntervals; i++) {
		// Calculate the required PWM value for this interval step
		PWMval = round(pow (2, (i / r))) - 1;
		EEPval = EEPROM.read(i);
		if (EEPval != PWMval) EEPROM.write (i,PWMval); // Only write if changed
	}
}

void ramp (float BPM, float minutesDuration) {
	//40% in (up) 60% out (down) for total of 60,000/BPM milliseconds in up/down cycle
	unsigned int upmsdelay = round(0.4*msPerMin/BPM/(pwmIntervals-minpwm));
	unsigned int downmsdelay = round(0.6*msPerMin/BPM/(pwmIntervals-minpwm));
	byte reps = round(minutesDuration*BPM);
	byte brightness = 0;

	diagPrint (BPM,10);diagPrint (" breaths per min ");
	diagPrint ("for ");diagPrint (minutesDuration,10);diagPrintln (" min(s)");
	diagPrint ("up=");diagPrintln (upmsdelay);
	diagPrint ("down=");diagPrintln (downmsdelay);
	diagPrint ("reps=");diagPrintln (reps);

	unsigned long actual;
	unsigned long start = millis();

	for (int rep = 0;rep<reps;rep++) {
		digitalWrite (diag,HIGH);
		for (byte interval = (int)minpwm; interval <= (int)pwmIntervals; interval++) {
			brightness = EEPROM.read(interval);
			analogWrite(led, brightness);
			delay(upmsdelay);
		}
		digitalWrite (diag,LOW);
		for (byte interval = (int)pwmIntervals; interval > (int)minpwm; interval--) {
			brightness = EEPROM.read(interval);
			analogWrite(led, brightness);
			delay(downmsdelay);
		}
		diagPrint ("loop=");diagPrintln (rep,10);
	}

	actual = millis()-start;
	diagPrint ("each loop actual =");diagPrintln (actual/reps);
	diagPrintln ("---------");
}

void docycle () {
    diagPrintln ("DOCYCLE");
    int sensorVal = digitalRead(modeSwitch);

    //Check the switch to see if we sould run 8 or 20 mins
    if (sensorVal == LOW) {	//8 Min Structure
    	diagPrintln ("8 min loop");
    	ramp (11.0,1); 		//Min 1 = 11 breaths per minute
    	ramp (10.0,1);		//Min 2 = 10 breaths per minute
    	ramp (9.0,1);		//Min 3 = 9 breaths per minute
    	ramp (8.0,1);		//Min 4 = 8 breaths per minute
    	ramp (7.0,1);		//Min 5 = 7 breaths per minute
    	ramp (6.5,1);		//Min 6 = 6.5 breaths per minute
    	ramp (6.0,2);		//Min 7-8 = 6 breaths per minute
    } else { 				//20 Min Structure
       	diagPrintln ("20 min loop");
    	ramp (11.0,3); 		//Min 1-3 = 11 breaths per minute
    	ramp (10.0,3);		//Min 4-6 = 10 breaths per minute
    	ramp (9.0,3);		//Min 7-9 = 9 breaths per minute
    	ramp (8.0,3);		//Min 10-12 = 8 breaths per minute
    	ramp (7.0,3);		//Min 13-15 = 7 breaths per minute
    	ramp (6.5,2);		//Min 16-17 = 6.5 breaths per minute
    	ramp (6.0,3);		//Min 18-20 = 6 breaths per minute
    }
}


// the setup routine runs once powered on or when you press reset:
void setup() {
	diagBegin(9600);
    pinMode(diag, OUTPUT);
    pinMode(led, OUTPUT);
    pinMode(modeSwitch, INPUT_PULLUP);

    populateLookup(); //compute the fade values

	docycle(); //do it!

	/* Go into low power mode: -- thanks to Nick Gammon's blog: http://www.gammon.com.au/power
	 * All outputs to low
	 * ADC off
	 * Sleep mode to 'power down'
	 * Brown out disabled
	*/
	digitalWrite (diag,LOW);
    digitalWrite (led,LOW);
    digitalWrite (tx,LOW);
	ADCSRA = 0;
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_bod_disable();
    interrupts ();
    sleep_cpu ();
}

void loop() {
//never get here
}

