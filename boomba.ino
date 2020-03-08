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
const int DISARMED = 0;
const int ARMED = 1;
const int ACTIVE = 2;
const int READ = 3;
long timer;
int timerPrint;
bool press1;
bool press2;
//joystick
const int X_pin = A0; //analog pin connected to x output
const int Y_pin = A1; //analog pin connected to y output
int sensorValueX = 0;
int sensorValueY = 0; 
int x=0;
int y=0;

bool correct=false;
int rand_nums[4];
int lights[] = {42, 44, 46, 48};
int val;

const int UP = 42;
const int LEFT = 44;
const int DOWN = 46;
const int RIGHT = 48;

//random number for joysticks
int randNumber;
int inputNumber=0;
int counter=0;
bool audio = false;
bool checked = false;
int inputRand1;
int inputRand2;
int inputRand3;
int inputRand4;
void setup()
{
  randomSeed(analogRead(NULL));
  // All motor control pins are outputs
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(53, INPUT_PULLUP); //timer increment
  pinMode(52, INPUT_PULLUP); //arm
  pinMode(50, INPUT);
  pinMode(UP, OUTPUT);//up
  pinMode(LEFT, OUTPUT);//left
  pinMode(DOWN, OUTPUT);//down
  pinMode(RIGHT, OUTPUT);//right
  state = 0;
  timer = 0;
  timerPrint = 0;
  press1 = false;
  press2 = false;
  lcd.begin(16, 2);
  lcd.print("Hello!Set timer:");
  lcd.setCursor(0,1);
  lcd.print(timer);
  
  //Serial.begin(9600);
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
    
    if(digitalRead(53) == LOW && !press1){
      press1 = true;
      timer = timer + 1000;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Hello!Set timer:");
      lcd.setCursor(0, 1);
      timerPrint = timer/1000;
      lcd.print(timerPrint);
      }
    delay(250);
    press1 = false;   
      
    if(digitalRead(52) == LOW){ //start timer!
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("ARMED");
      state = 1;
     }
      
  break;
  case ARMED:
  
    while(timer >= 0){
      delay(500);
      lcd.setCursor(0,1);
      lcd.print(timer/1000);
      timer = timer-1000; //decrement timer
      delay(500);
      lcd.setCursor(0,1);
      lcd.print("                ");
    }
    state = 2; //time expired
  break;
  
  case ACTIVE:
    lcd.setCursor(0,0);
    lcd.print("ACTIVE");
    delay(3000);
    state = 3;
  break;
  
  case READ:
  
    lcd.clear();
    lcd.print("DISARM");
    
    if(!correct){
    for(int i = 0; i < 4; i++) {
      val = random(0,4);
      rand_nums[i] = val+1;
      digitalWrite(lights[val], HIGH);
      for(int j = 0; j < 4; j++){
        if (j != val){
          digitalWrite(lights[j], LOW);
        }
      }
      //lcd.setCursor(0,1);
      //lcd.print(val);
      delay(250);
      digitalWrite(lights[val], LOW);
      delay(250);
      }
    }
    
    int userInputs[4];
    while(counter<4){
      sensorValueX = analogRead(X_pin);
      sensorValueY = analogRead(Y_pin);
      if(sensorValueX>900 && !checked){
        
        userInputs[counter]=1;
        checked=true;
        //Serial.print("UP\n");
        counter++;
      }
      if (sensorValueX<400 && !checked){
        
        userInputs[counter]=3;
        checked=true;
        //Serial.print("DOWN \n");
        counter++;
      }
      if(sensorValueY>700 && !checked){
        
        userInputs[counter]=4;
        checked=true;
        //Serial.print("RIGHT \n");
        counter++;
      }
      if (sensorValueY<200 && !checked){
        
        userInputs[counter]=2;
        checked=true;
        //Serial.print("LEFT \n");
        counter++;
      }
      if(sensorValueY<700 && sensorValueY>200 && sensorValueX<900 && sensorValueY>400){
        checked = false;
      }
      delay(250);
    }
    
    correct=true;
    for (int i=0; i<4; i++){
      if(userInputs[i]!= rand_nums[i]){
        correct=false;
      }
    }
    
    while(correct && !audio){
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(LEFT, HIGH);
      digitalWrite(DOWN, HIGH);
      
      if(digitalRead(50) == HIGH){
        audio = true;
      }
    }
    
  if (correct && audio){
    digitalWrite(UP, LOW);
    digitalWrite(RIGHT, LOW);
    digitalWrite(LEFT, LOW);
    digitalWrite(DOWN, LOW);
    state = 0;
    timer = 0;
    lcd.clear();
    lcd.print("Hello!Set timer:");
    lcd.setCursor(0,1);
    lcd.print(timer);
    correct=false;
    audio = false;
    counter = 0;
  }else{
    counter = 0;
    }
    //digitalWrite(46, HIGH);
    //up pin 42
    
//Down pin 46
  
//Right pin 48 
 //Left pin 44
 
  break;
  
}
  
  
}
