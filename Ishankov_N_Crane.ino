/*
 * Description: Program for arduino to operate a crane using servos and a joystick
 * Author: Nicholas Ishankov
 * Date: 09/04/2019
 * Version: V1.0
 */
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;

int pos = 0;    // variable to store the servo position
const int analogInPinX = A0;
const int analogInPinY = A1;

const int analogOutPinX = 9;
const int analogOutPinY = 10;

int sensorValueX = 0;        // value read from the pot
int outputValueX = 0;        // value output to the PWM (analog out)

int sensorValueY = 0;        // value read from the pot
int outputValueY = 0;        // value output to the PWM (analog out)

int startX = 60; //starting positions
int startY = 160;

int xPickup = 0;
int yPickup = 0;

int xDrop = 0;
int yDrop = 0;

bool pickup;
bool automate;

void setup() {
  myservo1.attach(9);
  myservo2.attach(10);  // attaches the servo on pin 9/10 to the servo object

  pinMode(22,INPUT_PULLUP); //training button
  pinMode(12,INPUT_PULLUP); //auto button

  
  startX = 60;
  startY = 140;
  
  outputValueX = startX;
  outputValueY = startY;

  pickup = false;
  automate = false;
  
}

void loop() {
  
  // read the analog in value:
  sensorValueX = analogRead(analogInPinX);
  sensorValueY = analogRead(analogInPinY);

  //check if the user had pushed the joystick in a distinct direction
  if(sensorValueY<520){
    if(outputValueY>70){ //limit for the vertical arm
      outputValueY--; //decrease value to be output to servo
      }
    }
  else if(sensorValueY>530){
    if(outputValueY<170.5){ //limit for the vertical arm
      outputValueY++; //increase value to be output to servo
      }
    }
  if(sensorValueX<515){ 
    if(outputValueX>0){ //limit for the horizontal arm
      outputValueX--; //decrease value to be output to servo
      }
    }
  else if(sensorValueX>530){
    if(outputValueX<160){ //limit for the horizontal arm
      outputValueX++; //increase value to be output to servo
      }
    }

    //output values to the servos
    myservo1.write(outputValueX);
    myservo2.write(outputValueY);

  //record button presses to save pickup location
  if(digitalRead(22) == LOW && pickup == false){
    pickup = true;
    xPickup = outputValueX;
    yPickup = outputValueY;
    delay(100);
    
    }
  //save drop location
  if(digitalRead(22) == LOW && pickup == true){
    pickup == false;
    xDrop = outputValueX;
    yDrop = outputValueY;
    delay(100);
    
    }

  //record a button press to enter automatic mode
  if(digitalRead(12) == LOW && automate == false){
    automate = true;
    //move to the idle position
    myservo1.write(startX);
    myservo2.write(startY);
    delay(500);
    myservo1.write(xPickup+15); //move slightly past the pickup point to get around the hook on weight
    delay(1000);
    myservo2.write(yPickup); //move to desired height (pickup)
    delay(1000);
    myservo1.write(xPickup); //move to desired ground position (pickup)
    delay(1000);

    myservo2.write(startY); //return to the idle height to move weight
    delay(1000);
    myservo1.write(xDrop); //move to desired ground position (drop)
    delay(1500);
    myservo2.write(yDrop); //move to desired height (drop)
    delay(1000);
    myservo1.write(xDrop+15); //move slightly past the drop point to not knock weight over
    delay(1000);

    //return to idle and overwrite output values
    outputValueY = startY; 
    outputValueX = startX;

    yPickup = 0;
    xPickup = 0;
    yDrop = 0;
    xDrop = 0;

    myservo2.write(startY);
    delay(500);
    myservo1.write(startX);
    
    automate = false;
    }
    
  delay(20);
  

}
