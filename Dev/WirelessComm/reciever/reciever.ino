// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
#include <VirtualWire.h>

/*Global Message Variables*/

//Char string received by RF receiver
char sensorMsg[7];

//Int message after conversion from char string
int SensorData;

int soil;
int temp;
int uv;
int broadband;
int infrared;

String transmission = "";


void setup()
{
    Serial.begin(9600); // Debugging only
    //Serial.println("setup");
    pinMode(13, OUTPUT);
    // Initialise the IO and ISR
    //vw_set_rx_pin(0);  //Changes rx pin on board
    //Default rx pin is I/O pin 11
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void decodeMsg(){
  char** sensorData_parsed;
    char* sensorData;  
    const char delim[2] = "/";

    //char soil[4];
    //char temp[6];
    //char uv[5];
    //char broadBand[6];
    char infraRed_Char[4];

    int j = 1;
    
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    
    {
        int i;

        //Set LED to HIGH 
        //Indicates message received
        digitalWrite(13, HIGH); 
        // Message with a good checksum received, dump it.
        
        for (i = 0; i < buflen; i++)
        {
            sensorMsg[i]= char(buf[i]);
            
        }

        //Add null terminator to char string
        //String must be terminated to enable use of atoi function
        sensorMsg[buflen] = '\0';

        //Serial.print("Sensor Data 1: ");
        //Serial.print(sensorMsg);
        //Serial.println("");

        //Convert String to Int Value

        sensorData = strtok(sensorMsg, delim);
        //sensorData_parsed[0] = sensorData;

        //Serial.println("SensorData: ");
        //Serial.println(sensorData);
        

        while(sensorData != NULL){
           sensorData = strtok(NULL, delim);
           //Serial.println(sensorData);
           sensorData_parsed[j] = sensorData;
           j++;
        }

        soil = atoi(sensorData_parsed[1]);
        temp = atoi(sensorData_parsed[2]);
        uv = atoi(sensorData_parsed[3]);
        broadband = atoi(sensorData_parsed[4]);

        for(int p =0; p<2; p++){
          infraRed_Char[p] = sensorData_parsed[5][p];
        }
        
        infrared = atoi(infraRed_Char);
        
        /*
        Serial.println(soil);
        Serial.println(temp);
        Serial.println(uv);
        Serial.println(broadband);
        //Serial.println(infrared);
        */
        
        //create sendable message
        float tempC = float(temp)/100.0;
        float uvC = float(uv)/100.0;
        
        transmission="Soil: ";
        transmission+=String(soil);
        transmission+="/Temp: ";
        transmission+=String(tempC);
        transmission+=" degrees C/UV: ";
        transmission+=String(uvC);
        transmission+="/";
        
        Serial.println(transmission);

        
        //Set LED to LOW
        //indicating end of message transmission
        digitalWrite(13, LOW);
    }
}
void loop()
{
    decodeMsg();
    /*
    float tempC = float(temp)/100.0;
    float uvC = float(uv)/100.0;
    
    transmission="Soil: ";
    transmission+=String(soil);
    transmission+="/Temp: ";
    transmission+=String(tempC);
    transmission+="degree/UV: ";
    transmission+=String(uvC);
    transmission+="/";
    
    Serial.println(transmission);
    */
    
    /*
    char** sensorData_parsed;
    char* sensorData;  
    const char delim[2] = "/";

    //char soil[4];
    //char temp[6];
    //char uv[5];
    //char broadBand[6];
    char infraRed_Char[4];

    int j = 1;
    
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    
    {
        int i;

        //Set LED to HIGH 
        //Indicates message received
        digitalWrite(13, HIGH); 
        // Message with a good checksum received, dump it.
        
        for (i = 0; i < buflen; i++)
        {
            sensorMsg[i]= char(buf[i]);
            
        }

        //Add null terminator to char string
        //String must be terminated to enable use of atoi function
        sensorMsg[buflen] = '\0';

        Serial.print("Sensor Data 1: ");
        Serial.print(sensorMsg);
        Serial.println("");

        //Convert String to Int Value

        sensorData = strtok(sensorMsg, delim);
        //sensorData_parsed[0] = sensorData;

        Serial.println("SensorData: ");
        Serial.println(sensorData);
        

        while(sensorData != NULL){
           sensorData = strtok(NULL, delim);
           Serial.println(sensorData);
           sensorData_parsed[j] = sensorData;
           j++;
        }

        int soil;
        int temp;
        int uv;
        int broadband;
        int infrared;

        soil = atoi(sensorData_parsed[1]);
        temp = atoi(sensorData_parsed[2]);
        uv = atoi(sensorData_parsed[3]);
        broadband = atoi(sensorData_parsed[4]);

        for(int p =0; p<2; p++){
          infraRed_Char[p] = sensorData_parsed[5][p];
        }
        
        infrared = atoi(infraRed_Char);
        
        
        Serial.println(soil);
        Serial.println(temp);
        Serial.println(uv);
        Serial.println(broadband);
        //Serial.println(infrared);
        
        
        //Set LED to LOW
        //indicating end of message transmission
        digitalWrite(13, LOW);
    }
    */
}
