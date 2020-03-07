/*
 * Description: Alarm system program that uses uses a buzzer, lcd, and leds to communicate with the user, and uses a keypad, button, motion sensor, and photo resistor to transition from state to state. Stores a 4 digit code, that can be modified.
 * Date: 22/3/2019
 * Author: Nicholas Ishankov
 * Version: V1.0
 */
const int ARMED = 0;
const int DISARMED = 1;
const int GRACE = 2;
const int TRIPPED = 3;
const int READ = 4;
int state;
const int COL1 = 45;
const int COL2 = 41;
const int COL3 = 49;
const int ROW1 = 43;
const int ROW2 = 53;
const int ROW3 = 51;
const int ROW4 = 47;
const int LED1 = 25;
const int LED2 = 23;
const int LED3 = 24;
int total;
int pass;
int newInput;
int prevInput;
long graceTime;
long displayTime; 
int graceCount;
bool passcode;
bool readcode;
bool show;
int sensorValue;
#include <LiquidCrystal.h>                // include the library code
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);      // select the pins used on the LCD panel

#define NOTE_C4 262

void setup() {

  pinMode(ROW1,INPUT_PULLUP);
  pinMode(ROW2,INPUT_PULLUP);
  pinMode(ROW3,INPUT_PULLUP);
  pinMode(ROW4,INPUT_PULLUP);
  pinMode(50,INPUT_PULLUP); //button
  pinMode(48, INPUT); //motion sensor
  pinMode(COL1,OUTPUT);
  pinMode(COL2,OUTPUT);
  pinMode(COL3,OUTPUT);
  digitalWrite(COL1,HIGH);
  digitalWrite(COL2,HIGH);
  digitalWrite(COL3,HIGH);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(22, OUTPUT); 
  pinMode(52,OUTPUT); //buzzer
  Serial.begin(9600);
  total = 0;
  pass = 1111;
  lcd.begin(16, 2);                     // start the LCD with 2 rows and 16 columns
  lcd.print("Enter passcode:");                  // print a generic message to the LCD
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(22,HIGH); //LED for photoresistor  
  state = DISARMED; // starting state
  newInput = -1; //records user input value
  prevInput = newInput; //stores the users input value
  displayTime = 0; 
  passcode = false; //passcode not entered yet
  readcode = false; //no password update yet
  show = false; //nothing to display yet f
}

