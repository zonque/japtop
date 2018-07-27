#pragma once

#include <Arduino.h>
#include <Rotary.h>

typedef void (*RotaryEncoderCallback)(bool cw);

class RotaryEncoder {
public:
  explicit RotaryEncoder(int pinA, int pinB, RotaryEncoderCallback cb);
  void tick();

private:
  Rotary *_r;
  RotaryEncoderCallback _cb;
};