#include "Kinematics.h"
#include <Arduino.h>


int d_AC = 208;               
int d_GE = 210;                
int d_CG = 30 + 17;
int d_AB = 50;
int d_BD = 208;
int d_FG = 50;
float d_CE = sqrt(pow(d_GE,2) + pow(d_CG,2));

float d_AE, d_AD, d_BC, d_CD;
float r_AC[2], r_AE[2], r_CE[2], r_GE[2], r_FG[2], r_CD[2];
float xE, yE, gamma, gamma_2, gamma_3, phi_E, phi_1s, phi_2s, theta_1, theta_2, theta_2s, epsilon;
double input;


const double r2d = 180/PI;



Kinematics::TF Kinematics::solveFK(float q1, float q2) {

  float r_AB[] = {-d_AB*cos(q2 / r2d), d_AB*sin(q2 / r2d)};
  float r_AC[] = {d_AC*sin(q1 / r2d),  d_AC*cos(q1 / r2d)};
  float r_BC[] = {r_AC[0] - r_AB[0], r_AC[1] - r_AB[1]};
  d_BC = sqrt(pow(r_BC[0],2) + pow(r_BC[1],2));
  phi_2s = atan2(r_BC[1], r_BC[0]) * r2d;
  d_CD = d_AB;
  input = (pow(d_CD,2) + pow(d_BC,2) - pow(d_BD,2)) / (2 * d_BC * d_CD);
  if (abs(input) > 1) {
    Serial.println("FK not solveable!");
        return;
  }     
  gamma_3 = acos(input) * r2d;
  theta_2s = gamma_3 - phi_2s;
  float r_CG[] = {d_CG*cos((90 - theta_2s) / r2d), d_CG*sin((90 - theta_2s) / r2d)};
  float r_GE[] = {d_GE*cos(theta_2s / r2d), -d_GE*sin(theta_2s / r2d)};
  float r_GF[] = {-d_FG * cos(theta_2s / r2d), d_FG * sin(theta_2s / r2d)};
  xE = r_AC[0] + r_CG[0]+ r_GE[0];
  yE = r_AC[1] + r_CG[1]+ r_GE[1];

  TF fk_pos = {xE, yE};
  return fk_pos;

}



// Inverse Kinematik für Stiftposition berechnen

Kinematics::TF Kinematics::solveIK(float xE, float yE) {

  d_AE = sqrt(pow(xE,2) + pow(yE,2));
  input = (pow(d_AC,2) + pow(d_AE,2) - pow(d_CE,2)) / (2 * d_AC * d_AE);
  
if (abs(input) > 1) {
  Serial.println("Error: IK not solveable!");
  return;
}

  gamma = acos( input ) * r2d;
  phi_E = atan2(yE, xE) * r2d;
  theta_1 = 90 - gamma - phi_E;

  float r_AC[] = {d_AC*sin(theta_1/r2d), d_AC*cos(theta_1/r2d)};
  float r_AE[] = {xE, yE};
  float r_CE[] = {r_AE[0] - r_AC[0], r_AE[1] - r_AC[1]};
  epsilon = atan2(d_CG, d_GE)*r2d;

  float r_GE[] = {cos(epsilon/r2d)*r_CE[0] + sin(epsilon/r2d)*r_CE[1],
            -sin(epsilon/r2d)*r_CE[0] + cos(epsilon/r2d)*r_CE[1]};

  theta_2s = (atan2(0, 1) - atan2(r_GE[1],r_GE[0]))*r2d;

  float r_FG[] = {-d_FG*cos(theta_2s/r2d), d_FG*sin(theta_2s/r2d)};  
  d_AD = sqrt(pow(d_AC,2) + pow(d_FG,2) + 2 * d_AC * d_FG*sin((theta_2s - theta_1)/r2d));
  
  float r_AD[] = {r_AC[0] + r_FG[0], r_AC[1] + r_FG[1]};

  phi_1s = atan2(r_AD[1], r_AD[0])*r2d;;

  input = (pow(d_AB,2) + pow(d_AD,2) - pow(d_BD,2)) / (2 * d_AB * d_AD);
  if (abs(input) > 1) {
    Serial.println("Error: IK not solveable!");
    return;
  }

  gamma_2 = acos( input ) * r2d;
  theta_2 = 180 - gamma_2 - phi_1s;


TF ik_angles = {theta_1, theta_2};

return ik_angles;


}
