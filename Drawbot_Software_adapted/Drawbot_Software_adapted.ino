#include "Drawbot.h"
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>


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
#define SIGNAL_PIN          3


Drawbot db = Drawbot();
Kinematics kin;


void setup() {
  
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(X_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);
  
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Neustart");



  db.setup_motors();
  db.home_all();

  delay(1000);
  db.set_velocities(10,20);
  db.move_by_angles(90, 30);
  db.init_values();
  db.disable_motors();
  

}


void loop () {

   // do nothing
  
}
