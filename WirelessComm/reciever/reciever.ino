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
char sensorMsg[4];

//Int message after conversion from char string
int SensorData;

void setup()
{
    Serial.begin(9600); // Debugging only
    Serial.println("setup");
    pinMode(13, OUTPUT);
    // Initialise the IO and ISR
    //vw_set_rx_pin(0);  //Changes rx pin on board
    //Default rx pin is I/O pin 11
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}
void loop()
{
    char** sensorData_parsed;
    char* sensorData;  
    const char delim[2] = "/";

    char soil[4];
    char temp[6];
    char uv[5];
    char broadBand[6];
    char infraRed[4];

    int i = 1;
    
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

        Serial.println("SensorData: ");
        Serial.println(sensorData);

        while(sensorData != NULL){
           sensorData = strtok(NULL, delim);
           Serial.println(sensorData);
           sensorData_parsed[i] = sensorData;
           i++;
        }

        
        //int testing = atoi(sensorData_parsed[3]);
        //Serial.println(testing);
       
        
        //Set LED to LOW
        //indicating end of message transmission
        digitalWrite(13, LOW);
    }
}
