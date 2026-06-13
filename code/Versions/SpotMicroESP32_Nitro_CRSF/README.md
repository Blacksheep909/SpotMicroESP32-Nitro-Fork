# SpotMicroESP32 Nitro CRSF

Use this sketch for CRSF / ExpressLRS receivers.

This CRSF implementation has not yet been tested on the SpotMicroESP32-Nitro dog hardware. The `Esp32CrsfReader` code has worked on other ESP32 robot/quadruped projects, but test this sketch with the robot lifted safely or with servos disconnected before using it on the dog.

The `Esp32CrsfReader` files in this folder are copied/adapted from [Blacksheep909/ESP32_CRSF_Reader](https://github.com/Blacksheep909/ESP32_CRSF_Reader). That repo describes the library as a CRSF / ExpressLRS RC channel reader for ESP32 projects, not a complete CRSF telemetry/device implementation.

## Receiver

- Protocol: CRSF serial.
- Baud: `420000`.
- Code input: `crsf.begin(CrsfSerial, kCrsfRxPin, kCrsfTxPin)`.
- Default RX pin: GPIO26.
- Default optional telemetry TX pin: GPIO17.

## Wiring

| CRSF / ExpressLRS receiver | ESP32 DevKitC |
| --- | --- |
| TX | GPIO26 |
| RX | GPIO17, optional telemetry |
| GND | GND |
| 5V or 3V3 | Receiver-appropriate power |

Most receive-only setups only need receiver TX to ESP32 RX, plus power and common ground. Receiver RX is only needed later if telemetry back to the handset is added.

GPIO26 is used instead of GPIO16 because GPIO16 is already assigned to the buzzer in this dog code.

Helpful setup references:

- [ExpressLRS receiver wiring](https://www.expresslrs.org/quick-start/receivers/wiring-up/)
- [ExpressLRS receiver serial protocols](https://www.expresslrs.org/software/serial-protocols/)

## Failsafe

This sketch uses `crsf.linkAlive()` and channel range checks. If fresh CRSF frames are not being received, or channel data is invalid, the sketch enters failsafe, stops walking, and commands the dog to lie down if it was standing.
