/* AUTOMATIC FUME EXTRACTOR         JEC 25-03-2017
 * 
 * Fume extractor using an Attiny85 and Wire library
 * 
 */


#include <TinyWireM.h>
//#include <SoftwareSerial.h>

//SoftwareSerial serialPort(-1, 4);                           // Create a serial port


const float   thTemp = 30;                                  // Threshold temperature
const int     fanPin = 3;                                   // Fan pin
const long    delayOff = 10000;                             // Delay to switch off fan

float         temp;                                         // Measured temp
bool          bOldState = false;                            // Fan old state
unsigned long currentMillis = 0;                            // Current Millis()
unsigned long oldMillis     = 0;                            // Previous Millis()


void setup() {

  setupPowerSaving();

  pinMode(fanPin,OUTPUT);
  digitalWrite(fanPin,LOW);
  
  //serialPort.begin(9600);
  TinyWireM.begin();

}

void loop() {


  temp = sensorRead(0x5A,0x07);
  temp *= 0.02;
  temp -= 273;

  //serialPort.println(temp);

  if (temp > thTemp && temp < 1000) {
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


// Read Mlx register
// port : i2c address
// reg  : register number
uint16_t sensorRead (byte port, byte reg)
{

  uint16_t ret;                                             // Value to return
  
  TinyWireM.beginTransmission (port);                            // Start communication with device at "port" address
  TinyWireM.write (reg);                                         // Select register to read
  TinyWireM.endTransmission (false);                                  // End transmission
  TinyWireM.requestFrom (port, 3);                               // Request three bytes
    
  ret = TinyWireM.read();                                        // Read MSB
  ret |= TinyWireM.read() << 8;                                  // Shift MSB and read LSB
  
  uint8_t pec = TinyWireM.read();                                // read Package Error Code

  return ret;                                                    // return read data
} 

