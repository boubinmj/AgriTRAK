
void setup() {
  //Init Sensors
  //Serial.println("CLEARDATA");
  //Serial.println("LABEL,TIME,X");
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A5);
  
  Serial.print(x);
  Serial.println(",");
  delay(1000);
  
}
