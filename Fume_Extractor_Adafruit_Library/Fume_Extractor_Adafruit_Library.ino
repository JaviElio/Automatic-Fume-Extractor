/* AUTOMATIC FUME EXTRACTOR         JEC 25-03-2017
 * 
 * Fume extractor using an Attiny85 and Adafruit library
 * 
 */

#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 sensor = Adafruit_MLX90614();             // New object

const float   temp = 30;                                    // Threshold temperature
const int     fanPin = 13;                                  // Fan pin
const long    delayOff = 10000;                              // Delay to switch off fan

bool          bOldState = false;                            // Fan old state
unsigned long currentMillis = 0;                            // Current Millis()
unsigned long oldMillis     = 0;                            // Previous Millis()


void setup() {

  pinMode(fanPin,OUTPUT);
  digitalWrite(fanPin,LOW);
  sensor.begin();
  //Serial.begin(9600);

}

void loop() {

  //Serial.println(sensor.readObjectTempC());

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

  delay(1000);

}
