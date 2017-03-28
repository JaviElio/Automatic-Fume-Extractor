/* AUTOMATIC FUME EXTRACTOR         JEC 25-03-2017
 * 
 * Fume extractor using an Attiny85 and Adafruit library
 * 
 */

#include <Adafruit_MLX90614_TinyWire.h>
//#include <SoftwareSerial.h>


Adafruit_MLX90614_TinyWire sensor = Adafruit_MLX90614_TinyWire();             // New object
//SoftwareSerial serialPort(-1, 4);                                             // Create a serial port


const float   thTemp = 30;                                  // Threshold temperature
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
//  serialPort.begin(9600);

}

void loop() {



//  serialPort.println(sensor.readObjectTempC());

  if (sensor.readObjectTempC() > thTemp) {
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

  delay(2000);

}

void setupPowerSaving(){
  
  ADCSRA = 0;                                                   // Disable ADC

}

