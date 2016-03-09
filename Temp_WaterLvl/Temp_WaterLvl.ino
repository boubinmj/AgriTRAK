#include <OneWire.h> 
#define SERIESRESISTOR 470
#define ETAPE A0
#define LED_GREEN 12
#define LED_RED 11

int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2

void setup(void) {
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
} 

void loop(void) {
  /*read temperature from Temp Sensor*/
  float temperature = getTemp();
  Serial.print("Water temp: ");
  Serial.println(temperature);

  /*If temp is below 15 deg celcius, red led is on. If temp is above 15
  deg, green led is on*/
  if(temperature < 15){
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }
  else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);    
  }

  /*read voltage value from ETAPE*/
  float sensorValue = analogRead(ETAPE);
  //convert value to resistance
  sensorValue = (1023.0/sensorValue) - 1.0;
  sensorValue = SERIESRESISTOR/sensorValue;


  float voltage = sensorValue * (3.3/1023.0);
  Serial.print("eTape: ");
  Serial.println(sensorValue);
  
  delay(1000); //just here to slow down the output so it is easier to read


  
}


float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}
