
class Drawbot {
    int M1_Pos, M2_Pos, M1_Target, M2_Target;
    int XPos, YPos;
    bool ServoPos;
    int opmode;
    int delayUs;
    int m1_path[], m2_path[];
    
  public:
    void set_joint_values (int,int);
    void set_target_values (int,int);
    void move_to_target();
    void move_steps(int,int);
    void set_delayUs(int);
    void move_step(int);
    void move_path(int[],int[]);
    int get_values();
};
