//Procedure Functions Setup
void LD_Func() {  //Lie down (fast)
  pwm.setPWM((7), 0, angleToPulse(135));
  pwm.setPWM((4), 0, angleToPulse(45));
  pwm.setPWM((1), 0, angleToPulse(135));
  pwm.setPWM((10), 0, angleToPulse(45));
  //-----------------------------------
  pwm.setPWM((0), 0, angleToPulse(90));
  pwm.setPWM((3), 0, angleToPulse(90));
  pwm.setPWM((6), 0, angleToPulse(90));
  pwm.setPWM((9), 0, angleToPulse(90));
  //-----------------------------------
  pwm.setPWM((8), 0, angleToPulse(30));
  pwm.setPWM((5), 0, angleToPulse(150));
  pwm.setPWM((2), 0, angleToPulse(30));
  pwm.setPWM((11), 0, angleToPulse(150));
}
void SD_Func() {  //Stand up (slow)
  int StartupDistance = 0;
  int StartupPos135 = 135;
  int StartupPos45 = 45;
  int StartupPos30 = 30;
  int StartupPos150 = 150;

  // Initial positions
  pwm.setPWM(0, 0, angleToPulse(90));
  pwm.setPWM(3, 0, angleToPulse(90));
  pwm.setPWM(6, 0, angleToPulse(90));
  pwm.setPWM(9, 0, angleToPulse(90));

  pwm.setPWM(7, 0, angleToPulse(StartupPos135));
  pwm.setPWM(4, 0, angleToPulse(StartupPos45));
  pwm.setPWM(1, 0, angleToPulse(StartupPos135));
  pwm.setPWM(10, 0, angleToPulse(StartupPos45));

  pwm.setPWM(8, 0, angleToPulse(StartupPos30));
  pwm.setPWM(5, 0, angleToPulse(StartupPos150));
  pwm.setPWM(2, 0, angleToPulse(StartupPos30));
  pwm.setPWM(11, 0, angleToPulse(StartupPos150));

  // Loop to adjust positions
  while (StartupDistance != 45) {
    pwm.setPWM(7, 0, angleToPulse(StartupPos135));
    pwm.setPWM(4, 0, angleToPulse(StartupPos45));
    pwm.setPWM(1, 0, angleToPulse(StartupPos135));
    pwm.setPWM(10, 0, angleToPulse(StartupPos45));

    pwm.setPWM(8, 0, angleToPulse(StartupPos30));
    pwm.setPWM(5, 0, angleToPulse(StartupPos150));
    pwm.setPWM(2, 0, angleToPulse(StartupPos30));
    pwm.setPWM(11, 0, angleToPulse(StartupPos150));

    // Increment and decrement
    StartupPos135--;
    StartupPos45++;
    StartupPos30++;
    StartupPos150--;

    StartupDistance++;

    // Small delay for servo movement
    delay(20);
  }

  // Fine-tuning to 90 degrees
  for (int i = 0; i < 15; i++) {
    pwm.setPWM(5, 0, angleToPulse(StartupPos150 - i));
    pwm.setPWM(8, 0, angleToPulse(StartupPos30 + i));
    pwm.setPWM(11, 0, angleToPulse(StartupPos150 - i));
    pwm.setPWM(2, 0, angleToPulse(StartupPos30 + i));
    delay(20);  // Adjust the delay as needed
  }

  // Ensure all positions are exactly 90 degrees
  pwm.setPWM(5, 0, angleToPulse(90));
  pwm.setPWM(8, 0, angleToPulse(90));
  pwm.setPWM(11, 0, angleToPulse(90));
  pwm.setPWM(2, 0, angleToPulse(90));
}
void SD_Func_Invert() {  //Lie down (slow)
  int StartupDistance = 0;
  int StartupPos135 = 90;
  int StartupPos45 = 90;
  int StartupPos30 = 90;
  int StartupPos150 = 90;

  // Initial positions
  pwm.setPWM(0, 0, angleToPulse(90));
  pwm.setPWM(3, 0, angleToPulse(90));
  pwm.setPWM(6, 0, angleToPulse(90));
  pwm.setPWM(9, 0, angleToPulse(90));

  pwm.setPWM(7, 0, angleToPulse(StartupPos135));
  pwm.setPWM(4, 0, angleToPulse(StartupPos45));
  pwm.setPWM(1, 0, angleToPulse(StartupPos135));
  pwm.setPWM(10, 0, angleToPulse(StartupPos45));

  pwm.setPWM(8, 0, angleToPulse(StartupPos30));
  pwm.setPWM(5, 0, angleToPulse(StartupPos150));
  pwm.setPWM(2, 0, angleToPulse(StartupPos30));
  pwm.setPWM(11, 0, angleToPulse(StartupPos150));

  // Loop to adjust positions
  while (StartupDistance != 45) {
    pwm.setPWM(7, 0, angleToPulse(StartupPos135));
    pwm.setPWM(4, 0, angleToPulse(StartupPos45));
    pwm.setPWM(1, 0, angleToPulse(StartupPos135));
    pwm.setPWM(10, 0, angleToPulse(StartupPos45));

    pwm.setPWM(8, 0, angleToPulse(StartupPos30));
    pwm.setPWM(5, 0, angleToPulse(StartupPos150));
    pwm.setPWM(2, 0, angleToPulse(StartupPos30));
    pwm.setPWM(11, 0, angleToPulse(StartupPos150));

    // Increment and decrement
    StartupPos135++;
    StartupPos45--;
    StartupPos30--;
    StartupPos150++;

    StartupDistance++;

    // Small delay for servo movement
    delay(20);
  }

  // Fine-tuning to 90 degrees
  for (int i = 0; i < 15; i++) {
    pwm.setPWM(5, 0, angleToPulse(StartupPos150 + i));
    pwm.setPWM(8, 0, angleToPulse(StartupPos30 - i));
    pwm.setPWM(11, 0, angleToPulse(StartupPos150 + i));
    pwm.setPWM(2, 0, angleToPulse(StartupPos30 - i));
    delay(20);  // Adjust the delay as needed
  }

  // Ensure all positions are exact
  delay(200);
  pwm.setPWM(5, 0, angleToPulse(150));
  pwm.setPWM(8, 0, angleToPulse(30));
  pwm.setPWM(11, 0, angleToPulse(150));
  pwm.setPWM(2, 0, angleToPulse(30));
}
void SDS_Func() {  //Sit down for shake function
  SD_Func_Invert();
  pwm.setPWM((0), 0, angleToPulse(90));
  pwm.setPWM((3), 0, angleToPulse(90));
  pwm.setPWM((6), 0, angleToPulse(90));
  pwm.setPWM((9), 0, angleToPulse(90));
  //-----------------------------------
  pwm.setPWM((7), 0, angleToPulse(110));
  pwm.setPWM((4), 0, angleToPulse(110));
  pwm.setPWM((1), 0, angleToPulse(135));
  pwm.setPWM((10), 0, angleToPulse(45));
  //-----------------------------------
  pwm.setPWM((8), 0, angleToPulse(90));
  pwm.setPWM((5), 0, angleToPulse(90));
  pwm.setPWM((2), 0, angleToPulse(50));
  pwm.setPWM((11), 0, angleToPulse(120));
}