void loop() {
  newInput = -1; //reset the user value while not pressing any keys

  sensorValue = analogRead(A9); //light sensor records light level constantly
  
  digitalWrite(COL1,LOW);
  if (digitalRead(ROW1) == LOW){ //pressed 1
    tone(52,NOTE_C4);
    newInput = 1;
   
  }
  if (digitalRead(ROW2) == LOW){ //pressed 4
    tone(52,NOTE_C4); 
    newInput = 4;  
    
  }
  if (digitalRead(ROW3) == LOW){ //pressed 7
    tone(52,NOTE_C4);
    newInput = 7;
    

  }
  if (digitalRead(ROW4) == LOW){ //pressed *
    tone(52,NOTE_C4);
    displayTime = millis() + 1000; //start a timer that is used to display a message
    newInput = '*';  

  }  
  digitalWrite(COL1,HIGH);
  digitalWrite(COL2,LOW);
  if (digitalRead(ROW1) == LOW){ //pressed 2
    tone(52,NOTE_C4);
    newInput = 2;
    
  }
  if (digitalRead(ROW2) == LOW){ //pressed 5
    tone(52,NOTE_C4);
    newInput = 5;
    
  }
  if (digitalRead(ROW3) == LOW){ //pressed 8
    tone(52,NOTE_C4);
    newInput = 8;
    
  }
  if (digitalRead(ROW4) == LOW){ //pressed 0
    tone(52,NOTE_C4);
    newInput = 0;
    
  }    
  digitalWrite(COL2,HIGH);
  digitalWrite(COL3,LOW); 
  if (digitalRead(ROW1) == LOW){ //pressed 3
    tone(52,NOTE_C4);
    newInput = 3;
    
  }
  if (digitalRead(ROW2) == LOW){ //pressed 6
    tone(52,NOTE_C4);
    newInput = 6;
    
  }
  if (digitalRead(ROW3) == LOW){ //pressed 9
    tone(52,NOTE_C4);
    newInput = 9;
    
  }
  if (digitalRead(ROW4) == LOW){ //pressed #
    tone(52,NOTE_C4);
    displayTime = millis() + 1000; //set display timer
    newInput = '#';
    
  }      
  digitalWrite(COL3,HIGH);

  if(newInput!=prevInput && newInput != -1) { //if the users input has changed and it isnt -1 (nothing pressed)
      if(newInput =='*'){ //user pressed star, wants to check if password is correct and arm/disarm
        if(pass == total){ //is password correct display message and set a boolean variable to true to be used to change state
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.setCursor(0, 0);                  // set the cursor to column 0, line 1 (second row, since counting begins with 0):
            lcd.print("Code correct");
            passcode = true;
            show = true;
          }else{ //if the password was incorrect just display a message
            lcd.setCursor(0, 0);
            lcd.clear();
            lcd.setCursor(0, 0);                  // set the cursor to column 0, line 1 (second row, since counting begins with 0):
            lcd.print("Code rejected");
            passcode = false;
            show = true;
          }
          total = 0; //reset total, as user has just checked a password and needs to enter something again
        }else if(newInput =='#'){ //if user wants to update the password
          if (state == DISARMED){ //if in disarmed state
          lcd.setCursor(0, 0);
          lcd.clear();
          readcode = true; //check boolean that will change state  
          show = true;
          }else if(state == READ){ //if they pressed the button to save the current password
            passcode = true;
            show = true;
            }
          else{
          lcd.setCursor(0, 0);
          lcd.clear();
          lcd.setCursor(0, 0);                  // set the cursor to column 0, line 1 (second row, since counting begins with 0):
          lcd.print("Disarm system");
          readcode = false;
          show = true;
         }
          }else if(total < 1000){ //make sure that the password is not too long before updating the total 
            total = total*10;
            total = total + newInput; //update the total
            lcd.setCursor(0, 1);                  // set the cursor to column 0, line 1 (second row, since counting begins with 0)
            lcd.print(total);
            
           }
      }

   if(millis() > displayTime && show == true){
      lcd.setCursor(0, 0);                  // set the cursor to column 0, line 1 (second row, since counting begins with 0):
      lcd.print("Enter passcode:");
      lcd.setCursor(0, 1);
      lcd.print("               ");
      show = false;
    }
    
    prevInput = newInput; //overwrite previous input with current new input
    noTone(52); //turn tone off
  
  switch(state){

    case DISARMED:
       digitalWrite(LED1, HIGH); //only green light on
       digitalWrite(LED2, LOW);
       digitalWrite(LED3, LOW);
       if(passcode){ //if the user entered the correct password 
          state = ARMED; //arm
          passcode = false;
          lcd.setCursor(0, 1);
          lcd.print("Armed");
        }

        if(readcode){ //if user wants to update the password change state to read
          state = READ; 
          }
       
    break;

    case ARMED:
       digitalWrite(LED1, LOW); 
       digitalWrite(LED2, LOW);
       digitalWrite(LED3, HIGH); //only red light on
       if(passcode){
          state = DISARMED; //disarm
          lcd.setCursor(0, 1);
          passcode = false;
          lcd.print("Disarmed");
        }

        if(digitalRead(50) == LOW){ //if the user pressed the button
          state = GRACE; //enter grace state 
          graceTime = millis()+1000; //set the grace timer with a 1 second duration
          graceCount = 0; //set the grace counter 
          }else if(digitalRead(48) == HIGH){ //if user sets off motion sensor
          state = GRACE;
          graceTime = millis()+1000;
          graceCount = 0;
          }else if(sensorValue < 700){ //if the user trips the light sensor
          state = GRACE;
          graceTime = millis()+1000;
          graceCount = 0;
          }

        
    break;

    case GRACE:

       lcd.setCursor(0, 0);
       //lcd.print("                ");
       lcd.print("Arming in 30s     "); //display warning message

       if(passcode){ //if the user enters the correct password 
          state = DISARMED; //disarm
          lcd.setCursor(0, 1);
          passcode = false;
          lcd.print("Disarmed");
          }

        if(millis()<graceTime-500){ //half a second into the time flash light on and start tone
          digitalWrite(LED3, HIGH);
          tone(52,NOTE_C4);
          
          }else if(millis()<graceTime){ //in second half of the timer turn off note and light 
            digitalWrite(LED3, LOW);
            noTone(52);
          }else{
            graceTime = millis()+1000; //once the timer expires start it again and increase the counter to mark a second passing 
            graceCount++;
            }
        
        if(graceCount == 30){ //if the timer hits 30, 30 seconds have passed
            state = TRIPPED; //system not disarmed in time, enter tripped state
          }
        
    break;

    case TRIPPED:

      tone(52,NOTE_C4); //alarm sounds
      lcd.setCursor(0, 0);
      lcd.print("Intruder Alert"); //message displays
      
      if(passcode){ //if the password was entered correctly 
          state = DISARMED; //disarm
          lcd.setCursor(0, 1);
          passcode = false;
          lcd.print("Disarmed");
          noTone(52);
          }

    break;

    case READ:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH); //turn yellow light on
      digitalWrite(LED3, LOW);
      
      lcd.setCursor(0, 0);
      lcd.print("Enter new pass:"); //message
      pass = total; //update the password with the total the user enters 
       
      if(passcode){ //if the user confirms the password return to disarmed state
          state = DISARMED; //disarm
          lcd.setCursor(0, 0);
          passcode = false;
          readcode = false;
          lcd.print("Disarmed, saved");
          total = 0; //erase total
          }
          
    break;

    
  }
}
