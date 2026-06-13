# SpotMicroESP32 Arduino Code

This directory contains the Arduino code for controlling the SpotMicroESP32 robot dog. The structure includes files for managing gait, kinematics, and setup procedures, tailored for integration with our custom PCB and the FS-i6 hobby controller.

## Coding by Charlie Frater & Beckett Macdonald ##

- Below is some instructions on how to quickstart the provided (current) Arduino Code.

## Choose Your Receiver Version

The current code is split into separate receiver-specific sketches. Pick the folder that matches your receiver setup instead of trying to combine both receiver types in one sketch:

- `code/Versions/SpotMicroESP32_Nitro_IBUS` - for FlySky FS-i6X / FS-i6B iBus setups.
- `code/Versions/SpotMicroESP32_Nitro_CRSF_BETA` - for CRSF / ExpressLRS receiver setups (BETA).
- `code/Versions/archive/0.8.6_INDEX_sketch_nov2_GAIT_V8` - untouched archive copy of the older sketch.

The CRSF (BETA) implementation has not yet been tested on this SpotMicroESP32-Nitro dog hardware. The CRSF reader code has worked on other ESP32 robot/quadruped projects, but test this fork with the robot lifted safely, or with the servos disconnected, before trusting it on the dog.

The CRSF reader files included in `SpotMicroESP32_Nitro_CRSF_BETA` are copied/adapted from [Blacksheep909/ESP32_CRSF_Reader](https://github.com/Blacksheep909/ESP32_CRSF_Reader). That project is focused on reading CRSF / ExpressLRS RC channel frames on ESP32 and does not implement the full CRSF telemetry/device ecosystem.

### iBus Failsafe Note

The FS-i6X / FS-i6B failsafe can be awkward because the receiver may keep outputting configured failsafe channel positions over iBus instead of simply disappearing. That means firmware should not rely only on "no data received" to detect a failsafe. The iBus fork now treats invalid channel ranges and the configured failsafe channel pattern as unsafe, then lies the dog down without blocking inside a failsafe loop.

Useful external reference: [INAV's receiver documentation](https://github.com/iNavFlight/inav/blob/e7d5cceb06878ba82b35695131fa68ab9249032b/docs/Rx.md) describes iBus as the FlySky serial receiver protocol for FS-IA6B/FS-X6B/FS-IA10-style receivers and notes that signal loss can be detected through no RX data, serial receiver failsafe indication, or control-channel values outside configured valid ranges.

### CRSF Wiring Note

The CRSF fork uses `Serial2` and defaults to:

| CRSF / ExpressLRS receiver | ESP32 DevKitC |
| --- | --- |
| TX | GPIO26 |
| RX | GPIO17, optional telemetry |
| GND | GND |
| 5V or 3V3 | Receiver-appropriate power |

GPIO26 is used as the default CRSF RX pin because GPIO16 is already used by the buzzer in this dog code. If you use a different ESP32 pin, update `kCrsfRxPin` in `SpotMicroESP32_Nitro_CRSF_BETA.ino`.

Official ExpressLRS references:

- [ExpressLRS receiver wiring](https://www.expresslrs.org/quick-start/receivers/wiring-up/)
- [ExpressLRS receiver serial protocols](https://www.expresslrs.org/software/serial-protocols/)

## Features

- **Joystick-Controlled Movement**: Supports forward-backward (FB) and left-right (LR) control using the FS-i6 transmitter, with configurable deadzones for precise control.
- **Failsafe Functionality**: Integrated failsafe handling stops the robot safely if the FS-i6 signal is lost.
- **Gait Customization**: Choose between square and arc gait cycles, with options to control timing, speed, and deadzones.
- **Tilt Mode**: Allows the robot to tilt its head or body to look around, providing an additional range of motion.
- **Servo Interpolation Using Ramp Library**: Smooth, controlled movements with acceleration and deceleration support for more lifelike walking.

## Code Structure

- **`INDEX_sketch_(DATE-CODENAME).ino`**: Main program loop and high-level control functions. Manages gait logic and overall behavior.
- **`Kinematics.ino`**: Handles inverse kinematics calculations for each leg, translating movement commands into precise leg positions.
- **`Procedure_Functions_Setup.ino`**: Includes setup procedures for initializing servos, sensors, and any other components. This file may also define utility functions for specific actions or movements.

## Getting Started

### Requirements

- **Hardware**:
  - SpotMicroESP32 robot dog with our custom PCB.
  - One supported receiver setup:
    - FS-i6X transmitter with FS-i6B / iBus receiver for `SpotMicroESP32_Nitro_IBUS`.
    - CRSF / ExpressLRS receiver for `SpotMicroESP32_Nitro_CRSF_BETA`.
![Reciever](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/code/FS-I6X%2B20160331.450(1).png)

- **Software**:
  - [Arduino IDE](https://www.arduino.cc/en/software)
  
- **Required Libraries**:
  Below are the libraries required for this project. Be sure to download and install them in your Arduino IDE under `Sketch > Include Library > Add .ZIP Library...`:

  - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO): Used for I2C/SPI communication in various sensors.
  - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library): For graphical functions on displays.
  - [Adafruit MPU6050](https://github.com/adafruit/Adafruit_MPU6050): Provides support for the MPU6050 accelerometer/gyroscope.
  - [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library): Controls servos via the PCA9685 driver.
  - [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306): For OLED display support (e.g., 0.96" OLED).
  - [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor): Standardized sensor library used by many Adafruit sensor libraries.
  - [AxTypeTraits](https://github.com/xoRaxes/AxTypeTraits): Provides type-trait functionalities, required by some Arduino projects.
  - [Bonezegi PCA9685](https://github.com/bonezegi/PCA9685): An alternative PCA9685 library for servo control.
  - [IBusBM](https://github.com/bolderflight/IBusBM): Required for the FS-i6 / iBus version.
  - [Ramp](https://github.com/Erriez/ErriezRamp): Controls ramping (acceleration/deceleration) of servo movements.

  The CRSF version includes `Esp32CrsfReader.h` and `Esp32CrsfReader.cpp` directly in the sketch folder, copied/adapted from [Blacksheep909/ESP32_CRSF_Reader](https://github.com/Blacksheep909/ESP32_CRSF_Reader).

- All of these libraries can be accessed within the Arduino IDE in the Libraries tab
### Setup and Configuration

1. **Configure Arduino Code**:
   - Open `INDEX_sketch_(DATE-CODENAME).ino` to customize settings for the robot’s movement, including:
     - **Failsafe settings** for signal loss protection (this is only if you intend to change the current setup)
     - **LIPO BATTERY Cell Count (Very important)

2. **Upload to ESP32**:
### VERY IMPORTANT: 
   - **When installing ESP32 board manager by Espressif Systems, make sure to have an edition NO LATER than **2.0.17**, as versions any later than that will cause issues with IBUS-BM, causing the code to not upload or function!**

There may be another fix for this, or the IBUS-BM library may be updated to be compatible, but for now this is the *only* fix I have found.

   - Ensure all libraries are installed and configured in the Arduino IDE.
   - Connect the ESP32 to your computer and upload the code.

4. **Bind, Calibrate and Test**:
   - Follow [this video tutorial](https://www.youtube.com/watch?v=v5SXRkQMgkA) on how to bind the reciever and the transmitter (FS-i6X and FS-i6B).
   - With the FS-i6 powered on, test each movement mode and adjust parameters in the code as needed for optimal performance.

### Usage

- **Basic Movements**:
  - Use the FS-i6 joystick for basic navigation.
  - Forward-backward (FB) control is prioritized, with left-right (LR) control enabled when FB is neutral.
- **Tilt Mode**:
  - Engage tilt mode via joystick to adjust SpotMicro’s head position.
- **Failsafe Activation**:
  - When the FS-i6 signal is lost, the robot stops moving automatically and returns to idle position.

### Code Flowchart

![Flowchart](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/code/detailed_robot_dog_flowchart.png)


### Steps to Set Up Failsafe on the FS-i6X

A Failsafe on the FS-i6X transmitter is essential to ensure that your robot dog safely stops or returns to a safe position if it loses signal from the transmitter. This process essentially "teaches" the transmitter what signal to send when a connection loss occurs, allowing you to set specific servo positions or actions to minimize risk.

1. **Access the Failsafe Settings:**
   Power on your FS-i6X transmitter and navigate to the "RX Setup" menu. From there, select the "Failsafe" option, which allows you to set failsafe values for each channel.

2. **Set Desired Positions:**
   For each channel, adjust the controls to position the servos or outputs in the desired failsafe position (e.g., legs in a stable position, motors off). Once the controls are in the preferred positions, select "Set" to lock in these values as the failsafe.

3. **Confirm Failsafe Activation:**
   After setting, you can test the failsafe by powering off the transmitter to simulate signal loss. The servos should move to the predefined failsafe positions.

For a more detailed, step-by-step visual example, you can refer to [this video tutorial](https://www.youtube.com/watch?v=4N_hHfpEoHY).

*Photo here shows what it should look similar to.*
![Transmitter](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/code/20241114_190217(1).jpg)

### Steps to Set Up Output MODE on the FS-i6X
To ensure your FS-i6X transmitter is set to output in "PWM" and "I-BUS" mode, follow these steps:

1. **Access the Output Mode Menu:**
   - Power on the FS-i6X transmitter.
   - Press and hold the "OK" button to enter the main menu.
   - Use the directional buttons (up & down) to scroll to "System Setup," and press "OK" to select it.

2. **Select Output Mode:**
   - In the "System Setup" menu, go to 'RX Setup' press 'OK'. Next scroll down to find the "Output Mode" option (it's down a page), then press "OK" to open it.
   - You should see options for various output modes, including "PWM" and "I-BUS."

3. **Set to PWM and I-BUS:**
   - Highlight "PWM" to enable standard servo signal output for each channel.
   - Then select "I-BUS" if you need the transmitter to communicate with compatible receivers using FlySky's I-BUS protocol, often used for sensor data or expanded channel output.
   - Confirm your selections by pressing "OK."

4. **Save and Exit:**
   - Once both "PWM" and "I-BUS" are selected, HOLD the "CANCEL" button to exit back to the main screen.
   - The settings should now be saved, and your transmitter will operate in PWM and I-BUS modes. (settings will only be saved if cancel button is HELD)

Following these steps will configure your FS-i6X transmitter to ensure compatibility with devices using PWM and I-BUS outputs.

*Picture below of what the settings menu should look like when in the correct output mode*
![OUTPUT](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/code/20241114_211004(1).jpg)


# Future development
- The gait and inverse kinematics are currently still in the early-ish side of development. They work, but can always be better of course.
- I would like to add a tilt mode feature where the user can look around with the dog using a joystick.
- Another feature I would like to add is a move advanced walking gait -this is currently my main aim with the code
- A missing feature, which is of course fully neccesary is the LR code (left-right), so the dog can actually turn, but currently the shoulders are not included in the inverse kinematics.

There are many other things I am likely forgetting, and of course this will happen in due time, but I am unsure how long they may take -right now my aim is to upload this code to get people kickstarted, and maybe someone will take on the challenge to make it even better!

