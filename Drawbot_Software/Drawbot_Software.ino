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

  delay(1000);

  db.home_all();
  db.set_delayUs(1000);
  delay(2000);
  db.move_by_angles(60,0);    // Grundstellung
  db.init_angles();           // Winkel auf Null setzen
  delay(2000);

  int xp[] = {250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250};
  int yp[] = {50,   60,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230};

  int l = sizeof(xp)/sizeof(xp[1]);   // Länge des Pfads bestimmen
  Serial.println(l);

  //db.move_path_XY(xp,yp,l);

  int x= 250;
  for(int x = 250; x <= 350; x+=10) {
    for(int y = 50; y <= 230; y+=10) {
      db.move_to_point_XY(x,y);
    }
  }
  
}


void loop () {

  // do nothing
  
}
  
void read_endstops() {

  Serial.print(digitalRead(X_MIN_PIN));
  Serial.print(digitalRead(X_MAX_PIN));
  Serial.print(digitalRead(Y_MIN_PIN));
  Serial.println(digitalRead(Y_MAX_PIN));

  delay(200);
  
}
