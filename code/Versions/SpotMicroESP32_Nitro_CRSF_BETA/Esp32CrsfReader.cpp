#include "Esp32CrsfReader.h"

// Copied/adapted from Blacksheep909/ESP32_CRSF_Reader:
// https://github.com/Blacksheep909/ESP32_CRSF_Reader

#include <string.h>

Esp32CrsfReader::Esp32CrsfReader()
    : serial_(nullptr),
      parseState_(WAIT_ADDR),
      frameLen_(0),
      framePos_(0),
      filterAlpha_(0.25f),
      timeoutMs_(kDefaultTimeoutMs),
      lastFrameMs_(0),
      frameCount_(0) {
  resetChannels();
}

bool Esp32CrsfReader::begin(HardwareSerial &serial, int rxPin, int txPin, uint32_t baud) {
  serial_ = &serial;
  serial_->begin(baud, SERIAL_8N1, rxPin, txPin);
  parseState_ = WAIT_ADDR;
  frameLen_ = 0;
  framePos_ = 0;
  frameCount_ = 0;
  lastFrameMs_ = 0;
  resetChannels();
  return true;
}

bool Esp32CrsfReader::update() {
  return update(millis());
}

bool Esp32CrsfReader::update(uint32_t nowMs) {
  uint8_t type = 0;
  uint8_t payload[kMaxFrameLen] = {0};
  uint8_t payloadLen = 0;
  bool accepted = false;

  while (readFrame(type, payload, payloadLen)) {
    if (type == kTypeRcChannels && payloadLen == kRcPayloadLen) {
      acceptRcFrame(payload, nowMs);
      accepted = true;
    }
  }

  return accepted;
}

bool Esp32CrsfReader::linkAlive() const {
  return linkAlive(millis());
}

bool Esp32CrsfReader::linkAlive(uint32_t nowMs) const {
  return (frameCount_ > 0) && ((nowMs - lastFrameMs_) < timeoutMs_);
}

uint16_t Esp32CrsfReader::channelRaw(uint8_t channelIndex) const {
  if (channelIndex >= kChannelCount) {
    return 0;
  }
  return chRaw_[channelIndex];
}

int Esp32CrsfReader::channelUs(uint8_t channelIndex) const {
  if (channelIndex >= kChannelCount) {
    return 1500;
  }
  return chUs_[channelIndex];
}

float Esp32CrsfReader::channelNormalized(uint8_t channelIndex) const {
  const float value = (static_cast<float>(channelUs(channelIndex)) - 1500.0f) / 500.0f;
  return constrain(value, -1.0f, 1.0f);
}

uint32_t Esp32CrsfReader::frameCount() const {
  return frameCount_;
}

uint32_t Esp32CrsfReader::lastFrameMs() const {
  return lastFrameMs_;
}

void Esp32CrsfReader::setFilterAlpha(float alpha) {
  filterAlpha_ = constrain(alpha, 0.0f, 1.0f);
}

void Esp32CrsfReader::setTimeoutMs(uint32_t timeoutMs) {
  timeoutMs_ = timeoutMs;
}

void Esp32CrsfReader::resetChannels() {
  for (uint8_t i = 0; i < kChannelCount; ++i) {
    chRaw_[i] = 992;
    chUs_[i] = 1500;
    chFilt1_[i] = 1500.0f;
    chFilt2_[i] = 1500.0f;
  }
}

bool Esp32CrsfReader::readFrame(uint8_t &type, uint8_t *payload, uint8_t &payloadLen) {
  if (serial_ == nullptr) {
    return false;
  }

  while (serial_->available()) {
    const uint8_t byte = serial_->read();

    switch (parseState_) {
      case WAIT_ADDR:
        if (byte == kAddrFlightController || byte == kAddrRadioTransmitter || byte == kAddrBroadcast) {
          parseState_ = GOT_LEN;
        }
        break;

      case GOT_LEN:
        frameLen_ = byte;
        if (frameLen_ < 2 || frameLen_ > (kMaxFrameLen - 2)) {
          parseState_ = WAIT_ADDR;
          break;
        }
        framePos_ = 0;
        parseState_ = GOT_PAYLOAD;
        break;

      case GOT_PAYLOAD:
        frameBuf_[framePos_++] = byte;
        if (framePos_ >= frameLen_) {
          const uint8_t expected = crc8DvbS2(frameBuf_, frameLen_ - 1);
          if (expected == frameBuf_[frameLen_ - 1]) {
            type = frameBuf_[0];
            payloadLen = frameLen_ - 2;
            memcpy(payload, &frameBuf_[1], payloadLen);
            parseState_ = WAIT_ADDR;
            return true;
          }
          parseState_ = WAIT_ADDR;
        }
        break;
    }
  }

  return false;
}

void Esp32CrsfReader::acceptRcFrame(const uint8_t *payload, uint32_t nowMs) {
  unpackChannels11(payload, kRcPayloadLen, chRaw_);

  const float secondStageAlpha = constrain(filterAlpha_ * 2.0f, 0.0f, 1.0f);
  for (uint8_t i = 0; i < kChannelCount; ++i) {
    const int sampleUs = crsfToUs(chRaw_[i]);
    chFilt1_[i] += (sampleUs - chFilt1_[i]) * filterAlpha_;
    chFilt2_[i] += (chFilt1_[i] - chFilt2_[i]) * secondStageAlpha;
    chUs_[i] = static_cast<int>(chFilt2_[i] + 0.5f);
  }

  lastFrameMs_ = nowMs;
  ++frameCount_;
}

uint8_t Esp32CrsfReader::crc8DvbS2(const uint8_t *buf, int len) {
  uint8_t crc = 0;
  for (int i = 0; i < len; ++i) {
    crc ^= buf[i];
    for (int bit = 0; bit < 8; ++bit) {
      crc = (crc & 0x80) ? static_cast<uint8_t>((crc << 1) ^ 0xD5)
                         : static_cast<uint8_t>(crc << 1);
    }
  }
  return crc;
}

int Esp32CrsfReader::crsfToUs(uint16_t value) {
  value = constrain(value, static_cast<uint16_t>(172), static_cast<uint16_t>(1811));
  return 1000 + ((static_cast<int32_t>(value) - 172) * 1000L) / 1639L;
}

void Esp32CrsfReader::unpackChannels11(const uint8_t *payload, uint8_t payloadLen, uint16_t *out) {
  for (uint8_t i = 0; i < kChannelCount; ++i) {
    uint16_t value = 0;
    const uint32_t channelBitStart = static_cast<uint32_t>(i) * 11U;
    for (uint8_t bit = 0; bit < 11; ++bit) {
      const uint32_t srcBit = channelBitStart + bit;
      const uint8_t byteIndex = srcBit >> 3;
      if (byteIndex >= payloadLen) {
        break;
      }
      if ((payload[byteIndex] & (1U << (srcBit & 7))) != 0) {
        value |= (1U << bit);
      }
    }
    out[i] = value;
  }
}
