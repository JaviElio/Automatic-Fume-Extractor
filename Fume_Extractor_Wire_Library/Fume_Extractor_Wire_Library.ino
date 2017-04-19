/* AUTOMATIC FUME EXTRACTOR         JEC 25-03-2017
 * 
 * Fume extractor using an Attiny85 and Wire library
 * 
 */


#include <TinyWireM.h>
//#include <SoftwareSerial.h>

//SoftwareSerial serialPort(-1, 4);                           // Create a serial port


const float   thTemp = 10;                                  // Threshold temperature
const int     fanPin = 3;                                   // Fan pin
const int     ledPin = 1;                                   // Led pin
const long    delayOff = 10000;                             // Delay to switch off fan

float         objTemp;                                      // Measured object temp
float         ambTemp;                                      // Measured ambient temp
bool          bfanState = false;                            // Fan old state
bool          bDelayActive = false;                         // Fan delay state
unsigned long currentMillis = 0;                            // Current Millis()
unsigned long fanMillis     = 0;                            // Millis for fan delay
unsigned long ledMillis     = 0;                            // Millis for led blink


void setup() {

  setupPowerSaving();

  pinMode(fanPin,OUTPUT);
  pinMode(ledPin,OUTPUT);
  digitalWrite(fanPin,LOW);
  digitalWrite(ledPin,LOW);
  
  //serialPort.begin(9600);
  TinyWireM.begin();

}

void loop() {


//  Measure every 500ms
  if (millis() > currentMillis + 500) {

      currentMillis = millis();
      
      objTemp = sensorRead(0x5A,0x07);
      objTemp *= 0.02;
      objTemp -= 273;

      ambTemp = sensorRead(0x5A,0x06);
      ambTemp *= 0.02;
      ambTemp -= 273;

 //serialPort.print(objTemp);
 //serialPort.print("   ");
 //serialPort.println(ambTemp);
 

  }
      
 


// Manage fan output
  if ( (objTemp-ambTemp) > thTemp && objTemp < 1000) {
     digitalWrite(fanPin,HIGH);
     bfanState = true;
         
   }
   else if ( (objTemp-ambTemp) < thTemp) {

     if (bfanState == true) {
       fanMillis = millis();
       bfanState = false;
       bDelayActive = true;
      }

      if( (millis() - fanMillis) > delayOff  && !bfanState){
        digitalWrite(fanPin,LOW);
        bDelayActive = false;
       }
          
    }

// Manage LED output

  if (bfanState) digitalWrite(ledPin,HIGH);

  if (bDelayActive) {

      if (millis()-ledMillis > 300) {
        PORTB ^= 0x02;
        ledMillis = millis(); 
      }
  }
  else if (!bfanState) {
    
      digitalWrite(ledPin,LOW);
  }
  
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

