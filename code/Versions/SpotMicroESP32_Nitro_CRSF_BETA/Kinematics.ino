// Define the lengths of the leg segments
int readRcChannel(uint8_t channelIndex);

const float L1 = 11.1;  // Upper leg length
const float L2 = 11.8;  // Lower leg length

// Default position coordinates for paw at 90-degree angles
const float x_default = L2;  // When knee is straight out horizontally
const float y_default = L1;  // When upper leg is vertical

const float kStandX = -5.0f;
const float kStandY = 10.0f;
const float kTiltPitchYOffset = 3.0f;
const int kShoulderCenterDeg = 90;
const int kTiltShoulderMaxDeg = 10;
const int kTiltDeadzoneUs = 35;
const int kTiltInputSpanUs = 500;
const uint8_t kTiltRollChannel = 0;
const uint8_t kTiltPitchChannel = 1;

struct LegServoMap {
  uint8_t shoulder;
  uint8_t hip;
  uint8_t knee;
  bool mirrored;
};

const LegServoMap kFrontLeft = {0, 1, 2, true};
const LegServoMap kFrontRight = {3, 4, 5, false};
const LegServoMap kBackLeft = {6, 7, 8, true};
const LegServoMap kBackRight = {9, 10, 11, false};

// Rotation angle to align y-axis with the top leg
float rotationAngle = radians(-45);  // Adjust this as needed (its in degrees then converted to radians)
const bool kDebugKinematics = false;

void inverseKinematics(float x, float y, float &theta1, float &theta2) {
  // Adjust x and y by the default position to set origin at the paw's base
  x -= x_default;
  y -= y_default;

  // Rotate the coordinates by rotationAngle
  float x_rotated = x * cos(rotationAngle) - y * sin(rotationAngle);
  float y_rotated = x * sin(rotationAngle) + y * cos(rotationAngle);

  // Perform inverse kinematics calculations on rotated coordinates
  float cosTheta2 = (x_rotated * x_rotated + y_rotated * y_rotated - L1 * L1 - L2 * L2) / (2 * L1 * L2);
  cosTheta2 = constrain(cosTheta2, -1.0f, 1.0f);
  float sinTheta2 = sqrt(max(0.0f, 1.0f - cosTheta2 * cosTheta2));
  theta2 = atan2(sinTheta2, cosTheta2);

  float k1 = L1 + L2 * cosTheta2;
  float k2 = L2 * sinTheta2;

  theta1 = atan2(y_rotated, x_rotated) - atan2(k2, k1);
}

float rcOffset(uint8_t channelIndex, float maxOffset) {
  int delta = readRcChannel(channelIndex) - 1500;
  if (abs(delta) <= kTiltDeadzoneUs) {
    return 0.0f;
  }

  delta = constrain(delta, -kTiltInputSpanUs, kTiltInputSpanUs);
  return (static_cast<float>(delta) / static_cast<float>(kTiltInputSpanUs)) * maxOffset;
}

void setServoAngle(uint8_t channel, int angle) {
  pwm.setPWM(channel, 0, angleToPulse(constrain(angle, 0, 180)));
}

void setLegIk(const struct LegServoMap &leg, float x, float y) {
  float theta1, theta2;
  inverseKinematics(x, y, theta1, theta2);

  int hipAngle = radiansToDegrees(theta1);
  int kneeAngle = radiansToDegrees(theta2);

  if (leg.mirrored) {
    hipAngle = 180 - hipAngle;
    kneeAngle = 180 - kneeAngle;
  }

  setServoAngle(leg.hip, hipAngle);
  setServoAngle(leg.knee, kneeAngle);

  if (kDebugKinematics) {
    Serial.print("leg ");
    Serial.print(leg.hip);
    Serial.print("/");
    Serial.print(leg.knee);
    Serial.print(" hip=");
    Serial.print(hipAngle);
    Serial.print(" knee=");
    Serial.println(kneeAngle);
  }
}

void setShoulderAngles(int frontLeft, int frontRight, int backLeft, int backRight) {
  setServoAngle(kFrontLeft.shoulder, frontLeft);
  setServoAngle(kFrontRight.shoulder, frontRight);
  setServoAngle(kBackLeft.shoulder, backLeft);
  setServoAngle(kBackRight.shoulder, backRight);
}

void setStandingPoseWithShoulders(int frontLeft, int frontRight, int backLeft, int backRight) {
  setShoulderAngles(frontLeft, frontRight, backLeft, backRight);
  setLegIk(kFrontLeft, kStandX, kStandY);
  setLegIk(kFrontRight, kStandX, kStandY);
  setLegIk(kBackLeft, kStandX, kStandY);
  setLegIk(kBackRight, kStandX, kStandY);
}

void TiltFunc() {
  int rollOffset = static_cast<int>(rcOffset(kTiltRollChannel, kTiltShoulderMaxDeg));
  float pitchOffset = rcOffset(kTiltPitchChannel, kTiltPitchYOffset);

  int leftShoulder = kShoulderCenterDeg + rollOffset;
  int rightShoulder = kShoulderCenterDeg - rollOffset;

  setShoulderAngles(leftShoulder, rightShoulder, leftShoulder, rightShoulder);

  setLegIk(kFrontLeft, kStandX, kStandY + pitchOffset);
  setLegIk(kFrontRight, kStandX, kStandY + pitchOffset);
  setLegIk(kBackLeft, kStandX, kStandY - pitchOffset);
  setLegIk(kBackRight, kStandX, kStandY - pitchOffset);
}


void stepfrbl(float x, float y){
  setLegIk(kBackLeft, x, y);
  setLegIk(kFrontRight, x, y);
}

void stepflbr(float x, float y){
  setLegIk(kBackRight, x, y);
  setLegIk(kFrontLeft, x, y);
}

void WalkFunc() {
    currentMillis = millis();
    int joystickValueFB = readRcChannel(1);
    // int joystickValueLR = readRcChannel(4);
    
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

    if (kDebugKinematics) {
      Serial.println("Step Timer FB: " + String(stepTimerFB));
    }

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
      setStandingPoseWithShoulders(kShoulderCenterDeg, kShoulderCenterDeg, kShoulderCenterDeg, kShoulderCenterDeg);
      isWalking = 0;
      stepFlagFB = 0;
      stepFlagLR = 0;
    }
}


