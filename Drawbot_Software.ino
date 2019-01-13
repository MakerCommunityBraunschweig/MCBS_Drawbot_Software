#include "Drawbot.h"

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

  
  db.set_delayUs(500);
  db.set_directions(FWD,FWD);
  db.home_all();
  
}


void loop () {

  // Define a path

  int m1_pos[] = {0, 300, 3000, 3700, 4000, 1200};
  int m2_pos[] = {0, 300, 200, 1200, 2200, 800};

//  db.manual_mode();
  
  db.set_delayUs(1000);                   // Defines the velocity 
  
  //db.move_path(m1_pos, m2_pos);

  db.move_linear_in_js(300,300);
  db.move_linear_in_js(2700,-100);

  delay(100000);
  
}

void read_endstops() {

  Serial.print(digitalRead(X_MIN_PIN));
  Serial.print(digitalRead(X_MAX_PIN));
  Serial.print(digitalRead(Y_MIN_PIN));
  Serial.println(digitalRead(Y_MAX_PIN));

  delay(200);
  
}
