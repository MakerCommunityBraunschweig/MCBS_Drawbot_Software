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
#define POTI_PIN           A9
#define POTI_PIN_2         A5

#define X_INPUT            A4
#define Y_INPUT            A3
#define XY_BUTTON           0

const int x_start = 210;
const int y_start = 255;


Drawbot db = Drawbot();
Kinematics kin;

struct res {
  float c1, c2;
};


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

  pinMode(X_INPUT, INPUT);
  pinMode(Y_INPUT, INPUT);
  pinMode(XY_BUTTON, INPUT);
  pinMode(POTI_PIN, INPUT);
  
  digitalWrite(X_ENABLE_PIN, LOW);    
  digitalWrite(Y_ENABLE_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Neustart");

// Setup Commands

  db.setup_motors();
  move2initpose(db);

  delay(2000);
  db.set_velocity(10);
  db.set_acceleration(400);
  db.set_linear_speed();
  db.move_to_point_XY(x_start, y_start);  
  db.show_values();

}


float x_ist = x_start, y_ist = y_start;
float x_soll = x_start, y_soll = y_start;
double error = 0;



short tolerance = 5;
short velocity = 10;
short old_vel = 10;

void loop () {

    read_joystick();
    tolerance = read_poti_1();
    velocity = read_poti_2();
    if (velocity != old_vel) {
      db.set_velocity(velocity);
    }
    
    error = sqrt(pow(x_soll-x_ist, 2) + pow(y_soll-y_ist, 2));
    if (error > tolerance){
      db.move_to_point_XY(x_soll, y_soll);
      x_ist = x_soll;
      y_ist = y_soll;
      //db.show_values();
    }
    
}
