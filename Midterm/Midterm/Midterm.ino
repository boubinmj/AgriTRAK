#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_TSL2561_U.h"
#include "Midterm.h"

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
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
   tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  //tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
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

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) 
{
  /*PIN initialization*/
  pinMode(MOISTURE, INPUT);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(3, INPUT);
  pinMode(4, INPUT);
  
  Serial.begin(9600);
  Serial.println("Light Sensor Test"); Serial.println("");
  
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

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{  
  /* Calculate Luminocity using TSL2561 Library Sensor Event*/
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /*
  if (event.light)
  {
    Serial.print(event.light); 
    Serial.println(" lux");
  }
  
  else
  {
    Serial.println("Sensor overload"); 
  }
  */
  /* Calculate Soil Moisture Level using Sparkfun 13322 VIN pin */

  int soil_moisture = analogRead(MOISTURE);

  int soil_state = soil_moisture/400;
  int light_state = event.light/24000;

  int soil = digitalRead(4);
  int light = digitalRead(3);

  Serial.print("moisture: ");
  Serial.println(soil_state);

  Serial.print("light_state: ");
  Serial.println(light_state);

  Serial.print("soil_in");
  Serial.println(soil);
  Serial.print("light");
  Serial.println(light);


  int state = soil_state + (3 - light_state);

  Serial.print("state: ");
  Serial.println(state);

  bool pushbutton_soil = false;
  bool pushbutton_light = false;

  if(soil > 5){
    pushbutton_soil = true;
  }

  if(light > 5){
    pushbutton_light = true;
  }

  if(pushbutton_soil){
    switch(soil_state){
    case 0:
      digitalWrite(LED_GREEN, HIGH);
      break;
    case 1:
      digitalWrite(LED_YELLOW, HIGH);
      break;
    case 2:
      digitalWrite(LED_RED, HIGH);
      break;
    }
  }

  else if(pushbutton_light){
    switch(light_state){
    case 0:
      digitalWrite(LED_RED, HIGH);
      break;
    case 1:
      digitalWrite(LED_YELLOW, HIGH);
      break;
    case 2:
      digitalWrite(LED_GREEN, HIGH);
      break;
    }
  }

else{
  switch(state){
    case 0:
      digitalWrite(LED_GREEN, HIGH);
      break;
    case 1:
      digitalWrite(LED_YELLOW, HIGH);
      break;
    case 2:
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      break;
    case 3:
      digitalWrite(LED_RED, HIGH);
      break;
    case 4:
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, HIGH);
      break;
    case 5:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      break;
    case 6:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_GREEN,HIGH);
      break;
    default:
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_GREEN,HIGH);
      break;    
  } 
}
  
  
  delay(1000);
  
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN,LOW);
  pushbutton_soil = false;
  pushbutton_light = false;
}
