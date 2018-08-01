#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedStrip {
public:

  struct StartupSequenceStep {
    int millis;
    uint8_t pixel;
    uint8_t hue;
  };

  enum Mode {
    LEDSTRIP_MODE_STARTUP,
    LEDSTRIP_MODE_ALL,
    LEDSTRIP_MODE_MASKED,
    LEDSTRIP_MODE_SINGLE,
    LEDSTRIP_MODE_CHASING,
    LEDSTRIP_MODE_RAINBOW,
    LEDSTRIP_MODE_MAX
  };

  const float ColorRed = 0.0f;

  enum Hue {
    LEDSTRIP_HUE_RED = 0,
    LEDSTRIP_HUE_YELLOW = 30,
    LEDSTRIP_HUE_GREEN = 120,
    LEDSTRIP_HUE_CYAN = 180,
    LEDSTRIP_HUE_BLUE = 240,
    LEDSTRIP_HUE_MAGENTA = 300,
  };

  explicit LedStrip(int pin, int numLeds);
  int numPixels();
  void tick();
  void setHSV(int h, int v, int s);
  void increaseHue(int delta);
  Mode mode() { return _mode; }
  void setMode(Mode mode);
  void setPosition(unsigned int position);
  void reverseDirection();
  void setRandomMask();

private:
  void HSVtoRGB(float h, float s, float v, int *r, int *g, int *b);

  Adafruit_NeoPixel *_strip;
  Mode _mode;
  int _position;
  int _h, _s, _v;
  int _counter;
  bool *_active_mask;
  bool _direction;
  StartupSequenceStep *_step;
};