// Define the lengths of the leg segments
const float L1 = 11.1;  // Upper leg length
const float L2 = 11.8;  // Lower leg length

// Default position coordinates for paw at 90-degree angles
const float x_default = L2;  // When knee is straight out horizontally
const float y_default = L1;  // When upper leg is vertical

// Rotation angle to align y-axis with the top leg
float rotationAngle = radians(-45);  // Adjust this as needed (its in degrees then converted to radians)

void inverseKinematics(int x, int y, float &theta1, float &theta2) {
  // Adjust x and y by the default position to set origin at the paw's base
  x -= x_default;
  y -= y_default;

  // Rotate the coordinates by rotationAngle
  float x_rotated = x * cos(rotationAngle) - y * sin(rotationAngle);
  float y_rotated = x * sin(rotationAngle) + y * cos(rotationAngle);

  // Perform inverse kinematics calculations on rotated coordinates
  float cosTheta2 = (x_rotated * x_rotated + y_rotated * y_rotated - L1 * L1 - L2 * L2) / (2 * L1 * L2);
  float sinTheta2 = sqrt(1 - cosTheta2 * cosTheta2);
  theta2 = atan2(sinTheta2, cosTheta2);

  float k1 = L1 + L2 * cosTheta2;
  float k2 = L2 * sinTheta2;

  theta1 = atan2(y_rotated, x_rotated) - atan2(k2, k1);
}


void stepfrbl(int x, int y){
  // Calculate inverse kinematics
  float theta1, theta2;
  inverseKinematics(x, y, theta1, theta2);

  // Convert angles from radians to degrees
  int RhipAngle = radiansToDegrees(theta1);
  int RkneeAngle = radiansToDegrees(theta2);

  // Convert from right to left
  int LhipAngle = (180-RhipAngle);
  int LkneeAngle = (180-RkneeAngle);

  // Clamp the angles to be between 0 and 180 degrees
  RhipAngle = constrain(RhipAngle, 0, 180);
  RkneeAngle = constrain(RkneeAngle, 0, 180);
  LhipAngle = constrain(LhipAngle, 0, 180);
  LkneeAngle = constrain(LkneeAngle, 0, 180);

  // Move servos to calculated angles
  pwm.setPWM(7, 0, angleToPulse(LhipAngle));//left side converted
  pwm.setPWM(8, 0, angleToPulse(LkneeAngle));//left side converted
  pwm.setPWM(4, 0, angleToPulse(RhipAngle));// right side default
  pwm.setPWM(5, 0, angleToPulse(RkneeAngle));// right side default
  Serial.println(LhipAngle);
  Serial.println(LkneeAngle);
  Serial.println(RhipAngle);
  Serial.println(RkneeAngle);
}

void stepflbr(int x, int y){
  // Calculate inverse kinematics
  float theta1, theta2;
  inverseKinematics(x, y, theta1, theta2);

  // Convert angles from radians to degrees
  int RhipAngle = radiansToDegrees(theta1);
  int RkneeAngle = radiansToDegrees(theta2);

  // Convert from right to left
  int LhipAngle = (180-RhipAngle);
  int LkneeAngle = (180-RkneeAngle);

  // Clamp the angles to be between 0 and 180 degrees
  RhipAngle = constrain(RhipAngle, 0, 180);
  RkneeAngle = constrain(RkneeAngle, 0, 180);
  LhipAngle = constrain(LhipAngle, 0, 180);
  LkneeAngle = constrain(LkneeAngle, 0, 180);

  // Move servos to calculated angles
  pwm.setPWM(10, 0, angleToPulse(RhipAngle));//left side converted
  pwm.setPWM(11, 0, angleToPulse(RkneeAngle));//left side converted
  pwm.setPWM(1, 0, angleToPulse(LhipAngle));// right side default
  pwm.setPWM(2, 0, angleToPulse(LkneeAngle));// right side default
  Serial.println(LhipAngle);
  Serial.println(LkneeAngle);
  Serial.println(RhipAngle);
  Serial.println(RkneeAngle);
}

