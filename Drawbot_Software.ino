#include "Drawbot.h"

#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define LED_PIN            13


Drawbot db;

void setup() {
  
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Test");

  
  db.set_joint_values(0,0);

}


void loop () {
  


  int period = 2000; 
  int delayUs = 1000;
  int triAmp = 500;
  
  //test_run(10*period,period,delayUs);
  //test_run_2(10*period,period,delayUs,triAmp);
  //MoveIt();
  
  
  db.set_delayUs(500);
 
  db.set_target_values(200,400);
  db.move_to_target();
  delay(2000);
  
  db.set_target_values(0,0);
  db.move_to_target();
  delay(2000);
  
  

}
