#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "ece387";


////////////////////
////Serial comm/////
float serialVal = 0.0;
String serialReading = "No reading";
String serialReading2 = "No reading";
String serialReading3 = "No reading";
String serialReading4 = "No reading";

WiFiServer server(80);


void initHardware()
{
  Serial.begin(9600);
  Serial.println("Serial monitor connected");
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  String AP_NameString = "AgriTrak";
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


  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";

    //strings
    int incoming = Serial.available();
    if(incoming>0){
        serialReading = Serial.readStringUntil('$');
        serialReading2 = Serial.readStringUntil('$');
        serialReading3 = Serial.readStringUntil('$');
        serialReading4 = Serial.readStringUntil('$');
    } 
    delay(500);
    

    s+="<head>";
    s+="<style> body {background-color:lightgrey;}";
    s+="h1 {text-align:center; font-family:verdana; color:#3d3d29;}";
    s+="h3 {text-align:center; font-family:verdana; color:#3d3d29;}";
    s+="h5 {text-align:center; font-family:verdana; color:#3d3d29;}";
    s+="</style>";
    s+="</head>";

    s+="<body>";
    s+="<h1>Welcome to AgriTrak.</h1>"; 
    s+="<h3><br>AgriTRAK is a sensor network project that is designed to monitor the necessary metrics of an agricultural system.<br><br><br></h3>"; 
    
    s+="<h5>";
    s+=String(serialReading);
    s+="<br><br>"; // Go to the next line.
    s+=serialReading2;
    s+="<br><br>";
    s+=serialReading3;
    s+="<br><br>";
    s+=serialReading4;
    s+="<br><br>";
    s+="</h5>";
    s+="</body>";
  
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
}





