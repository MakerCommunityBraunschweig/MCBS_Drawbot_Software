#include "Drawbot.h"
#include <Arduino.h>
#include <stdio.h>

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

  // save old positions
  int M1_Old = M1_Pos;
  int M2_Old = M2_Pos;

  // update joint positions
  if(M1_Dir == FWD) {
    M1_Pos += s1;
  } else if (M1_Dir == BCKWD) {
    M1_Pos -= s1;
  }
  
  if(M2_Dir == FWD) {
    M2_Pos += s2;
  } else if (M2_Dir == BCKWD) {
    M2_Pos -= s2;
  }

  // ensure that the desired movement is possible
  bool possible = check_boundaries(M1_Pos, M2_Pos);
  if (possible == false) {
    Serial.println ("Position nicht anfahrbar! Grenzen werden verletzt!");
    M1_Pos = M1_Old;
    M2_Pos = M2_Old;
    return;
  }

  int m1_steps = 0, m2_steps = 0;
  while((m1_steps < s1) and (m2_steps < s2)) {
    move_step(1);
    move_step(2);
    m1_steps++;
    m2_steps++;
  }
  while(m1_steps < s1) {
    move_step(1);
    m1_steps++;
  }
  while(m2_steps < s2) {
    move_step(2);
    m2_steps++;
  } 
  
}

void Drawbot::set_delayUs(int del) {
  delayUs = del;
}

int Drawbot::get_delayUs() {
  return delayUs;
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

  int m1_dir, m2_dir;
  // set directions
  if(M1_Target > M1_Pos) {
    m1_dir = FWD;
  } else if (M1_Target < M1_Pos) {
    m1_dir = BCKWD;
  }
  if(M2_Target > M2_Pos) {
    m2_dir = FWD;
  } else if (M2_Target < M2_Pos) {
    m2_dir = BCKWD;
  }
  set_directions(m1_dir, m2_dir);

  while((M1_Target != M1_Pos) || (M2_Target != M2_Pos)) {
    
    if (M1_Target != M1_Pos) {
      move_steps(1,0);
    }
    if (M2_Target != M2_Pos) {
      move_steps(0,1);
    }
  
  }
  
  Serial.println("New pos: " + String(M1_Pos) + " | " + String(M2_Pos)); 

}

void Drawbot::set_path(int x[], int y[]) {
  
}

void Drawbot::move_path(int m1_pos[], int m2_pos[]) {

  int nop = sizeof(m1_pos);
  for (int i=0; i <= nop; i++) {
    set_target_values(m1_pos[i],m2_pos[i]);
    move_to_target();
    delay(2000); 
  }
  
}

int Drawbot::get_joint_values () {

  return M1_Pos, M2_Pos;
}

void Drawbot::home_all() {

  int oldDelay = get_delayUs();

  set_delayUs(1500);
  set_directions(BCKWD,BCKWD);
  
  while(digitalRead(X_MIN_PIN) and digitalRead(Y_MIN_PIN)) {
    move_step(1);
    move_step(2);
  }
  while(digitalRead(X_MIN_PIN)) {
    move_step(1);
  }
  while(digitalRead(Y_MIN_PIN)) {
    move_step(2);
  }
  set_joint_values(0,0);
  set_directions(FWD,FWD);
  Serial.println("Home all successfull!");

  set_delayUs(oldDelay);
  
}

void Drawbot::manual_mode() {

  #define STEP_SIZE 100
  #define DELAY_TIME 500
  
  home_all();
  set_delayUs(1000);
  Serial.println("Manual mode started! \n ----------------- \n ");

  set_directions(FWD,FWD);
  delay(2000);
  move_steps(300,300);
  Serial.println("Joint positions: " + String(M1_Pos) + " | " + String(M2_Pos));
  

  while(millis() < 500000) {
    if((digitalRead(X_MIN_PIN) == 0) and (digitalRead(Y_MIN_PIN)==0)) {
      // change directions
      invert_directions();
      Serial.println("Directions have changed!");
      delay(3000);
    }
    if(digitalRead(X_MIN_PIN) == 0) {
      move_steps(STEP_SIZE,0);
      Serial.println("New pos: " + String(M1_Pos) + " | " + String(M2_Pos));
      delay(500);
    }
    if(digitalRead(Y_MIN_PIN) == 0) {
      move_steps(0,STEP_SIZE);
      Serial.println("New pos: " + String(M1_Pos) + " | " + String(M2_Pos));
      delay(500);
    }

  }
  
}

void Drawbot::set_directions(int m1_newDir, int m2_newDir) {
  M1_Dir = m1_newDir;
  M2_Dir = m2_newDir;

  if (m1_newDir == FWD) {
    digitalWrite(X_DIR_PIN, HIGH);
  } else if (m1_newDir == BCKWD) {
    digitalWrite(X_DIR_PIN, LOW);
  }

  if (m2_newDir == FWD) {
    digitalWrite(Y_DIR_PIN, HIGH);
  } else if (m2_newDir == BCKWD) {
    digitalWrite(Y_DIR_PIN, LOW);
  }
  
}

void Drawbot::invert_directions() {
  M1_Dir = -M1_Dir;
  M2_Dir = -M2_Dir;
  set_directions(M1_Dir, M2_Dir);
}


bool Drawbot::check_boundaries(int m1, int m2) {

  float m1_max = 1.667 * m2 + 4000;  
  float m1_min = 2.333 * m2 - 1160;  
  
  float m2_min = 0.5 * m1 - 1820;  
  float m2_max = 0.5 * m1 + 242;

  bool valid = true;

  if ((m1 < m1_max) and (m1 > m1_min)) {
    // do nothing
  } else {
    Serial.println("Desired position of motor 1 invalid!");
    valid = false;
  }
  if ((m2 < m2_max) and (m2 > m2_min)) {
    // do nothing
  } else {
    Serial.println("Desired position of motor 2 invalid!");
    valid = false;
  }

  if (m1 <= 7000) {
    // do nothing
  } else {
    Serial.println("Motor 1 hat seine Grenze erreicht.");
    valid = false;
  }

  return valid; 
  
}
