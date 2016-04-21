#include <Wire.h>
#include <VirtualWire.h>

#include "OneWire.h"
#include "Adafruit_TSL2561_U.h"
#include "group.h"

//Sets datapin to be used by Temp Sensor
OneWire ds(TempIn); 

//Instantiates I2C communication ojbect for TSL2651 Visible and Infrared band light sensor   
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);


/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/

void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
   //tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
   //tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */  
  
}


//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**************************************************************************/
/*
    Method converts Temp Sensor Volage input to Degrees Celcius
*/
/**************************************************************************/

float getTemp(){


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
ds.write(0x44,1); 

byte present = ds.reset();
ds.select(addr); 
ds.write(0xBE); 


for (int i = 0; i < 9; i++) { 
data[i] = ds.read();
}

ds.reset_search();

byte MSB = data[1];
byte LSB = data[0];

float TRead = ((MSB << 8) | LSB); 
float Temperature = TRead / 16;

return Temperature;

}


/**************************************************************************/
/*
    Arduino setup function
    Configures Pin I/O and Serial settings
*/
/**************************************************************************/
void setup(void) 
{
  /*PIN initialization*/

  pinMode(MoistureIn, INPUT);
  
  Serial.begin(9600);
  Serial.println("Soil"); Serial.println("");

  pinMode(UVIn, INPUT);
  pinMode(UVRef, INPUT);
  
  /* Debugging TSL */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  //Configures TSL frame rate and signal amplification
  configureSensor();
  

  Serial.println("Degbug");
  pinMode(13, OUTPUT);
  // Initialise the IO and ISR for RF Communication
  //vw_set_tx_pin(1);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec

    
}


void loop(void) 
{  

  delay(1000);
  const char *msg = "hello";

  uint16_t broadband = 0;
  uint16_t infrared = 0;
  tsl.getLuminosity (&broadband, &infrared);
  

  int soil = analogRead(MoistureIn);
  
  float temp = getTemp();

  int uvLevel = averageAnalogRead(UVIn);
  int refLevel = averageAnalogRead(UVRef);

  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0);


  /***Print Sensor Values for Debugging*/
  Serial.print("soil_in: ");
  Serial.println(soil);
  Serial.print("temperature: ");
  Serial.println(temp);
  Serial.print("UV Intensity (mW/cm^2): ");
  Serial.println(uvIntensity);
  Serial.print("visual light (lux): ");
  Serial.println(broadband);
  Serial.print("Infrared Light (lux): ");
  Serial.println(infrared);

  
  

  /*Send Sensor Values in Array*/
  digitalWrite(13, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, LOW);

  /*Delay*/
  delay(2000);
}
