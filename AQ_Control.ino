/*---------------------
      Cammand List
  ---------------------
  1.) Get temprature.
  2.) Turn on light.
  3.) Turn off light.
  ---------------------
*/

int intRead; // int to identify command chosen.
float temp; // Temprature of the water.
bool light = 0; // lights off by default.

void getTemp() {
  temp = 13.37; // test float for temprature wright out.
}

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

