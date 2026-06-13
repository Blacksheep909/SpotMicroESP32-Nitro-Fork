#pragma once

// Copied/adapted from Blacksheep909/ESP32_CRSF_Reader:
// https://github.com/Blacksheep909/ESP32_CRSF_Reader

#include <Arduino.h>

class Esp32CrsfReader {
 public:
  static constexpr uint32_t kDefaultBaud = 420000;
  static constexpr uint32_t kDefaultTimeoutMs = 1000;
  static constexpr uint8_t kChannelCount = 16;

  Esp32CrsfReader();

  bool begin(HardwareSerial &serial, int rxPin, int txPin, uint32_t baud = kDefaultBaud);

  bool update();
  bool update(uint32_t nowMs);

  bool linkAlive() const;
  bool linkAlive(uint32_t nowMs) const;

  uint16_t channelRaw(uint8_t channelIndex) const;
  int channelUs(uint8_t channelIndex) const;
  float channelNormalized(uint8_t channelIndex) const;

  uint32_t frameCount() const;
  uint32_t lastFrameMs() const;

  void setFilterAlpha(float alpha);
  void setTimeoutMs(uint32_t timeoutMs);

 private:
  static constexpr uint8_t kAddrFlightController = 0xC8;
  static constexpr uint8_t kAddrRadioTransmitter = 0xEA;
  static constexpr uint8_t kAddrBroadcast = 0x00;
  static constexpr uint8_t kTypeRcChannels = 0x16;
  static constexpr int kMaxFrameLen = 64;
  static constexpr int kRcPayloadLen = 22;

  enum ParseState : uint8_t {
    WAIT_ADDR = 0,
    GOT_LEN = 1,
    GOT_PAYLOAD = 2,
  };

  HardwareSerial *serial_;
  ParseState parseState_;
  uint8_t frameLen_;
  uint8_t framePos_;
  uint8_t frameBuf_[kMaxFrameLen];

  uint16_t chRaw_[kChannelCount];
  int chUs_[kChannelCount];
  float chFilt1_[kChannelCount];
  float chFilt2_[kChannelCount];

  float filterAlpha_;
  uint32_t timeoutMs_;
  uint32_t lastFrameMs_;
  uint32_t frameCount_;

  void resetChannels();
  bool readFrame(uint8_t &type, uint8_t *payload, uint8_t &payloadLen);
  void acceptRcFrame(const uint8_t *payload, uint32_t nowMs);

  static uint8_t crc8DvbS2(const uint8_t *buf, int len);
  static int crsfToUs(uint16_t value);
  static void unpackChannels11(const uint8_t *payload, uint8_t payloadLen, uint16_t *out);
};
