//Motor Connections
#define EnA 9
#define EnB 10
#define In1 4
#define In2 5
#define In3 6
#define In4 7

void setup()
{
  // All motor control pins are outputs
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  Serial.begin(9600);
}

void goStraight(int duration, int speedA, int speedB)   //run both motors
{
  // turn on motor A
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  // turn on motor B
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  // set speed of motor A
  analogWrite(EnA, speedA);
  // set speed of motor B
  analogWrite(EnB, speedB);
  delay(duration);
  // now turn off motors
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);  
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}
void loop()
{
  delay(2000);
  //go right
  goStraight(1000, 200, 200);
  
  
}
