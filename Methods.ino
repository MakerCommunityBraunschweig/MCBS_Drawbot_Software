
void test_run (int overAll, int period, int delayUs) {

  while(millis() < overAll) {

    set_dirs_contra(period);
    
    digitalWrite(X_STEP_PIN, HIGH);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(delayUs);
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(delayUs);

     if (delayUs > 500) {
        delayUs -= 1;  
     }
    
  } 
  
}

void test_run_2 (int overAll, int period, int delayUs, int triAmp) {

  int opmode = 0;
  int add = 0;

  while(millis() < overAll) {

    set_dirs_contra(period);
    
    digitalWrite(X_STEP_PIN, HIGH);
    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(delayUs+add);
    digitalWrite(X_STEP_PIN, LOW);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(delayUs+add);

    /* change delay time according to this pattern:

      /\    /\
     /  \  /  \
    /    \/    \
    
    */

    switch(opmode) {
      case 0: {
        if (add < triAmp) {
          add += 1;
        } else {
          opmode = 1;
        }
      }; break;
      case 1: {
        if (add > -triAmp) {
          add -= 1;
        } else {
          opmode = 0;
        }
      }; break;
      
    }

    //Serial.println(delayUs + add);
    
  } 
  
}


void measure_steps (int numOfSteps) {

  digitalWrite(X_DIR_PIN, HIGH);
  digitalWrite(Y_DIR_PIN, HIGH);

  int step_count;
  for(int i = 0; i <= numOfSteps; i++) {
    MoveStepX(500);
    //MoveStepY(500);
    step_count = i;
    Serial.println(step_count);
  } 
}





// _______________ HELP METHODS ________________ //


void SetDirX(int dir) {
  switch(dir) {
  case 0: digitalWrite(X_DIR_PIN, HIGH);  
  case 1: digitalWrite(X_DIR_PIN, LOW); 
  }
}

void SetDirY(int dir) {
  switch(dir) {
  case 0: digitalWrite(Y_DIR_PIN, HIGH);  
  case 1: digitalWrite(Y_DIR_PIN, LOW); 
  }
}

void MoveStepX(int delayUs) {
  digitalWrite(X_STEP_PIN, HIGH);
  delayMicroseconds(delayUs);
  digitalWrite(X_STEP_PIN, LOW);
  delayMicroseconds(delayUs);
}

void MoveStepY(int delayUs) {
  digitalWrite(Y_STEP_PIN, HIGH);
  delayMicroseconds(delayUs);
  digitalWrite(Y_STEP_PIN, LOW);
  delayMicroseconds(delayUs);
}



/* Setting the directions for CONTRA-ROTATION mode of the stepper motors */
void set_dirs_contra (int period) {
  
  if (millis() %period < period/2) {
    digitalWrite(X_DIR_PIN, HIGH);
    digitalWrite(Y_DIR_PIN, LOW);
    }
    else {
      digitalWrite(X_DIR_PIN, LOW);
      digitalWrite(Y_DIR_PIN, HIGH);
    }
}

void blinkit () {

   if (millis() %1000 <500) 
      digitalWrite(LED_PIN, HIGH);
   else
      digitalWrite(LED_PIN, LOW);
      
}
