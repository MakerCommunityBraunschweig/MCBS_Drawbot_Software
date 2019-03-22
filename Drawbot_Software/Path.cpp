#include "Path.h"
#include <Arduino.h>



void Path::add_values(int x, int y) {
  x_vals[0] = x;
  y_vals[0] = y;
}

Path::Path () {
  x_vals[0] = {};
  y_vals[0] = {};
}
