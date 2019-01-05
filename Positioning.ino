

/* Test Program to demonstate positioning */
void MoveIt() {
  
  Drawbot db;
  db.set_joint_values(0,0);
  int a = db.get_joint_values();
  Serial.println(a);
}
