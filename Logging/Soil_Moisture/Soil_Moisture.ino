
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
  if(x>600){
    digitalWrite(13,HIGH);
  }
  Serial.print("DATA,TIME,");
  Serial.println(x);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  
}
