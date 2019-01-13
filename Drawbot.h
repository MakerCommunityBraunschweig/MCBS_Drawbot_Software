
#define FWD       1
#define BCKWD     -1

class Drawbot {
    int M1_Pos, M2_Pos, M1_Dir, M2_Dir;
    int delayUs;
    
  public:
    void set_directions (int,int);
    void invert_directions();
    void set_joint_values (int,int);
    int get_joint_value(int);
    void set_delayUs(int);
    int get_delayUs();

    void move_step(int);
    void move_steps(int,int);
    void move_path(int[],int[]);
    void move_linear_in_js(int,int);
    void home_all();
    
    void manual_mode();
    bool check_boundaries(int,int);

};
