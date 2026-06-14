# Inverse Kinematics #
-Updated, but not quite finished.
## The legs ##

The length-measurements of the legs is performed on the centers of the rotational axis and from the perspective of the foot tip touching the ground in its center.

The foot-tip actually just elevates the body 20mm from the ground on every leg and is because of this is disregarded for now.

![L1_L2](https://github.com/michaelkubina/SpotMicroESP32/blob/master/kinematics/L1_L2.png)

![L3](https://github.com/michaelkubina/SpotMicroESP32/blob/master/kinematics/L3.png)

![L4](https://github.com/michaelkubina/SpotMicroESP32/blob/master/kinematics/L4.png)


## The body ##
The body (the plane where the four legs are attached to) is 78mm \* 207,5mm. The axis of the front and rear leg-pairs are 78mm apart, while the center of the first rotational axis of the legs is 207,5mm apart. The 207,5mm are the sum of the chassis side (150mm), both inner shoulders (each 5mm thick) and two times the halfes of the bottom/top shoulder blocks (2 \* 23,75mm = 47,5mm).

![Body](https://github.com/michaelkubina/SpotMicroESP32/blob/master/kinematics/body.png)

In this current code iteration, we’ve set up inverse kinematics (IK) to calculate the angles required for the robot dog’s legs to move to specified positions. Let's break down the implementation and cover some key points, especially the current limitations and assumptions about the setup.

### Explanation of the Code

#### 1. **Leg Segment Lengths**
   - `L1` and `L2` represent the lengths of the upper and lower leg segments:
     - `L1` = 11.1 (upper leg length)
     - `L2` = 11.8 (lower leg length)
   
#### 2. **Default Positioning and Rotation**
   - `x_default` and `y_default` specify the default coordinates for the paw when the legs are at 90 degrees:
     - `x_default = L2`: represents the x-coordinate when the knee is fully extended horizontally.
     - `y_default = L1`: represents the y-coordinate when the upper leg is vertical.

   - A `rotationAngle` of -45 degrees is applied to align the y-axis with the top leg in its initial position. This rotation aligns the coordinate system to match the robot dog’s anatomy.
     
   - **In the future I will re-align the kinematic, so that the y-axis is vertical not horizontal.**

#### 3. **Coordinate Transformation**
   - Since the y-axis of this coordinate system is rotated by 90 degrees, the positive y-axis points **forward** in the direction of the dog’s movement.
   - To make calculations simpler, we adjust the `x` and `y` coordinates to set the paw’s default position as the origin (0,0).
     
   - **(-5,10) is about zeroed out to servo paw default position when standing.**
   
   - Then, we rotate the coordinates by `rotationAngle` to match the orientation of the leg.

#### 4. **Inverse Kinematics Calculations**
   - The IK calculations use the adjusted (rotated) coordinates to compute angles for the joints.
   - For a given `(x, y)` target position:
     - `theta2` (the knee angle) is calculated using the law of cosines.
     - `theta1` (the hip angle) is calculated using trigonometry based on the rotated coordinates and the knee angle.
   - These angles (`theta1` and `theta2`) determine the leg's configuration to achieve the desired paw position.

#### 5. **Conversion for Servos**
   - The angles calculated in radians (`theta1` and `theta2`) are converted to degrees to match the servo's requirements.
   - Since the left and right sides are mirrored:
     - Right hip/knee angles are directly applied.
     - Left hip/knee angles are calculated by mirroring the right angles (`180 - RhipAngle` and `180 - RkneeAngle`).

### Code Limitations and Future Adjustments

#### Shoulder Axis Status
   - The walking IK still solves the **hip** and **knee** joints in the leg side plane.
   - The code now names the four shoulder/abduction servo channels and uses them in a conservative tilt mode, but this is not a fully calibrated 3D leg solver yet.
   - A full shoulder IK model still needs hardware-verified shoulder axis directions, servo zero offsets, and safe lateral foot limits for each leg.

#### Rotation of the Coordinate System
   - The entire coordinate system for the IK calculations is rotated 90 degrees to the right. This means:
     - The positive y-axis points forward in the direction the dog faces.
     - Adjustments have been made accordingly in the IK function to handle this axis alignment.

### Gait Sequences in `WalkFunc`
![Gait Pattern](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/gait_timer_illustrate.jpg)

   - Above, is a photo which illustrates the current (soon to be outdated) gait cycle, with 'x' being the ground plane. 
   - *Gait cycles can easily be planned out in [Desmos Calculator](https://www.desmos.com/calculator), by typing in coordinates directly eg:* **(0,0)**
   - The `WalkFunc` function controls the robot’s gait (movement pattern) based on joystick input and ramps velocity of the gait based on joystick position:

1. **Joystick Control for Forward and Reverse Walking (NUMBERS ACCURATE TO CURRENT GAIT CYCLE)**
   - A deadzone is implemented to prevent small joystick movements from triggering unintended steps.
   - Outside the deadzone, the joystick position determines the timing (`stepTimerFB`) for each step in the gait cycle.
   - Forward and reverse gait cycles are defined:
     - **Forward cycle:** Paw moves in an arc from `(-5,15)` to `(-5,5)`, then repeats. (these may change with gait interations in the future - 12/11/24)
     - **Reverse cycle:** Paw moves in the reverse order, allowing backward movement.

2. **Forward-Backward (FB) and Left-Right (LR) Modes**
   - When moving forward or backward, only the forward-backward gait sequence is executed, and any left-right control is ignored.
   - A similar approach would apply for turning left or right (though it's currently commented out).

### How the Inverse Kinematics and Servo Angles Work Together

Each step in the gait cycle triggers the `stepfrbl` or `stepflbr` functions, which call the inverse kinematics function with specific `(x, y)` coordinates. This function:
   - Calculates the angles needed to reach those coordinates.
   - Applies those angles to the servos controlling the leg joints, moving each leg accordingly.

### Summary

This code provides a foundation for controlling the robot dog's legs using inverse kinematics. However:
   - The walking IK model is still a 2D hip/knee solver.
   - Tilt mode uses the shoulder channels for a small roll offset and shifts front/rear hip/knee IK targets for pitch, but should be tested with the robot lifted before ground use.
   - The coordinate system is rotated to match the dog's orientation, with the positive y-axis facing forward (I intend to fix this an make the y-axis vertical).
   
### Next Steps

To further improve the robot’s movement:
1. Hardware-calibrate the shoulder servo directions and neutral offsets.
2. Extend the current 2D solver into a real 3D shoulder/hip/knee IK model with lateral foot targets.
3. Adjust and refine gait patterns for smoother and more natural movements.
4. Tune the joystick deadzones and movement thresholds for responsive control.
