#include <Wire.h>
#include "OneWire.h"
#include "Adafruit_TSL2561_U.h"
#include "group.h"

OneWire ds(TempIn); 

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   I2C Address
   ===========
   The address will be different depending on whether you leave
   the ADDR pin floating (addr 0x39), or tie it to ground or vcc. 
   The default addess is 0x39, which assumes the ADDR pin is floating
   (not connected to anything).  If you set the ADDR pin high
   or low, use TSL2561_ADDR_HIGH (0x49) or TSL2561_ADDR_LOW
   (0x29) respectively.
    
   History
   =======
   2013/JAN/31  - First version (KTOWN)
*/
   
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

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
  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         "); Serial.println("Auto");
  Serial.print  ("Timing:       "); Serial.println("13 ms");
  Serial.println("------------------------------------");
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
    Arduino setup function (automatically called at startup)
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
  
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
  
  /* Setup the sensor gain and integration time */
  configureSensor();
  
  /* We're ready to go! */
  Serial.println("");
}


void loop(void) 
{  
  /* Calculate Luminocity using TSL2561 Library Sensor Event
  sensors_event_t event;
  tsl.getEvent(&event);
 
  
  if (event.light)
  {
    Serial.print(event.light); 
    Serial.println(" lux");
  }
  
  else
  {
    Serial.println("Sensor overload"); 
  }
  
  /* Calculate Soil Moisture Level using Sparkfun 13322 VIN pin */

  uint16_t broadband = 0;
  uint16_t infrared = 0;
  tsl.getLuminosity (&broadband, &infrared);

  int soil = analogRead(MoistureIn);
  float temp = getTemp();

  int uvLevel = averageAnalogRead(UVIn);
  int refLevel = averageAnalogRead(UVRef);

  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0);

  Serial.print("soil_in");
  Serial.println("");
  Serial.println(soil);
  Serial.println("temperature");
  Serial.println(temp);
   Serial.print(" / UV Intensity (mW/cm^2): ");
  Serial.println(uvIntensity);
  Serial.print("visual light: ");
  Serial.println(broadband);
  Serial.print("Infrared Light: ");
  Serial.println(infrared);
  delay(2000);
}
