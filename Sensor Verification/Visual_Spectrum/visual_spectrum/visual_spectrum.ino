#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);

}

void loop() {
   Wire.requestFrom(1, 6);    // request 6 bytes from slave device #2

  while(Wire.available())    // slave may send less than requested
  { 
    int c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
}
