#include "Kinematics.h"
#include <Arduino.h>


int r01 = 208;                  // 20.7
int r1E_a = 210;                  // 23
int r1E_b = 30;                 // 2.8
int r00 = 50;
int r01ss = 210;
int r11 = 50;
float r1E = sqrt(pow(r1E_a,2) + pow(r1E_b,2));


Kinematics::TF Kinematics::solveFK(float q1, float q2) {

  double r2d = 180/PI;
  float xE = r01*sin(q1/r2d) + r1E_a*cos(q2/r2d) + r1E_b*cos((90-q2)/r2d);
  float yE = r01*cos(q1/r2d) - r1E_a*sin(q2/r2d) + r1E_b*sin((90-q2)/r2d);
  
  TF fk_pos = {xE*10, yE*10};
  return fk_pos;

}

Kinematics::TF Kinematics::solveIK(float xE, float yE) {

// Inverse Kinematik für Drawbot berechnen

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


Kinematics::TF Kinematics::solveIK_advanced(float xE, float yE) {

// Inverse Kinematik für Drawbot berechnen


// Pose des Endeffektors bzgl. der Basis definieren (Einheit: cm)
    // Grundstellung: xE = r1E_a;
    // Grundstellung: yE = r01 + r1E_b;
float r0E = sqrt(pow(xE,2) + pow(yE,2));    // Abstand EE von Basis berechnen


// Gelenkwinkel 1 bestimmen (Einheit: °)

    // Input für acos prüfen: komplexe Winkel abfangen!
double input = (pow(r01,2) + pow(r0E,2) - pow(r1E,2)) / (2*r01*r0E);
Serial.print(input);

if (abs(input) > 1) {
  Serial.println("Error: IK not solveable!");
  return;
}

double r2d = 180/PI;

float gamma = acos( input ) * r2d;
float phi_E = atan2(yE, xE) * r2d;
float theta_1 = 90 - gamma - phi_E;      // Winkel 1 berechnen

//  Gelenkwinkel 2* bestimmen (Einheit: °)
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

float theta_2s = (atan2(y2,y1) - atan2(x2,x1))*r2d;


  
// Gelenkwinkel 2 bestimmen            

float r11_vec[] = {-r11*cos(theta_2s/r2d), r11*sin(theta_2s/r2d)};
float r01s_vec[] = {r01_vec[0] + r11_vec[0], r01_vec[1] + r11_vec[1]};
float r01s = sqrt(pow(r01,2) + pow(r11,2) + 2*r01*r11*sin((theta_2s - theta_1)/r2d));
float phi_1s = atan2(r01s_vec[1], r01s_vec[0])*r2d;

input = (pow(r00,2) + pow(r01s,2) - pow(r01ss,2)) / (2*r00*r01s);
if (abs(input) > 1) {
  Serial.println("Error: IK not solveable!");
  return;
}

float gamma_2 = acos( input ) * r2d;
float theta_2 = 180 - gamma_2 - phi_1s;


TF ik_angles = {theta_1, theta_2};

return ik_angles;


}
