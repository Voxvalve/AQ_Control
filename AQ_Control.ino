/*---------------------
      Cammand List
  ---------------------
  1.) Get temprature.
  2.) Turn on light.
  3.) Turn off light.
  ---------------------
*/
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
#include <OneWire.h>

int intRead; // int to identify command chosen.
int pin_number = 2;
float temp; // Temprature of the water.
byte i;
byte present = 0;
byte data[12];
byte addr[8];
boolean light = 0; // lights off by default.
OneWire ds(pin_number); // Select pin wher DS18B22 is connected.


// Function to get temperature.
boolean getTemp(){
 
 //If device not found on addr.
 if (!ds.search(addr)) {
   ds.reset_search(); // Clear search state.
   return false;
 }
 
 if (OneWire::crc8( addr, 7) != addr[7]) {
   return false;
 }
 
 if (addr[0] != DS18S20_ID && addr[0] != DS18B20_ID) {
   return false;
 }
 ds.reset(); // Reset cycle to see if device still present.
 ds.select(addr); // Select rom after reset.
 
 ds.write(0x44, 1); // Start conversion
 
 delay(850); // Wait some time.
 present = ds.reset(); // Check if DS18B22 is present.
 ds.select(addr); // Select rom command addr 8.
 
 // Issue Read scratchpad command
 ds.write(0xBE);
 
 // Receive 9 bytes
 for ( i = 0; i < 9; i++) {
   data[i] = ds.read();
 }
 
 // Calculate temperature value
 temp = ( (data[1] << 8) + data[0] )*0.0625;
 return true;
}

// Control Lights ON/OFF.
void lights() {
  if (light == 0) {
    // Turn on light.
  }
  else if (light == 1) {
    // Turn off light.
  }
}

void setup() {
  Serial.begin(9600); // Enable serial communication.
}

void loop() {
  if (Serial.available()) { // read from serial.
    intRead = Serial.read(); // Read the most recent int.    
    
    if (intRead == '1') // Command "1".
    {      
      getTemp(); // Run function to get temprature.
      delay(5); // Pause for stability.
      Serial.println(temp); // Print temprature to serial.
    }
  }
}

