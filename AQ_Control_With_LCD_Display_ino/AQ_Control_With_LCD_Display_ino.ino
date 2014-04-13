/*! \Cammand List
*      Commands that can used from a serial connection.
*
*   1.) Print temprature.
*   2.) Turn on light.
*   3.) Turn off light.
*   4.) Print time from RTC.
*/
  
#define DS18S20_ID 0x10
#define DS18B20_ID 0x28
#define DS1307_ADDRESS 0x68
#define ONE_WIRE_BUS_PIN 14
#define RELAY1 8
#define RELAY2 9
#define RELAY3 10
#define RELAY4 11
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>


// -- Variables to hold RTC --
int second;                                     
int minute;                                     
int hour;                                       
int weekDay;                                    
int monthDay;                                   
int month;                                      
int year;                                       
// ---------------------------

int intRead;                                    // int to identify command chosen.
int light = 0;                                  // State of lights (1 = On or 0 = Off).
int offTime = 22;                               // Hour of the day to turn off lights.
int onTime = 10;                                // Hour of the day to turn on lights.
float temp;                                     // Holds temprature value of current read.
float temp1;                                    // Holds temprature of sensor1.
float temp2;                                    // Holds temprature of sensor2.
byte zero = 0x00;                               // Workaround for issue #527 in RTC.
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);            // Initialize the library with the numbers of the interface pins.
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);

// Temprature sensor's addresses.
DeviceAddress Probe01 = { 0x28, 0x60, 0xB3, 0xA0, 0x05, 0x00, 0x00, 0x40 }; 
DeviceAddress Probe02 = { 0x28, 0xAD, 0x7A, 0xA0, 0x05, 0x00, 0x00, 0x44 };

/*! \Function to convert time.
*      bcdToDec();
*
*   Convert binary coded decimal 
*   to normal decimal numbers.
*/
byte bcdToDec(byte val)  {

  return ( (val/16*10) + (val%16) );
}

/*! \Function to get time.
*      getDate();
*
*   This gets the time from the DS1307
*/
void getDate() {
  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111);       //24 hour time
  weekDay = bcdToDec(Wire.read());               //0-6 -> sunday - Saturday
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

/*! \Function to print time.
*      printDate();
*
*   Print the date EG 23:59:59
*/
void printDate(){
  
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);
}

/*! \Function to get temperature.
*      printTemperature();
*
*   This gets the tempreature from
*   the DS18B20's Connected to pin 14
*   and converts it to degrees celsius.
*/
void printTemperature(DeviceAddress deviceAddress) {

   float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) 
   {
     Serial.print("Error getting temperature  ");
   }
   else
   {
     temp = tempC;
   }
}

/*! \Control Lights ON.
*      lightsON();
*
*   This function will control the lights.
*   it will serve as the way for the timer
*   to turn the lights on at a 
*   specific time of the day.
*/
void lightsOn() {
  digitalWrite(RELAY4,LOW);                     // Turn off light 4.
  delay(3000);                                  
  digitalWrite(RELAY3,LOW);                     // Turn off light 3.
  delay(3000);                                  
  digitalWrite(RELAY2,LOW);                     // Turn off light 2
  delay(3000);                                  
  digitalWrite(RELAY1,LOW);                     // Turn off light 1.
  light = 1;
}

/*! \Control Lights Off.
*      lightsOff();
*
*   This function will control the lights.
*   it will serve as the way for the timer
*   to turn the lights off at a 
*   specific time of the day.
*/
void lightsOff() {
  digitalWrite(RELAY1,HIGH);                    // Turn on light 1.
  delay(3000);                                  
  digitalWrite(RELAY2,HIGH);                    // Turn on light 2.
  delay(3000);                                  
  digitalWrite(RELAY3,HIGH);                    // Turn on light 3.
  delay(3000);                                  
  digitalWrite(RELAY4,HIGH);                    // Turn on light 4.
  light = 0;
}

//-------- SETUP START --------//
void setup() {
  pinMode(RELAY1, OUTPUT);                      
  pinMode(RELAY2, OUTPUT);                      
  pinMode(RELAY3, OUTPUT);                      
  pinMode(RELAY4, OUTPUT);                      
  Serial.begin(9600);                           // Enable serial communication.
  lcd.begin(16, 2);                             // LCD number of colums and rows.
  Wire.begin();                                 // Start the connection to the RTC.
  lightsOn();                                   // Turn lights on by default.
  delay(5);                                     // Pause for stability.
  sensors.setResolution(Probe01, 10);
  sensors.setResolution(Probe02, 10);
}
//--------- SETUP END ---------//

//-------- LOOP START--------//
void loop() {
  
  delay(250);                                   // Wait for stability.
  sensors.requestTemperatures();
  printTemperature(Probe01);                    // Get temprature from sensor1.
  temp1 = temp;
  printTemperature(Probe02);                    // Get temprature from sensor2.
  temp2 = temp;
  
  lcd.setCursor(0, 1);                          
  lcd.print(temp1);                             // Write temprature from sensor1 to LCD.
  lcd.setCursor(5, 1);                          
  lcd.print("C");                               
  lcd.setCursor(10, 1);                         
  lcd.print(temp2);                             // Write temprature from sensor2 to LCD.
  lcd.setCursor(15, 1);                         
  lcd.print("C");
  getDate();
  
  //---- Light controlled by time ----//
  if(hour >= offTime || hour < onTime) {
    if (light == 1)
    {
      lightsOff();                              // Run function to turn off lights.
      delay(5);                                 // Pause for stability.
    }
  }
  else if (hour >= onTime && hour < offTime) {
    if (light == 0)
    {
      lightsOn();                               // Run function to turn off lights.
      delay(5);                                 // Pause for stability.
    }
  }
  //----------------------------------//
  
  if (temp1 < 24.00 || temp2 < 24.00) {
    lcd.setCursor(0, 0);                        
    lcd.print("WARNING TEMP LO ");              // Warning for low temprature.
  }
  else if (temp1 > 27.20 || temp2 > 27.20) {
    lcd.setCursor(0, 0);                        
    lcd.print("WARNING TEMP HI ");              // Warning for high temprature.
  }
  else {
    lcd.setCursor(0, 0);                        
    lcd.print("AQ-Control ");                   // Write standard text on screen.    
    
    // Making sure no number is left on display.
    if (hour < 10) {
      lcd.print("0");
      lcd.print(hour);
      lcd.print(":");
    }
    else {
      lcd.print(hour);
      lcd.print(":");
    }
    if (minute < 10) {
      lcd.print("0");
      lcd.print(minute);
    }
    else {
      lcd.print(minute);
    }
  }
  
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
      Serial.print(temp1);                      // Print temprature from sensor1 to serial.
      Serial.println("C");                      
      Serial.print(temp2);                      // Print temprature from sensor2 to serial.
      Serial.println("C");
      delay(5);                                 // Pause for stability.
    }
    else if (intRead == '2')                    // Command "2" from the command list.
    { 
      lightsOn();                               // Turn on lights.
      delay(5);                                 // Pause for stability.
    }
    else if (intRead == '3')                    // Command "3" from the command list.
    { 
      lightsOff();                              // Turn off lights.
      delay(5);                                 // Pause for stability.
    }
    else if (intRead == '4')                    // Command "4" from the command list.
    { 
      printDate();                              // Print date time to console.
      delay(5);                                 // Pause for stability.
    }
  }  
}
//--------- LOOP END ---------//
