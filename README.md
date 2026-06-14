# SpotMicroESP32 Arduino Code- Nitro Fork

This fork of the SpotMicroESP32 project builds on the excellent foundation by michaelkubina. It introduces several enhancements, including new features for Arduino code integration, a custom PCB design, and support for the FS-i6X hobby controller, making it easier to control and expand the functionality of the SpotMicro robot dog. This version also uses the ESP32-DevKitC. Work-in-Progress!

You can find the 3D-printing parts on Thingiverse as well: https://www.thingiverse.com/thing:4559827/files
(There are a few modification files in this repo for updated metal servo horns, which I fully recommend using)

## Overview
This SpotMicroESP32 project fork is designed to provide a more modular and user-friendly experience, particularly for those interested in robotics and DIY projects. With the added support for the FS-i6X transmitter and a custom PCB, users now have more flexible and powerful ways to interact with and program their robot dog.

Key Enhancements in This Fork

### Arduino Code Integration: 
- I've added Arduino-based code that simplifies control logic, particularly for managing the SpotMicro’s movement and gait sequences. This code is compatible with the FS-i6 hobby controller, allowing for responsive joystick-based navigation.

### Custom PCB Design: 
- The custom PCB allows for better organization and management of SpotMicro’s hardware components, providing dedicated ports for servos, the ESP32, power distribution, and sensor connections. This streamlined layout reduces wiring complexity and optimizes power delivery for consistent performance.

### FS-i6X Transmitter Support: 
- This version supports the FS-i6X transmitter, a popular and powerful hobby-grade controller. The controller setup includes the following:
        Channel Assignments: The first four channels on the FS-iX6 are configured to control SpotMicro’s primary movement functions, including forward-backward and left-right directional movement, as well as additional modes like tilt and reverse.
        Failsafe Configuration: A failsafe mode activates when the signal from the FS-i6X transmitter is lost, ensuring SpotMicro safely stops movement to prevent any unintended actions.

### Improved Movement Control:
- This fork introduces refined gait sequences and step timings, using our custom Arduino code to achieve smooth, natural movement. The gait cycle has been optimized for simultaneous leg movement, allowing SpotMicro to achieve a faster and more stable walk. (in progress)
- (Future) Using the Ramp library for servo control enables smooth interpolation and acceleration for each step phase.

![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/20241112_144816(1).jpg)

## Note of thanks
I want to thank Deok-yeon Kim (KDY0523) for his beautiful and well thought out design of the SpotMicro, without whom this derivate would not have been possible. You can find his original Design here on Thingiverse: https://www.thingiverse.com/thing:3445283

Him sharing the files with an open licence, made it possible to let it grow a community around it. This is why I want to reference to them as well - the SpotMicroAI Community: https://spotmicroai.readthedocs.io/en/latest/

## Preface
This is a work-in-progress, so many bugs an issues may still be present.

## Features
- Modular Code Structure: Each feature is built modularly, making it easier to update individual components without affecting the entire codebase.
- Inverse Kinematics Adjustments: The coordinate system is adapted to provide precise control over each leg, enhancing SpotMicro’s ability to mimic natural walking movements.
- Customizable Gait and Deadzone Control: Includes deadzones for both the forward-backward (FB) and left-right (LR) joystick axes, enhancing movement accuracy. Additional gait options allow for different walking styles, including a square gait cycle with improved vertical foot lift.

This repository is currently just my hobby and as such, it will have its own pace.

## Repo/Code Usage Aims
- Basic Walking and Navigation: Use the FS-i6X joystick to control SpotMicro’s direction. Push forward to walk, pull back to reverse, and move the stick left or right to turn.
- Tilt Mode: This mode uses small shoulder-servo roll offsets and front/rear IK target shifts so the robot can lean while standing. Test this with the robot lifted first, because shoulder servo direction can vary by build.
- Failsafe Activation: If the FS-i6X loses signal, SpotMicro automatically stops, ensuring safety during operation.

## Receiver Code Versions

The Arduino code now has separate receiver-specific sketch folders under `code/Versions`:

- `SpotMicroESP32_Nitro_IBUS` for FlySky FS-i6X / FS-i6B iBus.
- `SpotMicroESP32_Nitro_CRSF_BETA` for CRSF / ExpressLRS (BETA).
- `archive/0.8.6_INDEX_sketch_nov2_GAIT_V8` as an untouched archive copy of the older sketch.

The CRSF (BETA) fork is experimental on this specific dog hardware. The CRSF reader has worked on other ESP32 robot/quadruped projects, but it should be tested with the dog lifted safely or with servos disconnected first.

The CRSF reader files in the CRSF sketch are copied/adapted from [Blacksheep909/ESP32_CRSF_Reader](https://github.com/Blacksheep909/ESP32_CRSF_Reader), which describes itself as a small Arduino / PlatformIO library for reading CRSF / ExpressLRS RC channel frames on ESP32.

Additional Resources

For more information on hardware assembly and some electronics please refer to the [original repository](https://github.com/michaelkubina/SpotMicroESP32) and check out the documentation in this fork for specific PCB and code setup details.

## Contributing

- For contributions or bug reports, please open a GitHub issue or pull request in this fork's repository.

Please use GitHub issues or pull requests for project discussion, bug reports, and contributions.
