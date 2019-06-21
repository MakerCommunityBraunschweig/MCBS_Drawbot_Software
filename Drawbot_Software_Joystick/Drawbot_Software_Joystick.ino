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

#define Y_INPUT            A3
#define X_INPUT            A4
#define XY_BUTTON           0


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
  
  digitalWrite(X_ENABLE_PIN, LOW);    
  digitalWrite(Y_ENABLE_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Neustart");

  db.setup_motors();
  db.enable_motors();

  db.home_all();

  delay(2000);
  db.set_velocity(10);
  db.moveX(90, 35);
  db.init_values();
  db.show_values();
  delay(2000);
  db.set_velocity(10);
  db.move_to_point_XY(210, 255);  
  db.show_values();

  //db.disable_motors();
  

}


float x_ist = 210, y_ist = 238;
float x_soll = 210, y_soll = 238;
double error = 0;

int xmin = 160;
int xmax = 260;
int ymin = 188;
int ymax = 288;

void loop () {

  read_joystick();

  error = sqrt(pow(x_soll-x_ist, 2) + pow(y_soll-y_ist, 2));

  if (error > 1){
    db.move_to_point_XY(x_soll, y_soll);
    x_ist = x_soll;
    y_ist = y_soll;
    db.show_values();
  }

  if (!digitalRead(XY_BUTTON)){
    db.move_to_point_XY(x_soll, y_soll);
    x_ist = x_soll;
    y_ist = y_soll;
    db.show_values();
  }
  
  
}



bool read_joystick() {
  int x_in = analogRead(X_INPUT);
  int y_in = analogRead(Y_INPUT);
  bool result = false;
  
  float dx = 1;
  float dy = 1;

  if (x_in > 515 && x_soll < xmax) {
    x_soll += dx;
    result= true;
  } 
  else if (x_in < 505 && x_soll > xmin) {
    x_soll -= dx;
    result= true;
  }
  if (y_in > 505 && y_soll < ymax) {
    y_soll += dy;
    result= true;
  }
  else if (y_in < 495 && y_soll > ymin) {
    y_soll -= dy;
    result= true;
  }
  
  Serial.println("x: " + String(x_soll) + " | y: " + String(y_soll)); 
  return result;
}

char rx_byte = 0;

res user_input(int xE, int yE) {

  int done = 0;
  Serial.print("Enter dir: ");

  while(done == 0) {

    if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character

      switch(rx_byte) {
        case 'd':  xE += 10;  break;
        case 'a':  xE -= 10;  break;
        case 'w':  yE += 10;  break;
        case 's':  yE -= 10;  break;
        case 'e':  db.enable_motors();  break;
        case 'q':  db.disable_motors(); break;
        case 'h':  db.home_all(); break;
        case 'r':  db.set_velocity(10); db.moveX(90, 35); db.init_values(); break;
      }
      done = 1;
    } 
  }

  res result = {xE, yE};
  return result;

}
