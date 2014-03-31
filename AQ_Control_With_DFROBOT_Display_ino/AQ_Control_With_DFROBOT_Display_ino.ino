/*-----------------------
       Cammand List
  -----------------------
  1.) Get temprature.
  2.) Turn on light.
  3.) Turn off light.
  -----------------------
  0.) Get sensor address.
  -----------------------
*/
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
#define RELAY1 15
#define RELAY2 16
#define RELAY3 17
#define RELAY4 18
#include <OneWire.h>
#include <LiquidCrystal.h>

int intRead;                                    // int to identify command chosen.
int sensor = 14;
float temp;                                     // Temprature of the water.
byte i;
byte present = 0;
byte data[12];                                  // This will hold the data returned by sensor.
byte addr[8];                                   // This will hold the address of the sensor.
OneWire ds(sensor);                             // Select pin wher DS18B22 is connected.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);            // initialize the library with the numbers of the interface pins.

//----------------------------------
//    Function to get temperature.
//----------------------------------
// This gets the tempreature from
// the DS18B20's Connected to pin 14
// and converts it to degrees celsius.
//----------------------------------
boolean getTemp(){
 
  temp = 00.00;                                 // Debug purpose.
  
  // If device not found on addr return false.
  if (!ds.search(addr)) {                       // Search for devices.
    ds.reset_search();                          // Clear search state.
    //return false;
  }

  if (OneWire::crc8( addr, 7) != addr[7]) {
    return false;
  }
  
  if (addr[0] != DS18B20_ID) {
    return false;
  }
  
  ds.reset();                                   // Reset cycle to see if device still present.
  ds.select(addr);                              // Select rom after reset.
  
  ds.write(0x44, 1);                            // Start conversion
  
  delay(50);                                    // Wait some time.
  present = ds.reset();                         // Check if DS18B22 is present.
  ds.select(addr);                              // Select rom command addr 8.
  
  ds.write(0xBE);                               // Issue Read scratchpad command
   
  // Receive 9 bytes
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
   
  temp = ( (data[1] << 8) + data[0] )*0.0625;   // Calculate temperature value  
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
void lightsOn() {
  digitalWrite(RELAY1,HIGH);                    // Turn on light 1.
  delay(3000);                                  // Wait for a while.
  digitalWrite(RELAY2,HIGH);                    // Turn on light 2.
  delay(3000);                                  // Wait for a while.
  digitalWrite(RELAY3,HIGH);                    // Turn on light 3.
  delay(3000);                                  // Wait for a while.
  digitalWrite(RELAY4,HIGH);                    // Turn on light 4.
}
void lightsOff() {
  digitalWrite(RELAY4,LOW);                     // Turn off light 4.
  delay(3000);                                  // Wait for a while.
  digitalWrite(RELAY3,LOW);                     // Turn off light 3.
  delay(3000);                                  // Wait for a while.
  digitalWrite(RELAY2,LOW);                     // Turn off light 2
  delay(3000);                                  // Wait for a while.
  digitalWrite(RELAY1,LOW);                     // Turn off light 1.
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
        Serial.print('0');                      // Add a leading '0' if required.
      }
      Serial.print(addr[i], HEX);               // print the actual value in HEX
      if (i < 7) {
        Serial.print(", ");                     // Print comma between the values.
      }
    }
  Serial.print("\r\n");                         // New line after print.
}

//-------- SETUP START --------//
void setup() {
  pinMode(RELAY1, OUTPUT);                      // Set RELAY1(pin 14) to output.
  pinMode(RELAY2, OUTPUT);                      // Set RELAY2(pin 15) to output.
  pinMode(RELAY3, OUTPUT);                      // Set RELAY3(pin 16) to output.
  pinMode(RELAY4, OUTPUT);                      // Set RELAY3(pin 17) to output.
  digitalWrite(RELAY1,LOW);                     // Turn off light 1 by default.
  digitalWrite(RELAY2,LOW);                     // Turn off light 2 by default.
  digitalWrite(RELAY3,LOW);                     // Turn off light 3 by default.
  digitalWrite(RELAY4,LOW);                     // Turn off light 4 by default.
  Serial.begin(9600);                           // Enable serial communication.
  lcd.begin(16, 2);                             // LCD colums, rows.
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
  if (Serial.available()) {                     // read from serial.
    intRead = Serial.read();                    // Read the most recent int.    
    
    if (intRead == '1')                         // Command "1"from the command list.
    {      
      getTemp();                                // Run function to get temprature.
      delay(5);                                 // Pause for stability.
      Serial.print(temp);                       // Print temprature to serial.
      Serial.println("C");                      // Put a C on it!.
    }
    else if (intRead == '2')                    // Command "2" from the command list.
    { 
      lightsOn();                               // Run function to turn on lights.
      delay(5);                                 // Pause for stability.
    }
    else if (intRead == '3')                    // Command "2" from the command list.
    { 
      lightsOff();                              // Run function to turn off lights.
      delay(5);                                 // Pause for stability.
    }
    else if (intRead == '0')                    // Command "2" from the command list.
    {
      getAddr();                                // Run function to control lights.
      delay(5);                                 // Pause for stability.
    }
  }
  delay(1000);                                  // wait for stability.
  getTemp();                                    // Run function to get temprature.
  lcd.setCursor(0, 1);                          // Set LCD cursor to charater 0 on 2nd. line.
  lcd.print(temp);                              // Write temp to LCD.
  lcd.setCursor(5, 1);                          // Set LCD cursor to charater 5 on 2nd line.
  lcd.print("C");                               // Put a C on it LCD edition.
  
  if (temp < 24.00) {
    lcd.setCursor(0, 0);                        // Set LCD cursor before writing.
    lcd.print("WARNING TEMP LO ");              // Warning for low temprature.
  }
  else if (temp > 27.70) {
    lcd.setCursor(0, 0);                        // Set LCD cursor before writing.
    lcd.print("WARNING TEMP HI ");              // Warning for high temprature.
  }
  else {
    lcd.setCursor(0, 0);                        // Set LCD cursor before writing.
    lcd.print("AQ-Control      ");              // Write standard text on screen.
  }
}
//--------- LOOP END ---------//

