### Overview of the PCB Design and Installation
![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/v3-blue.png)

The provided PCB template is designed to simplify the assembly and wiring of your robot dog. It's intended as a flexible base where you can easily install components by soldering female pin headers onto the board. This allows components to be attached or removed as needed without complex soldering each time. 

This board includes pinouts for every expected component, such as:

- **ESP32 Dev Kit**: Used as the primary microcontroller.
- **iBus RC Receiver (FS-i6B)**: For remote control capability.
- **PCA9685**: Controls servo motors.
- **Voltage and Current Sensors**: Monitors power levels.
- **Ultrasonic Sensors**: For object detection.
- **OLED Display**: For real-time feedback display.
- **Relay Module (KY-019)**: To control high-power components.
- **IMU (MPU6050)**: For motion tracking.
- **KY-006 BUZZER**: For Beeps and Boops.

The design also anticipates future additions, like an **ESP32-CAM** module, but currently, the appropriate pinouts are NOT included.

### Installation Instructions

- There are multiple ways of approaching the PCBs components, one is two use female pin headers, which lets you easily attach/detach components. The other method is not using female pins, and directly soldering the components to the board. This method is what I reccomend, as it is more reliable, and lets you cut wires for components such as the screen, at your desired length.

1. **Soldering Female Pin Headers**: Begin by soldering female pin headers to the PCB. There are two way sThis setup enables you to plug in each component rather than soldering them directly, making it easier to swap out or troubleshoot components.

2. **Mounting the ESP32**: Insert the ESP32 Dev Kit into the designated header pins on the PCB. This board will serve as the main control unit, connecting to other components through the pin headers.

3. **Connecting the PCA9685 Servo Driver**: Insert the PCA9685 module into the specific header on the PCB. This module controls the servos, allowing movement of the robot's legs. The PCA9685 has a power output on the board -you will need to solder wires from this output, either into the terminal, or by breaking off the terminal carefully and attaching them straight to the board's pads.

4. **Adding Sensors**:
   - **Voltage and Current Sensors**: These sensors plug into their designated slots and help monitor power usage, which is especially useful for managing battery life.
   - **Ultrasonic Sensors**: These can be added in the front of the robot to help detect obstacles and avoid collisions.

5. **Connecting the RC Receiver (FS-i6B)**: The iBus receiver plugs into the board and allows remote control of the robot through the FS-i6B transmitter. This receiver supports multiple channels, giving you control over various functions of the robot dog.
   
![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/1100x1100.jpg)

   - Here is a photo of where to connect the 3-pin connector to the FS-i6B. We want to use the 'IBUS SERVO' pins on the reciever, which are outlined in green on the photo.

### Optional CRSF / ExpressLRS Receiver Wiring

If you are using the `SpotMicroESP32_Nitro_CRSF_BETA` sketch instead of the iBus sketch, wire a CRSF / ExpressLRS receiver to a hardware UART on the ESP32.

Default CRSF sketch wiring:

| CRSF / ExpressLRS receiver | ESP32 DevKitC |
| --- | --- |
| TX | GPIO26 |
| RX | GPIO17, optional telemetry |
| GND | GND |
| 5V or 3V3 | Receiver-appropriate power |

Most receive-only robot control only needs receiver `TX` to ESP32 `RX`, plus power and common ground. Receiver `RX` is only needed later if telemetry back to the handset is added.

GPIO26 is used by default because GPIO16 is already assigned to the buzzer in the current dog code. If your PCB or wiring uses a different UART pin, update `kCrsfRxPin` in `code/Versions/SpotMicroESP32_Nitro_CRSF_BETA/SpotMicroESP32_Nitro_CRSF_BETA.ino`.

The CRSF (BETA) implementation has not yet been tested on this SpotMicroESP32-Nitro dog hardware. Test receiver channel output with the robot lifted safely, or with the servos disconnected, before using it on the dog.

6. The **KY-006 Buzzer** can be attached to output pins on the PCB. I reccommend placing the Buzzer somewhere in the dog where it is still inside the shell, and won't interfere with other parts. You can also solder it directly to the board.

7. **OLED Display and Relay Module**:
   - **OLED Display**: Plug in the OLED to display data like battery voltage, current action state, and other information.
   - **Relay Module (KY-019)**: The relay module slot is ready to handle higher power components safely.
   
### Mounting Component Photos
![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/pcbassemble.jpg)
*The board in this photo was the first prototype, so a couple of traces are missing here, but are now updated in the newest version!*
### Future Expansion

The PCB in the future could have additional pinouts including one for an ESP32-CAM module, which can be used for video or facial recognition. This slot is NOT yet populated, but the design allows for easy addition in the future, with straightforward integration into the PCB.

### Example Images

Here are images of the board and setup to help guide you through the assembly:

1. **Robot Dog’s Internal Setup**:
   ![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/finishedpcb.jpg)
   ![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/finishedwideangle.jpg)
   - This image shows the internal layout of the robot, with components neatly arranged on the PCB template. The wiring is "organized" so that the top cover will fit on.
   - You may notice an extra switch for the FS-i6B Reciever for power -this is no longer neccesary, as the code now has a way of disabling noise from the reciever when uploading new code.
3. **PCB Design**:
   ![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/schematic-v3.png)
   
   - This shows the layout of the PCB with labeled areas for each component. Follow these labels to install each part correctly.

5. **Wiring Schematic**:
   ![SpotMicroESP32](https://github.com/Blacksheep909/SpotMicroESP32/blob/master/electronics/Images/image.png)
   - The schematic provides a complete wiring overview, detailing connections between the ESP32, sensors, motor drivers, and other peripherals.
   - Some parts are missing from this original schematic, but are included on the PCB design downloadable above. Parts that are missing from this schematic include: KY-006 (buzzer) and FS-I6B (reciever)
     
   ## This schematic is updated from the original schematic as two of the I2C were moved to accomodate the ESP32 DevKitC V4. 


### SERVO DRIVER - PCA9685 ###

Manufacturer Homepage: https://www.nxp.com/products/power-management/lighting-driver-and-controller-ics/ic-led-controllers/16-channel-12-bit-pwm-fm-plus-ic-bus-led-controller:PCA9685
Datasheet: https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf

### 0,96" OLED - SSD1306 ###

Manufacturer Homepage: https://www.solomon-systech.com/en/product/advanced-display/oled-display-driver-ic/ssd1306/
Datasheet: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
