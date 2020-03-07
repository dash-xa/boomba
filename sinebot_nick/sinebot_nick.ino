//Motor Connections
#define EnA 9
#define EnB 10
#define In1 4
#define In2 5
#define In3 6
#define In4 7

#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 3, 2, 1, 0);

int state;
int DISARMED = 0;
int ARMED = 1;
int ACTIVE = 2;
int READ = 3;
int timer;

void setup()
{
  // All motor control pins are outputs
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  pinMode(53, INPUT_PULLUP); //timer increment
  pinMode(52, INPUT_PULLUP); //arm

  lcd.begin(16, 2);
  lcd.print("Hello!Set timer:");

  lcd.setCursor(0,1);
  lcd.print(timer);

  state = 0;
  timer = 0;

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
  

  switch(state){

  case DISARMED: //read user inputs for timer
    if(digitalRead(53) == LOW){
      timer+=15000;
      lcd.setCursor(0,1);
      lcd.print(timer/1000);
      } 
    if(digitalRead(52) == LOW){ //start timer!
      lcd.setCursor(0,0);
      lcd.print("                ");
      lcd.setCursor(0,0);
      lcd.print("ARMED");
      state = 1;
     }
      
  break;

  case ARMED:
    while(timer >= 0){
      delay(500);
      lcd.setCursor(0,1);
      lcd.print(timer/1000);
      timer-=1000; //decrement timer
      delay(500);
      lcd.setCursor(0,1);
      lcd.print("                ");
    }

    state = 2; //time expired

  break;

  case ACTIVE:
    goStraight(1000, 200, 200);
    
  break;

  case READ:
    
  break;
  
}
  
  
}
