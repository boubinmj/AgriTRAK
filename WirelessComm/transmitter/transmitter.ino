// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
#include <VirtualWire.h>
void setup()
{
    Serial.begin(9600);   // Debugging only
    Serial.println("setup");
    pinMode(13, OUTPUT);
    // Initialise the IO and ISR
    //vw_set_tx_pin(1);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
}
void loop()
{
    char msg[50];
    String bigString = "";
    
    int sensor_int = 10;

    bigString += sensor_int;
    bigString += '/';
    bigString += 200;

    Serial.print("string val: ");
    Serial.println(bigString);

    bigString.toCharArray(msg, 50);

    Serial.print("char val: ");
    Serial.println(msg);
    
    digitalWrite(13, HIGH); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, LOW);
    delay(500);
}
