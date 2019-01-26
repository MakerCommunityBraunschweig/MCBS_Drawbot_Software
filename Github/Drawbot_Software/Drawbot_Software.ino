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


Drawbot db;
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

  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);

  Serial.begin(9600);
  Serial.println("Neustart");


  float xE = 210, yE = 237;
  
  Kinematics::TF q;
  q = kin.solveIK_advanced(xE,yE);
  float q1 = q.c1;
  float q2 = q.c2;
  Serial.println(String(q1) + " | " + String(q2));

  delay(1000);

  db.home_all();
  db.set_delayUs(500);
  delay(2000);
  db.move_by_angles(90,15);    // Grundstellung
  db.init_angles();           // Winkel auf Null setzen
  delay(2000);
  db.set_delayUs(2500);
  db.move_to_point_XY(210,237);

}

char rx_byte = 0;
String rx_str = "";
boolean not_number = false;
int x_result, y_result;


void loop () {

   user_input();
  
  
  
}


void user_input() {

  int done = 0;
  Serial.print("Enter X: ");

  while(done == 0) {

    if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character
      
      if ((rx_byte >= '0') && (rx_byte <= '9')) {
        rx_str += rx_byte;
      }
      else if (rx_byte == '\n') {
        // end of string
        if (not_number) {
          Serial.println("Not a number");
        }
        else {
          x_result = rx_str.toInt();
          done = 1;
        }
        not_number = false;         // reset flag
        rx_str = "";                // clear the string for reuse
      }
      else {
        // non-number character received
        not_number = true;    // flag a non-number
      }
    } // end: if (Serial.available() > 0)
  }
  Serial.println(x_result);
  Serial.print("Enter Y: ");
  done = 0;
  while(done == 0) {

    if (Serial.available() > 0) {    // is a character available?
      rx_byte = Serial.read();       // get the character
      
      if ((rx_byte >= '0') && (rx_byte <= '9')) {
        rx_str += rx_byte;
      }
      else if (rx_byte == '\n') {
        // end of string
        if (not_number) {
          Serial.println("Not a number");
        }
        else {
          y_result = rx_str.toInt();
          done = 1;
        }
        not_number = false;         // reset flag
        rx_str = "";                // clear the string for reuse
      }
      else {
        // non-number character received
        not_number = true;    // flag a non-number
      }
    } // end: if (Serial.available() > 0)
  }
  Serial.println(y_result);

  db.move_to_point_XY(x_result,y_result);
  
}
