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
#define TRANSLATION 4.5                    // Übersetzung



BasicStepperDriver stepperX(MOTOR_STEPS, X_DIR_PIN, X_STEP_PIN, X_ENABLE_PIN);
BasicStepperDriver stepperY(MOTOR_STEPS, Y_DIR_PIN, Y_STEP_PIN, Y_ENABLE_PIN);
MultiDriver controller(stepperX, stepperY);


Drawbot::Drawbot() {

  theta_1 = 0;
  theta_2 = 0;
  motors_rpm = 50;
  motors_acc = 500;
  mtype = 1;          // linear movement

}


/*
 *   Basic methods
 */



void Drawbot::setup_motors() {

  stepperX.begin(motors_rpm, MICROSTEPS);
  stepperY.begin(motors_rpm, MICROSTEPS);

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

void Drawbot::set_acceleration(int acc) {

    stepperX.setSpeedProfile(motors_acc, motors_acc);
    stepperY.setSpeedProfile(motors_acc, motors_acc);

}

void Drawbot::set_linear_speed() {

    stepperX.setSpeedProfile(stepperX.LINEAR_SPEED);
    stepperY.setSpeedProfile(stepperY.LINEAR_SPEED);
 
}

void Drawbot::set_constant_speed() {
    stepperX.setSpeedProfile(stepperX.CONSTANT_SPEED);
    stepperY.setSpeedProfile(stepperY.CONSTANT_SPEED);
}


void Drawbot::set_velocity(short rpm) {

  motors_rpm = rpm;
  stepperX.setRPM(rpm);
  stepperY.setRPM(rpm);

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
  Serial.println("Motorwinkel: q1 = " + String(theta_1) + "°, q2 = " + String(theta_2) + "°");
  Serial.println("Position:     x = " + String(x_global) + "mm, y = " + String(y_global) + "mm");

}




/*
 *   Movement methods
 */


void Drawbot::move_to_point_XY(float xE, float yE) {
  long i = millis();
  Kinematics::TF q;
  q = kin.solveIK(xE, yE);
  float delta_1 = q.c1 - theta_1;
  float delta_2 = q.c2 - theta_2;
  //Serial.print("Rechenzeit: "); Serial.print(millis()-i); Serial.println("ms");
  i = millis();

  controller.rotate(TRANSLATION * delta_1, TRANSLATION * delta_2);
  theta_1 += delta_1;
  theta_2 += delta_2;
  
  //Serial.print("Bewegungszeit: "); Serial.print(millis()-i); Serial.println("ms");

}


void Drawbot::moveA(float delta_1, float delta_2) {
  controller.rotate(TRANSLATION * delta_1, TRANSLATION * delta_2);
  theta_1 += delta_1;
  theta_2 += delta_2;
}


void Drawbot::home_all() {
  set_constant_speed();

  while (digitalRead(X_MIN_PIN) and digitalRead(Y_MIN_PIN)) {
    controller.move(-30, -30);
  }
  while (digitalRead(X_MIN_PIN)) {
    controller.move(-30, 0);
  }
  while (digitalRead(Y_MIN_PIN)) {
    controller.move(0, -30);
  }
  init_values();

  Serial.println("Home all successfull!");
  set_linear_speed();

}
