#include "Kinematics.h"
#include <Arduino.h>

void Kinematics::set_parameters(float a, float b, float c) {
  r01 = a;
  r1E_a = b;
  r1E_b = c;
}

Kinematics::TF Kinematics::solveFK(float q1, float q2) {

  double r2d = 180/PI;
  float xE = r01*sin(q1/r2d) + r1E_a*cos(q2/r2d) + r1E_b*cos((90-q2)/r2d);
  float yE = r01*cos(q1/r2d) - r1E_a*sin(q2/r2d) + r1E_b*sin((90-q2)/r2d);
  
  TF fk_pos = {xE*10, yE*10};
  return fk_pos;

}

Kinematics::TF Kinematics::solveIK(float xE, float yE) {

  xE = xE / 10;
  yE = yE / 10;

// Inverse Kinematik für Drawbot berechnen

// Geometrische Parameter definieren (Einheit: cm)
  
//float r01 = 20;                  // 20.7
//float r1E_a = 20;                  // 23
//float r1E_b = 2;                 // 2.8
float r1E = sqrt(pow(r1E_a,2) + pow(r1E_b,2));


// Pose des Endeffektors bzgl. der Basis definieren (Einheit: cm)
    // Grundstellung: xE = r1E_a;
    // Grundstellung: yE = r01 + r1E_b;
float r0E = sqrt(pow(xE,2) + pow(yE,2));    // Abstand EE von Basis berechnen


// Gelenkwinkel 1 bestimmen (Einheit: °)

double input = (pow(r01,2) + pow(r0E,2) - pow(r1E,2)) / (2*r01*r0E);

if (abs(input) > 1) {
  Serial.println("Error: IK not solveable!");
  return;
}

double r2d = 180/PI;

float gamma = acos( input ) * r2d;
float phi_E = atan2(yE, xE) * r2d;
float theta_1 = 90 - gamma - phi_E;      // Winkel 1 berechnen

//  Gelenkwinkel 2 bestimmen (Einheit: °)
float r01_vec[] = {r01*sin(theta_1/r2d), r01*cos(theta_1/r2d)};
float r0E_vec[] = {xE, yE};
float r1E_vec[] = {r0E_vec[0] - r01_vec[0], r0E_vec[1] - r01_vec[1]};
float epsilon = atan2(r1E_b, r1E_a)*r2d;

float r1E_a_vec[] = {cos(epsilon/r2d)*r1E_vec[0] + sin(epsilon/r2d)*r1E_vec[1],
                    -sin(epsilon/r2d)*r1E_vec[0] + cos(epsilon/r2d)*r1E_vec[1]};

float x1 = r1E_a_vec[0];
float x2 = r1E_a_vec[1];
float y1 = 1;
float y2 = 0;

float theta_2 = (atan2(y2,y1) - atan2(x2,x1))*r2d;

TF ik_angles = {theta_1, theta_2};

return ik_angles;


}


