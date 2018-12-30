
class Drawbot {
    int M1_Pos, M2_Pos, M1_Target, M2_Target;
    int XPos, YPos;
    bool ServoPos;
    int opmode;
    int delayUs;
    int path;
    
  public:
    void set_joint_values (int,int);
    void set_target_values (int,int);
    void move_to_target();
    void move_steps(int,int);
    void set_delayUs(int);
    void move_step(int);
    void define_path();
    int get_values();
};
