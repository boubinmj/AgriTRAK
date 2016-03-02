
void setup() {
  //Init Sensors
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A5);
  Serial.println(x);
  delay(100);
  
}
