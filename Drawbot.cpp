#include "Drawbot.h"
#include <Arduino.h>
#include <stdio.h>
#include <math.h>

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


// -----------  get and set methods -------------//

void Drawbot::set_joint_values (int q1, int q2) {
  M1_Pos = q1;
  M2_Pos = q2;
}

int Drawbot::get_joint_value (int qi) {
  switch(qi) {
    case 1: return M1_Pos;
    case 2: return M2_Pos;
  }
}

void Drawbot::set_delayUs(int del) {
  delayUs = del;
}

int Drawbot::get_delayUs() {
  return delayUs;
}

void Drawbot::set_directions(int m1_newDir, int m2_newDir) {
  // set new direction variables
  M1_Dir = m1_newDir;
  M2_Dir = m2_newDir;

  // set direction pins accordingly
  switch(m1_newDir) {
    case FWD:   digitalWrite(X_DIR_PIN, HIGH); break;
    case BCKWD: digitalWrite(X_DIR_PIN, LOW); break;
  }
  switch(m2_newDir) {
    case FWD:   digitalWrite(Y_DIR_PIN, HIGH); break;
    case BCKWD: digitalWrite(Y_DIR_PIN, LOW); break;
  }
}

void Drawbot::invert_directions() {
  set_directions(-M1_Dir, -M2_Dir);
}


// -------- END: get and set methods ---------------- //



// ----------- START: joint movement methods -------- //

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



void Drawbot::move_steps (int s1, int s2) {

  // update joint positions; s1 and s2 can be both positive and negative
  int M1_New_Pos = M1_Pos + s1;
  int M2_New_Pos = M2_Pos + s2;

  // ensure that the desired movement is possible
  bool possible = check_boundaries(M1_New_Pos, M2_New_Pos);
  if (possible == false) {
    Serial.println ("Position nicht anfahrbar! Grenzen werden verletzt!");
    return;
  }

  int m1_steps = 0,       m2_steps = 0;                 // count the steps
  s1 = abs(s1);           s2 = abs(2);                  // make step numbers positive
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

  M1_Pos = M1_New_Pos;    M2_Pos = M2_New_Pos;        // update motor positions
  
}





void Drawbot::move_path(int m1_pos[], int m2_pos[]) {

  int nop = sizeof(m1_pos)/sizeof(int);              // get number of points
  nop = 5;                                           // set nop manually, if the method above is not working                 
  int m1_steps, m2_steps;

  for(int i = 0; i < nop; i++) {
    m1_steps = m1_pos[i] - M1_Pos;
    m2_steps = m2_pos[i] - M2_Pos;
    move_linear_in_js(m1_steps, m2_steps);
    delay(1000);
    Serial.println(String(M1_Pos) + " | " + String(M2_Pos));
  }
  
}


void Drawbot::move_linear_in_js (int s1, int s2) {

  // Set directions
  int m1_dir = copysign(1.0, s1);
  int m2_dir = copysign(1.0, s2);
  set_directions(m1_dir, m2_dir);
  
  // Set new motor positions
  M1_Pos += s1;
  M2_Pos += s2;

  int nos = max(s1,s2);                                       // Number of steps
  float m1_spi = (float) s1/nos,   m2_spi = (float) s2/nos;   // Steps per iteration
  int m1_steps = 0,                m2_steps = 0;              // Count the steps

  // Berechnung & Ausführung
  for(int i = 1; i <= nos; i++){    
    if (m1_steps < i*m1_spi) {
      move_step(1);
      m1_steps++;
    }
    if (m2_steps < i*m2_spi) {
      move_step(2);
      m2_steps++;
    } 
  }

}


void Drawbot::home_all() {

  int original_delay = get_delayUs();

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

  set_delayUs(original_delay);
  
}


// ============== END: movement methods ===================//

void Drawbot::manual_mode() {

  #define STEP_SIZE 100
  #define DELAY_TIME 500
  #define MAXTIME 5000000
  
  home_all();
  set_delayUs(1000);
  Serial.println("Manual mode started! \n ----------------- \n ");

  set_directions(FWD,FWD);
  delay(2000);
  move_steps(300,300);    // inital position
  Serial.println("Joint positions: " + String(M1_Pos) + " | " + String(M2_Pos));

  while(millis() < MAXTIME) {
    if(!digitalRead(X_MIN_PIN) and !digitalRead(Y_MIN_PIN)) {
      invert_directions();
      Serial.println("Directions have changed!");
      delay(3000);
    }
    if(!digitalRead(X_MIN_PIN)) {
      move_steps(STEP_SIZE,0);
      Serial.println("New pos: " + String(M1_Pos) + " | " + String(M2_Pos));
      delay(500);
    }
    if(!digitalRead(Y_MIN_PIN)) {
      move_steps(0,STEP_SIZE);
      Serial.println("New pos: " + String(M1_Pos) + " | " + String(M2_Pos));
      delay(500);
    }

  }
  
}



bool Drawbot::check_boundaries(int m1, int m2) {

  float m1_max = 1.667 * m2 + 4000;  
  float m1_min = 2.333 * m2 - 1160;  
  float m2_min =   0.5 * m1 - 1820;  
  float m2_max =   0.5 * m1 + 242;

  bool valid = true;
  int tolerance = 100;              // define tolerance to even out interpolation errors

  if ((m1 < m1_max + tolerance) and (m1 > m1_min - tolerance)) {
    // do nothing
  } else {
    Serial.println("Desired position of motor 1 invalid!");
    valid = false;
  }
  if ((m2 < m2_max + tolerance) and (m2 > m2_min - tolerance)) {
    // do nothing
  } else {
    Serial.println("Desired position of motor 2 invalid!");
    valid = false;
  }

  if (m1 > 7000) {
    Serial.println("Motor 1 hat seine Grenze erreicht.");
    valid = false;
  }

  return valid; 
  
}

