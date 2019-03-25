#include "Path.h"
#include <Arduino.h>



void Path::add_values(int x, int y) {
  x_vals[0] = x;
  y_vals[0] = y;
}

void Path::print_path() {
  for (int i = 0; i < n_coords; i++) {
    Serial.print(String(pathnew[i].x) + " | " + String(pathnew[i].y));
  }
}

Path::Path () {

}
