/*-----------------------
       Cammand List
  -----------------------
  1.) Get temprature.
  2.) Turn on light.
  -----------------------
  0.) Get sensor address.
  -----------------------
*/
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
#define RELAY1 6
#define RELAY2 7
#define LED 13
#include <OneWire.h>

int intRead; // int to identify command chosen.
int pin_number = 2;
float temp; // Temprature of the water.
byte i;
byte present = 0;
byte data[12]; // This will hold the data returned by sensor.
byte addr[8]; // This will hold the address of the sensor.
boolean light = 0; // lights off by default.
OneWire ds(pin_number); // Select pin wher DS18B22 is connected.

//----------------------------------
//    Function to get temperature.
//----------------------------------
// This gets the tempreature from
// the DS18B20 Connected to pin 2
// and converts it to degrees celsius.
//----------------------------------
boolean getTemp(){
 
  temp = 00.00; // Debug purpose.
  
  // If device not found on addr return false.
  if (!ds.search(addr)) {
    ds.reset_search(); // Clear search state.
    //return false;
  }
  
  if (OneWire::crc8( addr, 7) != addr[7]) {
    return false;
  }
  
  if (addr[0] != DS18B20_ID) {
    return false;
  }
  
  ds.reset(); // Reset cycle to see if device still present.
  ds.select(addr); // Select rom after reset.
  
  ds.write(0x44, 1); // Start conversion
  
  delay(50); // Wait some time.
  present = ds.reset(); // Check if DS18B22 is present.
  ds.select(addr); // Select rom command addr 8.
  
  ds.write(0xBE); // Issue Read scratchpad command
   
  // Receive 9 bytes
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  
  temp = ( (data[1] << 8) + data[0] )*0.0625; // Calculate temperature value
  return true;
}

//------------------------------------
//     Control Lights ON/OFF.
//------------------------------------
//This function will control the lights.
//it will serve as the way for the timer
//to turn the lights on and off at a 
//specific time of the day.
//------------------------------------
void lights() {
  if (light == 0) {
    digitalWrite(RELAY1,HIGH); // Turn on light 1.
    delay(2500); // Wait for a while.
    digitalWrite(RELAY2,HIGH); // Turn on light 2.
    light = 1; // Set light variable to on.
    digitalWrite(LED,HIGH); // Turn on LED to indicate lights on.
  }
  else if (light == 1) {
    digitalWrite(RELAY2,LOW); // Turn off light 2.
    delay(3000); // Wait for a while before turning on 2nd light.
    digitalWrite(RELAY1,LOW); // Turn off light 1.
    light = 0; // Set light variable to off.
    digitalWrite(LED,LOW); // Turn off LED to indicate lights off.
  }
}

//------------------------------------
//       Get sensor address.
//------------------------------------
//This function will assemble the
//address of the sensor and print in
//hexidecimal.
//------------------------------------
void getAddr() {
  for( i=0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0'); // Add a leading '0' if required.
      }
      Serial.print(addr[i], HEX); // print the actual value in HEX
      if (i < 7) {
        Serial.print(", "); // Print comma between the values.
      }
    }
  Serial.print("\r\n"); // New line after print.
}

//-------- SETUP START --------//
void setup() {
  pinMode(RELAY1, OUTPUT); // Set RELAY1(pin 6) to output.
  pinMode(RELAY2, OUTPUT); // Set RELAY2(pin 7) to output.
  pinMode(LED, OUTPUT); // Set onboard led to output.
  digitalWrite(RELAY1,LOW); // Turn off light 1 by default.
  digitalWrite(RELAY2,LOW); // Turn off light 2 by default.
  digitalWrite(LED,LOW); // turn off onboard led to indicate lights off.
  Serial.begin(9600); // Enable serial communication.
}
//--------- SETUP END ---------//

//-------- LOOP START--------//
void loop() {
  //-----------------------------
  //  Get command from serial
  //  and run the command listed.
  //-----------------------------
  // This will handle input from
  // the serial connection, and
  // run the functions requested
  // by the user/control unite.
  //-----------------------------
  if (Serial.available()) { // read from serial.
    intRead = Serial.read(); // Read the most recent int.    
    
    if (intRead == '1') // Command "1"from the command list.
    {      
      getTemp(); // Run function to get temprature.
      delay(5); // Pause for stability.
      Serial.print(temp); // Print temprature to serial.
      Serial.println("C");
    }
    else if (intRead == '2') // Command "2" from the command list.
    { 
      lights(); // Run function to control lights.
      delay(5); // Pause for stability.
    }
    else if (intRead == '0') // Command "2" from the command list.
    { 
      getAddr(); // Run function to control lights.
      delay(5); // Pause for stability.
    }
  }
}
//--------- LOOP END ---------//

