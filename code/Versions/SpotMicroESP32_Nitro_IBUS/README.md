# SpotMicroESP32 Nitro IBUS

Use this sketch for the FlySky FS-i6X transmitter with an FS-i6B / iBus receiver.

## Receiver

- Protocol: FlySky iBus.
- Code input: `IBusSerial.begin(115200, SERIAL_8N1, 3, -1)`.
- Receiver signal: existing iBus wiring used by the Nitro PCB/code.

## Failsafe

The FS-i6B can keep outputting configured failsafe channel values instead of simply stopping serial output. This sketch treats two cases as unsafe:

- Any of the first six RC channels is outside the valid receiver range.
- The configured failsafe channel pattern is present:
  - zero-indexed channel `2` >= `1965`
  - zero-indexed channel `3` >= `1750`

When unsafe, the sketch enters failsafe once, stops walking, shows `FAILSAFE!`, and commands the dog to lie down if it was standing. It does not block inside a failsafe `while` loop.

Test failsafe with the robot lifted safely before trusting it on the ground.

Reference: [INAV receiver documentation](https://github.com/iNavFlight/inav/blob/e7d5cceb06878ba82b35695131fa68ab9249032b/docs/Rx.md) describes iBus as the FlySky serial receiver protocol and lists receiver data loss, serial receiver failsafe indication, and out-of-range control channel values as signal-loss detection cases.

## Tilt Mode

Channel 5 low selects tilt mode while the dog is standing. Channel 1 controls a small shoulder-servo roll offset, and channel 2 shifts the front/rear leg IK targets for pitch.

The defaults are intentionally conservative: `10` degrees maximum shoulder offset and `3` IK-coordinate units maximum pitch shift. Test with the robot lifted first, because the shoulder servo sign may need reversing on a specific build.
