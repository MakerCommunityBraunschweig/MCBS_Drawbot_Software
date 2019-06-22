
void move2initpose(Drawbot db) {
  db.home_all();
  delay(2000);
  db.set_velocity(20);
  //db.moveX(90, 35);
  db.moveX(135, 80);
  db.init_values();
  db.show_values();
}


int x_min = x_start - 150;
int x_max = x_start + 50;
int y_min = y_start - 100;
int y_max = y_start + 50;


bool read_joystick() {
  int x_in = analogRead(X_INPUT);
  int y_in = analogRead(Y_INPUT);
  bool result = false;
  
  float dx = 1;
  float dy = 1;

  if (x_in > 600 && x_soll < x_max) {
    x_soll += dx;
    result = true;
  } 
  else if (x_in < 400 && x_soll > x_min) {
    x_soll -= dx;
    result = true;
  }
  if (y_in > 600 && y_soll < y_max) {
    y_soll += dy;
    result = true;
  }
  else if (y_in < 400 && y_soll > y_min) {
    y_soll -= dy;
    result = true;
  }
  
  Serial.println("x: " + String(x_soll) + " | y: " + String(y_soll)); 
  return result;
}

int read_potentiometer() {
  int poti = analogRead(POTI_PIN);  
  int res = map(poti, 0, 1023, 1, 10);
  return res;
}
