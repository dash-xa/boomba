/* This arduino project was created for Newhacks2020 
* uses 1 ardunio, LN298, 2 dc motors, rasperry pi, google speech to text api, joystick, and buttons
* to created a screaming alarm clock that runs away from you
* need to solve puzzle and say a safeword to stop it from buzzing
*/
//Motor Connections
#define EnA 9
#define EnB 10
#define In1 4
#define In2 5
#define In3 6
#define In4 7
//Buzzer notes' frequency
#define NOTE_FS4 370
#define NOTE_C4 262
#include <LiquidCrystal.h> //uses lcd
LiquidCrystal lcd(13, 12, 3, 2, 1, 0); //pins for lcd
int state; //state is used in the loop which tells us which state the arduino is in. 4 states: disarmed, armed, active and read
const int DISARMED = 0;
const int ARMED = 1;
const int ACTIVE = 2;
const int READ = 3;
//timer to hold the value for co
long timer;
int timerPrint;//the int that gets printed out
//button presses. button 1 increments timer, while button 2 initiates the countdown
bool press1;
bool press2;
//joystick (x and y are was switched around...i.e up and down was represented by x)
const int X_pin = A0; //analog pin connected to x output
const int Y_pin = A1; //analog pin connected to y output
int sensorValueX = 0; //records the sensorValue for x
int sensorValueY = 0; 
int x=0; //used for recording if up or downwards motion was detected
int y=0;//used for recording if right or left motion was detected

int buzzer = 11; //buzzer pin
//used for joystick game. Holds the random pattern of lights (up=1, left=2, down=3, right=4)
bool correct=false; //returns true if entered the correct pattern of lights
int rand_nums[4]; //holds random pattern of lights
int lights[] = {42, 44, 46, 48}; //output pins for the lights
int val;

const int UP = 42;
const int LEFT = 44;
const int DOWN = 46;
const int RIGHT = 48;

//random number for joysticks (not used)
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
  //randomizer
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
  pinMode(buzzer, OUTPUT);
 //initialize variables
  state = 0; 
  timer = 0;
  timerPrint = 0;
  press1 = false;
  press2 = false;
  lcd.begin(16, 2); //sets lcd (16 columns, 2 rows)
  lcd.print("Hello!Set timer:");//prints the first row on lcd
  lcd.setCursor(0,1);
  lcd.print(timer); //prints 0
  
  //Serial.begin(9600); messes with lcd
}
void goStraight(int duration, int speedA, int speedB)   //run both motors forward
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
void goBack(int duration, int speedA, int speedB)   //run both motors
{
  
  // turn on motor A
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  // turn on motor B
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
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
    
    if(digitalRead(53) == LOW && !press1){ //press button 1 to increment by 1 second
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
      
    if(digitalRead(52) == LOW){ //button 2 starts timer! sets to state ARMED
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("ARMED");
      state = 1;
     }
      
  break;
      
  case ARMED: //counts down till timer = 0
  
    while(timer >= 0){
      delay(500);
      lcd.setCursor(0,1);
      lcd.print(timer/1000);
      timer = timer-1000; //decrement timer
      delay(500);
      lcd.setCursor(0,1);
      lcd.print("                ");
    }
    state = 2; //time expired. Goes to next state ACTIVE
  break;
  
  case ACTIVE: //buzzes and moves around. Basic instructions but it can be expanded upon using collision sensors
    lcd.setCursor(0,0);
    lcd.print("ACTIVE");
    tone(buzzer,NOTE_FS4); //buzz
    delay(500);
    noTone(buzzer);
    delay(500);
    tone(buzzer,NOTE_C4); //buzz
    delay(500);
    noTone(buzzer);
    delay(500);
    goStraight(2000, 150, 150); //moves forward
    delay(500);
    goBack(2000, 150, 150); //moves backward
    state = 3; //goes to next state READ
  break;
  
  case READ: //continue to buzz, but now accepts user inputs to deactivate alarm
    tone(buzzer,NOTE_FS4); //plays note until deactivated
    lcd.clear();
    lcd.print("DISARM");
    
    if(!correct){ //if you dont get the joystick game correct
    for(int i = 0; i < 4; i++) { //displays a pattern of 4 led lights and saves to rand_nums so that it can be compared
      val = random(0,4);
      rand_nums[i] = val+1;
      digitalWrite(lights[val], HIGH);
      for(int j = 0; j < 4; j++){ //switches other lights off
        if (j != val){
          digitalWrite(lights[j], LOW);
        }
      }
      //lcd.setCursor(0,1);
      //lcd.print(val);
      delay(250);
      digitalWrite(lights[val], LOW); //switches itself off
      delay(250);
      }
    }
    int userInputs[4]; //array to hold user's input
    while(counter<4){ //reads user's input and puts into array userInputs.
      sensorValueX = analogRead(X_pin); 
      sensorValueY = analogRead(Y_pin);
      if(sensorValueX>900 && !checked){ //UP check
        
        userInputs[counter]=1;
        checked=true;
        //Serial.print("UP\n");
        counter++;
      }
      if (sensorValueX<400 && !checked){ //DOWN check
        
        userInputs[counter]=3;
        checked=true;
        //Serial.print("DOWN \n");
        counter++;
      }
      if(sensorValueY>700 && !checked){ // RIGHT CHECK
        
        userInputs[counter]=4;
        checked=true;
        //Serial.print("RIGHT \n");
        counter++;
      }
      if (sensorValueY<200 && !checked){  //LEFT CHECK
        
        userInputs[counter]=2;
        checked=true;
        //Serial.print("LEFT \n");
        counter++;
      }
      if(sensorValueY<700 && sensorValueY>200 && sensorValueX<900 && sensorValueY>400){ //doesn't record value if you dont move the joystick
        checked = false;
      }
      delay(250); //adds a bit of delay between inputs
    }
     //below checks if userInputs == rand_nums. Any deviations will result in a new rand_num to be generated thus they gotta input a new pattern
    correct=true;
    for (int i=0; i<4; i++){
      if(userInputs[i]!= rand_nums[i]){
        correct=false;
      }
    }
    //waits for audio input. LEDs used for joystick game all lights up
    while(correct && !audio){
      
      digitalWrite(UP, HIGH);
      digitalWrite(RIGHT, HIGH);
      digitalWrite(LEFT, HIGH);
      digitalWrite(DOWN, HIGH);
      if(digitalRead(50) == HIGH){
        audio = true;
      }
    }
    
  if (correct && audio){ //if both audio and joystick game was completed, then reset all values and then go back to disarmed state 
    digitalWrite(UP, LOW); //switches off led
    digitalWrite(RIGHT, LOW);
    digitalWrite(LEFT, LOW);
    digitalWrite(DOWN, LOW);
    state = 0;
    timer = 0;
    lcd.clear();
    lcd.print("Hello!Set timer:");
    lcd.setCursor(0,1);
    lcd.print(timer);
    correct = false;
    audio = false;
    counter = 0;
    noTone(buzzer);
  }else{ //if doesn't pass then restart counter then loop again
    counter = 0;
    }

  break;
  
}
  
  
}
