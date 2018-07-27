#include "rotary-encoder.h"

RotaryEncoder::RotaryEncoder(int pinA, int pinB, RotaryEncoderCallback cb)
{
  _r = new Rotary(pinA, pinB);
  _cb = cb;
}

void RotaryEncoder::tick()
{
  unsigned char result = _r->process();

  switch (result) {
    case DIR_CCW:
      _cb(false);
      break;

    case DIR_CW:
      _cb(true);
      break;

    case DIR_NONE:
    default:
      // do nothing
      break;
  }
}
