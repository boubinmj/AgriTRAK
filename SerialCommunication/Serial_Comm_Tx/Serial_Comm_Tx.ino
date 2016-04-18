/*
For more details see: http://projectsfromtech.blogspot.com/

Connect the Tx pin of this board to the Rx of the board running Serial_Comm_Rx.ino
Connect the Grounds of the two boards

Transmits numbers 0-10 to the Rx board every second
Blinks the LED on pin 13 when every number is tranmitted
*/

const int led = 13;
void setup()
{
  Serial.begin(9600);
}

void loop()
{

  Serial.println(3);
  delay(1000);
  /*
 for (int i = 1; i <= 10; i++)
 {
   Serial.println (i); //Send i to Rx Arduino
   //digitalWrite(led, HIGH);
   
   //digitalWrite(led, LOW);
   delay(1000);   
 }
 delay(100);
 */
}
