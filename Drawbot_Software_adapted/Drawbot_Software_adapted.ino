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
  
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Neustart");



  db.setup_motors();
  db.home_all();

  delay(2000);
  db.set_velocity(10);
  db.moveX(90, 35);
  db.init_values();
  db.show_values();
  delay(2000);
  db.set_velocity(10);
  db.move_to_point_XY(210, 238);  
  db.show_values();

  //db.disable_motors();
  

}


int xPos = 210, yPos = 238;

void loop () {

    res r = user_input(xPos, yPos);
    xPos = r.c1;
    yPos = r.c2;

    db.move_to_point_XY(xPos,yPos);
    db.show_values();

//  db.move_to_point_XY(200,238);
//  db.show_values();
//  delay(3000);
//  db.move_to_point_XY(210,238);
//  db.show_values();
//  delay(3000);
//  db.move_to_point_XY(210,228);
//  db.show_values();
//  delay(3000);
//  db.move_to_point_XY(200,228);
//  db.show_values();
//  delay(3000);

    
}

char rx_byte = 0;

res user_input(int xE, int yE) {

  int done = 0;
  Serial.print("Enter dir: ");

  while(done == 0) {

    if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character

      switch(rx_byte) {
        case 'r':  xE += 10;  break;
        case 'l':  xE -= 10;  break;
        case 'u':  yE += 10;  break;
        case 'd':  yE -= 10;  break;
      }
      done = 1;
    } 
  }

  res result = {xE, yE};
  return result;

}
