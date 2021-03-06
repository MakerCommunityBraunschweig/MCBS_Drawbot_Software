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

#define X_INPUT            A3
#define Y_INPUT            A4
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


  db.enable_motors();
  db.setup_motors();
  
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


int xPos = 210, yPos = 238;

void loop () {

    res r = user_input(xPos, yPos);
    xPos = r.c1;
    yPos = r.c2;

    db.move_to_point_LERP(xPos,yPos);
    db.show_values();

    
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
