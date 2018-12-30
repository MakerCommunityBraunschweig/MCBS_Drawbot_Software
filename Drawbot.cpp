#include "Drawbot.h"
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

void Drawbot::set_joint_values (int q1, int q2) {
  M1_Pos = q1;
  M2_Pos = q2;
}

void Drawbot::set_target_values (int t1, int t2) {
  M1_Target = t1;
  M2_Target = t2;
}

void Drawbot::move_steps (int s1, int s2) {
  
  digitalWrite(X_DIR_PIN, HIGH);

  int step_count;
  for(int i = 0; i <= s1; i++) {
    move_step(1);
    step_count = i;
    Serial.println(step_count);
  } 
}

void Drawbot::set_delayUs(int del) {
  delayUs = del;
}

void Drawbot::move_step(int motor) {

  int pin;
  switch(motor) {
    case 1: pin = X_STEP_PIN; break;
    case 2: pin = Y_STEP_PIN; break;
  }
  digitalWrite(pin, HIGH);
  delayMicroseconds(delayUs);
  digitalWrite(pin, LOW);
  delayMicroseconds(delayUs);
}


void Drawbot::move_to_target() {

  // set directions
  if(M1_Target > M1_Pos) {
    digitalWrite(X_DIR_PIN, HIGH);
  } else if (M1_Target < M1_Pos) {
    digitalWrite(X_DIR_PIN, LOW);
  }
  if(M2_Target > M2_Pos) {
    digitalWrite(Y_DIR_PIN, HIGH);
  } else if (M2_Target < M2_Pos) {
    digitalWrite(Y_DIR_PIN, LOW);
  }

  while((M1_Target != M1_Pos) || (M2_Target != M2_Pos)) {
    if (M1_Target > M1_Pos) {
      move_step(1);
      M1_Pos++;
    }
    if (M1_Target < M1_Pos) {
      move_step(1);
      M1_Pos--;
    }
    if (M2_Target > M2_Pos) {
      move_step(2);
      M2_Pos++;
    }
    if (M2_Target < M2_Pos) {
      move_step(2);
      M2_Pos--;
    } 
  
  }
  
  Serial.println("New pos: " + String(M1_Pos) + " | " + String(M2_Pos)); 

 
}

void Drawbot::define_path() {
  int path[2][2];
  path[0][0] = 0;
  path[0][1] = 0;
  path[1][0] = 100;
  path[1][1] = 200;

}

int Drawbot::get_values () {
  return XPos, YPos;
}
