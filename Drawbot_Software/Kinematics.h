
class Kinematics {

      float r01, r1E_a, r1E_b;
      public:
      struct TF {
        float c1,c2;
      };   
    
  public:
    TF solveFK(float,float);
    TF solveIK(float,float);
    TF solveIK_advanced(float,float);
    void set_parameters(float,float,float);
    

};
