# SpotMicroESP32 Nitro Code Versions

Pick one receiver-specific sketch folder:

- `SpotMicroESP32_Nitro_IBUS` - FlySky FS-i6X / FS-i6B iBus receiver version.
- `SpotMicroESP32_Nitro_CRSF_BETA` - CRSF / ExpressLRS receiver version (BETA).
- `archive/0.8.6_INDEX_sketch_nov2_GAIT_V8` - untouched copy of the older sketch before the receiver split.

Open only the folder that matches your receiver. The iBus and CRSF versions are intentionally separate so users can download or duplicate only the code for their receiver.

## Current Safety Notes

- The iBus version keeps support for the existing FS-i6B failsafe channel pattern and also treats out-of-range receiver values as unsafe.
- The CRSF (BETA) version uses `Esp32CrsfReader` and `crsf.linkAlive()` for receiver freshness.
- The CRSF (BETA) version has not yet been tested on this SpotMicroESP32-Nitro dog hardware. The CRSF reader has worked on other ESP32 robot/quadruped projects, but first tests should be done with the robot lifted or servos disconnected.
- The CRSF reader files are copied/adapted from [Blacksheep909/ESP32_CRSF_Reader](https://github.com/Blacksheep909/ESP32_CRSF_Reader).

## CRSF Default Pins

| CRSF / ExpressLRS receiver | ESP32 DevKitC |
| --- | --- |
| TX | GPIO26 |
| RX | GPIO17, optional telemetry |
| GND | GND |
| 5V or 3V3 | Receiver-appropriate power |

The CRSF reader example originally uses GPIO16/RX2, but this dog code already uses GPIO16 for the buzzer. GPIO26 avoids that conflict.
