#include "analog-button.h"

AnalogButton::AnalogButton(int pin, AnalogButtonCallback cb)
{
  _pin = pin;
  _cb = cb;
  _changeCount = 0;
  _lastState = false;
}

void AnalogButton::tick()
{
  bool state = analogRead(_pin) < 100;

  if (state != _lastState)
    _changeCount++;
  else
    _changeCount = 0;

  if (_changeCount > 10) {
    _lastState = state;
    _changeCount = 0;
    _cb(state);
  }
}