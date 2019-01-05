
#define FWD       1
#define BCKWD     -1

class Drawbot {
    int M1_Pos, M2_Pos, M1_Target, M2_Target, M1_Dir, M2_Dir;
    int XPos, YPos;
    bool ServoPos;
    int opmode;
    int delayUs;
    int m1_path[], m2_path[];
    public: bool isHomed;
    
  public:
    void set_directions (int,int);
    void invert_directions();
    void set_joint_values (int,int);
    int get_joint_values();
    void set_target_values (int,int);
    void move_to_target();
    void move_steps(int,int);
    void set_delayUs(int);
    int get_delayUs();
    void move_step(int);
    void set_path(int[], int[]);
    void move_path(int[],int[]);
    void manual_mode();
    
    void home_all();
};
