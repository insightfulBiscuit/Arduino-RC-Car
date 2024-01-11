#include <Servo.h> 
#include <Stepper.h>

const int joyXPin = 0;
const int joyYPin = 1;
const int joyPush = 2;

const int UDLEDPin = 5;
const int LRLEDPin = 3;

const int stepsPerRevolution = 100;

bool inMotion;

int moveCount;
int stopCount;
int turnBack;
int servoVal;

Servo servo;
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  Serial.begin(9600);
    
  pinMode(UDLEDPin, OUTPUT);
  pinMode(LRLEDPin, OUTPUT);
    
  pinMode(joyPush, INPUT);
  digitalWrite(joyPush, HIGH);
    
  servo.attach(LRLEDPin);

  inMotion = false;
  
  moveCount = 0;
  stopCount = 0;
  turnBack = 0;

  myStepper.setSpeed(200);
  myStepper.step(stepsPerRevolution);
}

void loop() {
  Serial.print("X Axis: ");
  Serial.print(analogRead(joyXPin), DEC);
  Serial.print("  |  ");
  Serial.print("Y Axis: ");
  Serial.print(analogRead(joyYPin), DEC);
  Serial.print("  |  ");
  Serial.print("PUSH: ");
  Serial.println(digitalRead(joyPush), DEC);

  analogWrite(UDLEDPin, analogRead(joyYPin)/4);

  if (analogRead(joyXPin) > 600){
    Serial.println("MOVING UP");

    if (digitalRead(joyPush) != 0){
      inMotion = true;
      turnBack = 0;
    
//      if (joyPush == 0){
//        myStepper.setSpeed(200);
//        myStepper.step(stepsPerRevolution);
//        stopCount = 0;
//      }
      
      if (moveCount == 0){
        myStepper.setSpeed(200);
        myStepper.step(stepsPerRevolution);
        moveCount = 1;
      }
    }

    analogWrite(UDLEDPin, analogRead(joyXPin)/4);
  }
  else if (analogRead(joyXPin) < 400){
    Serial.println("MOVING DOWN");

    if (digitalRead(joyPush) != 0){
      inMotion = true;
      turnBack = 0;
  
//      if (joyPush == 0){
//        myStepper.setSpeed(200);
//        myStepper.step(stepsPerRevolution);
//        stopCount = 0;
//      }
  
      if (moveCount == 0){
        myStepper.setSpeed(200);
        myStepper.step(stepsPerRevolution);
        moveCount = 1;
      }
    }

    analogWrite(UDLEDPin, analogRead(joyXPin)/4);
  }
  else{
    inMotion = false;
    moveCount = 0;
    
    if (turnBack == 0){
      myStepper.setSpeed(200);
      myStepper.step(-stepsPerRevolution);
      turnBack = 1;
    }
    
    analogWrite(UDLEDPin, 0);
  }


  if (digitalRead(joyPush) == 0){
    if (inMotion){
      if (stopCount == 0){
        myStepper.setSpeed(200);
        myStepper.step(-stepsPerRevolution * 2);
        stopCount = 1;
      }
    }
    else{
      if (stopCount == 0){
        myStepper.setSpeed(200);
        myStepper.step(-stepsPerRevolution);
        stopCount = 1;
      }
    }
  }
  else{
    if (stopCount == 1){
      myStepper.setSpeed(200);
      myStepper.step(stepsPerRevolution);
      stopCount = 0;
    }
  }
  
  if (analogRead(joyYPin) > 600){
    Serial.println("MOVING RIGHT");
    servo.write(0);
    delay(100);
  }
  else if(analogRead(joyYPin) < 400){
    Serial.println("MOVING LEFT");
    servo.write(180);
    delay(100);
  }
  else{
    Serial.println("SERVO NEUTRAL");
    servo.write(90);
    delay(100);
  }

  delay(100);

}