void WalkFunc() {
  while (isWalking == 1) {
    currentMillis = millis();
    int joystickValueFB = ibus.readChannel(1);
    // int joystickValueLR = ibus.readChannel(4);
    
    // Calculate the step timer based on the joystick position
    float stepTimerFB;
    int joystickCenter = 1500;
    int deadzone = 20; // Define a deadzone to avoid minor joystick movements

    // Deadzone settings
    int deadzoneFBLow = 1480;
    int deadzoneFBHigh = 1520;

    // Only consider values outside the deadzone
    if (joystickValueFB < (joystickCenter - deadzone) || joystickValueFB > (joystickCenter + deadzone)) {
      // Calculate the step timer based on distance from the center
      if (joystickValueFB < joystickCenter) {
        // Moving down
        stepTimerFB = map(joystickValueFB, joystickCenter - deadzone, 1000, 300, 0); // Faster with more downward movement
      } 
      else {
        // Moving up
        stepTimerFB = map(joystickValueFB, joystickCenter + deadzone, 2000, 300, 0); // Faster with more upward movement
      }
    } 
    else {
      // Within deadzone, set to a base speed
      stepTimerFB = 300; // Base speed when joystick is in the neutral range
    }

    // Apply limits to stepTimer
    stepTimerFB = constrain(stepTimerFB, 100, 300);

    Serial.println("Step Timer FB: " + String(stepTimerFB)); // Debugging output

    // Forward-Backward (FB) Movement with Deadzone
    if ((joystickValueFB > deadzoneFBHigh || joystickValueFB < deadzoneFBLow)) { 
      bool isForward = joystickValueFB > deadzoneFBHigh;

      if (currentMillis - previousStepMillis >= stepTimerFB) {
        previousStepMillis = currentMillis;

        // Use different gait sequences for forward and reverse
        if (isForward) {
          // Forward gait cycle
          //(-5,10) is about zeroed out to servo paw default pos
          switch (stepFlagFB) {
            case 0: stepFlagFB = 1; stepfrbl(-5, 15); break;
            case 1: stepFlagFB = 2; stepfrbl(-2.5, 15); break;
            case 2: stepFlagFB = 3; stepfrbl(-2.5, 5); break;
            case 3: stepFlagFB = 4; stepfrbl(-5, 5); break;

            case 4: stepFlagFB = 5; stepflbr(-5, 15); break;
            case 5: stepFlagFB = 6; stepflbr(-2.5, 15); break;
            case 6: stepFlagFB = 7; stepflbr(-2.5, 5); break;
            case 7: stepFlagFB = 0; stepflbr(-5, 5); break;
          }
        } else {
          // Reverse gait cycle
          switch (stepFlagFB) {
            case 0: stepFlagFB = 1; stepfrbl(-5, 5); break;
            case 1: stepFlagFB = 2; stepfrbl(-2.5, 5); break;
            case 2: stepFlagFB = 3; stepfrbl(-2.5, 15); break;
            case 3: stepFlagFB = 4; stepfrbl(-5, 15); break;

            case 4: stepFlagFB = 5; stepflbr(-5, 5); break;
            case 5: stepFlagFB = 6; stepflbr(-2.5, 5); break;
            case 6: stepFlagFB = 7; stepflbr(-2.5, 15); break;
            case 7: stepFlagFB = 0; stepflbr(-5, 15); break;
          }
        }
      }
      stepFlagLR = 0; // Reset LR flag when FB is in use
    } 
    /*
    // Left-Right (LR) Movement (commented out as per your previous code)
    else if ((joystickValueLR < deadzoneLRLow || joystickValueLR > deadzoneLRHigh)) {
      if (currentMillis - previousStepMillis >= stepTimerLR) {
        previousStepMillis = currentMillis;

        if (joystickValueLR < deadzoneLRLow) {
          // Left turn gait cycle
          switch (stepFlagLR) {
            case 0: stepFlagLR = 1; stepfrbl(-5, 15); break;
            case 1: stepFlagLR = 2; stepfrbl(-7.5, 10); break;
            case 2: stepFlagLR = 3; stepfrbl(-12.5, 12.5); break;
            case 3: stepFlagLR = 4; stepflbr(-5, 15); break;
            case 4: stepFlagLR = 5; stepflbr(-7.5, 10); break;
            case 5: stepFlagLR = 0; stepflbr(-12.5, 12.5); break;
          }
        } else if (joystickValueLR > deadzoneLRHigh) {
          // Right turn gait cycle
          switch (stepFlagLR) {
            case 0: stepFlagLR = 1; stepflbr(-5, 15); break;
            case 1: stepFlagLR = 2; stepflbr(-7.5, 10); break;
            case 2: stepFlagLR = 3; stepflbr(-12.5, 12.5); break;
            case 3: stepFlagLR = 4; stepfrbl(-5, 15); break;
            case 4: stepFlagLR = 5; stepfrbl(-7.5, 10); break;
            case 5: stepFlagLR = 0; stepfrbl(-12.5, 12.5); break;
          }
        }
      }
      stepFlagFB = 0; // Reset FB flag when LR is in use
    }
    */
    else {
      // Reset to standing position if joystick is in deadzone
      for (int i = 0; i < 12; i++) {
        pwm.setPWM(i, 0, angleToPulse(90));
      }
      isWalking = 0;
      stepFlagFB = 0;
      stepFlagLR = 0;
    }
  }
}


