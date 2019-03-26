#include "Drawbot.h"
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
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

#define MOTOR_STEPS 200
#define MICROSTEPS 16
#define TRANSL 4.5                    // Übersetzung

#define MOTOR_ACCEL 200
#define MOTOR_DECEL 100



BasicStepperDriver stepperX(MOTOR_STEPS, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN);
BasicStepperDriver stepperY(MOTOR_STEPS, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN);
MultiDriver controller(stepperX, stepperY);



Drawbot::Drawbot() {

  theta_1 = 0;
  theta_2 = 0;
  motors_rpm = 50;
  x_df = 0;
  y_df = 0;
  x_global = 0;
  y_global = 0;
  
}


void Drawbot::setup_motors() {

  stepperX.begin(motors_rpm, MICROSTEPS);
  stepperY.begin(motors_rpm, MICROSTEPS);
  stepperX.setSpeedProfile(stepperX.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  stepperY.setSpeedProfile(stepperY.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);

  stepperX.setEnableActiveState(LOW);
  stepperY.setEnableActiveState(LOW);
  enable_motors();

}


void Drawbot::enable_motors() {

  stepperX.enable();
  stepperY.enable();
  
}


void Drawbot::disable_motors() {

  stepperX.disable();
  stepperY.disable();
  
}


void Drawbot::set_velocity(int rpm) {

  motors_rpm = rpm;
  stepperX.begin(rpm, MICROSTEPS);
  stepperY.begin(rpm, MICROSTEPS);
  
}


void Drawbot::set_velocities(int x_rpm, int y_rpm) {

  stepperX.begin(x_rpm, MICROSTEPS);
  stepperY.begin(y_rpm, MICROSTEPS);
  
}


void Drawbot::move_by_angles(float delta_1, float delta_2) {
  controller.rotate(TRANSL*delta_1, TRANSL*delta_2);
  theta_1 += delta_1;
  theta_2 += delta_2;
}


void Drawbot::moveX(float delta_1, float delta_2) {

  float a1 = abs(delta_1);
  float a2 = abs(delta_2);

  if (delta_1 > delta_2) {
    set_velocities(motors_rpm, motors_rpm*(a2/a1));
  } else if (delta_1 < delta_2) {
    set_velocities(motors_rpm*(a1/a2), motors_rpm);
  } else {
    set_velocities(motors_rpm, motors_rpm);
  }
  
  controller.rotate(TRANSL*delta_1, TRANSL*delta_2);
  theta_1 += delta_1;
  theta_2 += delta_2;
}



void Drawbot::init_values() {
  theta_1 = 0;
  theta_2 = 0;
  x_df = 0;
  y_df = 0;
  Kinematics::TF global_pos;
  global_pos = kin.solveFK(0, 0);
  x_global = global_pos.c1;
  y_global = global_pos.c2;
}


void Drawbot::show_values() {

  Kinematics::TF global_pos;
  global_pos = kin.solveFK(theta_1, theta_2);
  x_global = global_pos.c1;
  y_global = global_pos.c2;
  Serial.println("- - - - - - - - - - - - - - - - - - - - - - ");
  Serial.println("Motorwinkel: q1 = " + String(theta_1)+ "°, q2 = " + String(theta_2) + "°");
  Serial.println("Pos. Lokal : x_DF = " + String(x_df) + "mm, y_DF = " + String(y_df) + "mm"); 
  Serial.println("Pos. Global: x_gl = " + String(x_global) + "mm, y_gl = " + String(y_global) + "mm"); 

  
}


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




// ----------- START: joint movement methods -------- //



void Drawbot::move_to_point_XY(float xE, float yE) {

  Kinematics::TF q;
  q = kin.solveIK(xE, yE);
  float q1 = q.c1;
  float q2 = q.c2;
  float delta_1 = q1 - theta_1;
  float delta_2 = q2 - theta_2;
  moveX(delta_1, delta_2);
  theta_1 = q1;
  theta_2 = q2;
  
}


void Drawbot::move_to_angles(float q1, float q2) {
  
  float delta_1 = q1 - theta_1;
  float delta_2 = q2 - theta_2;

  int m1 = round(delta_1*DEG);
  int m2 = round(delta_2*DEG);
  
  Serial.println("Sollwinkel: " + String(theta_1) + " | " + String(theta_2));
  Serial.println("Soll-Steps: " + String(m1) + " | " + String(m2));

  move_linear_in_js(m1, m2);
  theta_1 = q1;
  theta_2 = q2;
  
}




void Drawbot::home_all() {

  set_velocity(10);
  
  while(digitalRead(X_MIN_PIN) and digitalRead(Y_MIN_PIN)) {
    controller.move(-1, -1);
  }
  while(digitalRead(X_MIN_PIN)) {
    controller.move(-1, 0);
  }
  while(digitalRead(Y_MIN_PIN)) {
    controller.move(0, -1);
  }
  init_values();

  Serial.println("Home all successfull!");

  
}


// ============== END: movement methods ===================//





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
