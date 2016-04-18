#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "sparkfun";


////////////////////
////Serial comm/////
float serialVal = 0.0;
String serialReading = "";

WiFiServer server(80);

void initHardware()
{
  Serial.begin(9600);
  Serial.println("Serial monitor connected");
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP8266 Thing  " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  //Read serial data first (doesn't work)
  /*
  int incoming = Serial.available();
    if(incoming>0){
      serialVal = Serial.parseInt();
      Serial.println(serialVal);
    } 
  */
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
                // request type (read/set) and value if set.
  if (req.indexOf("/read") != -1)
    val = -2; // Will print pin reads
  // Otherwise request will be invalid. We'll say as much in HTML

  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

  if (val == -2)
  { // If we're reading pins, print out those values:

    //added//
    ///////
    /* floats
    int incoming = Serial.available();
    if(incoming>0){
      float temp = Serial.parseFloat();
      if(temp > 0.0){
        serialVal = temp;
      }
    } 
    */
    //strings
    int incoming = Serial.available();
    if(incoming>0){
        serialReading = Serial.readStringUntil('/');
    }
    
    
    s += "Serial Monitor Reading = ";
    s += String(serialReading);
    s += "<br>"; // Go to the next line.
    //s += "Digital Pin 12 = ";
    //s += String(digitalRead(DIGITAL_PIN));
  }
  else
  {
    s += "Welcome to our ECE 387 Project.<br> Try /read to view our sensor data.";
  }
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}





