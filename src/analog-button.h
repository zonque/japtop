#pragma once

#include <Arduino.h>

typedef void (*AnalogButtonCallback)(bool state);

class AnalogButton {
public:
  explicit AnalogButton(int pin, AnalogButtonCallback cb);
  void tick();

private:
  int _pin;
  int _changeCount;
  bool _lastState;
  AnalogButtonCallback _cb;
};