/* AUTOMATIC FUME EXTRACTOR         JEC 25-03-2017
 * 
 * Fume extractor using an Attiny85 and Adafruit library
 * 
 */

#include <Adafruit_MLX90614_TinyWire.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

Adafruit_MLX90614_TinyWire sensor = Adafruit_MLX90614_TinyWire();             // New object
SoftwareSerial serialPort(-1, 4);                                             // Create a serial port


const float   temp = 30;                                    // Threshold temperature
const int     fanPin = 3;                                   // Fan pin
const long    delayOff = 10000;                             // Delay to switch off fan

bool          bOldState = false;                            // Fan old state
unsigned long currentMillis = 0;                            // Current Millis()
unsigned long oldMillis     = 0;                            // Previous Millis()


void setup() {

  setupPowerSaving();

  pinMode(fanPin,OUTPUT);
  digitalWrite(fanPin,LOW);
  sensor.begin();
  serialPort.begin(9600);

}

void loop() {

  serialPort.println(sensor.readObjectTempC());

  if (sensor.readObjectTempC() > temp ) {
    digitalWrite(fanPin,HIGH);
    bOldState = true;
    
  }
  else {

    if (bOldState == true) {
      currentMillis = millis();
      bOldState = false;
    }

    if( (millis() - currentMillis) > delayOff  && !bOldState){
      digitalWrite(fanPin,LOW);
    }
      
    
  }

  sleep(1);

}


void setupWatchdog() {
  cli();                                            // Disable global interrupts
  // Set watchdog timer in interrupt mode
  // allow changes, disable reset
  WDTCR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval
  WDTCR = bit (WDIE) | bit (WDP2) | bit (WDP0);     // set WDIE, and 1 seconds delay
  sei();                                            // Enable global interrupts
}


void sleep(int times) {

  setupWatchdog();                                  // Configure and activate Watchdog

  for (int i = 0; i < times; i++) {

    cli();                                          // Disable interruptions just in case
    sleep_enable();                                 // Enable sleep

    sei();                                          // Enable interruptions
    sleep_cpu();                                    // Go to sleep
    sleep_disable();                                // When cpu is awake, disable sleep

  }

  wdt_disable();                                    // Disable watchdog in order to not have more WDT interruptions
}

ISR(WDT_vect) {

  // Nothing to do, just wake up

}

void setupPowerSaving(){
  
  ADCSRA = 0;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

}

