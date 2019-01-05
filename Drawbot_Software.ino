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
  Serial.println("Test");

  
  db.set_delayUs(200);
  db.isHomed = false;

}


void loop () {

  
  int period = 2000; 
  int delayUs = 500;
  int triAmp = 500;
  
  //test_run(10*period,period,delayUs);
  //test_run_2(10*period,period,delayUs,triAmp);
  //int m1_pos[] = {0, 800, 400, 300};
  //int m2_pos[] = {0, 100, 500, 300};
  //db.move_path(m1_pos, m2_pos);

  if (!db.isHomed) {
    db.home_all();
    db.isHomed = true;
  }

  delay(10000);
  int a = db.get_joint_values();
  
}

void read_endstops() {

  Serial.print(digitalRead(X_MIN_PIN));
  Serial.print(digitalRead(X_MAX_PIN));
  Serial.print(digitalRead(Y_MIN_PIN));
  Serial.println(digitalRead(Y_MAX_PIN));

  delay(200);
  
}
