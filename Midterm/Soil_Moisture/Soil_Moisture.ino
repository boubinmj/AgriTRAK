
void setup() {
  //Init Sensors
  //Serial.println("CLEARDATA");
  //Serial.println("LABEL,TIME,X");
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(A5);
  
  if(x > 900){
    digitalWrite(13, HIGH);    
  }
  else if(x > 600){
    digitalWrite(12, HIGH);    
  }
  else if(x > 300){
    digitalWrite(11, HIGH);    
  }
  
  delay(1000);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  
}
