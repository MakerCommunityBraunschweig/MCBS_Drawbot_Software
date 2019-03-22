#include "Kinematics.h"

#define FWD       1
#define BCKWD     -1
#define DEG       37

class Drawbot {
    int M1_Pos, M2_Pos, M1_Dir, M2_Dir;
    int delayUs;
    float theta_1, theta_2;
    Kinematics kin;
    
  public:
    void set_directions (int,int);
    void invert_directions();
    void set_joint_values (int,int);
    int get_joint_value(int);
    void set_delayUs(int);
    int get_delayUs();

    void move_step(int);
    void move_steps(int,int);
    void move_path(int[],int[], int);
    void move_path_XY(int[],int[], int);
    void move_linear_in_js(int,int);
    void move_by_angles(float, float);
    void move_to_angles(float, float);
    void init_angles();
    void move_to_point_XY(float, float);
    void home_all();
    
    void manual_mode();
    bool check_boundaries(int,int);

    double solveFK(float, float);
    double solveIK(float, float);

};
