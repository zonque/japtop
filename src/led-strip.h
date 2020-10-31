#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "sound-player.h"
class LedStrip {
public:

  struct StartupSequenceStep {
    int millis;
    uint8_t pixel;
    unsigned int hue;
  };

  enum Mode {
    LEDSTRIP_MODE_STARTUP,
    LEDSTRIP_MODE_GAME,
    LEDSTRIP_MODE_DIGITS,
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

  enum BlockColor {
    BLOCKCOLOR_BLUE = 0,
    BLOCKCOLOR_RED,
    BLOCKCOLOR_GREEN,
    BLOCKCOLOR_YELLOW,
    _BLOCKCOLOR_MAX
  };

  enum Direction {
    DIR_LEFT_TO_RIGHT,
    DIR_RIGHT_TO_LEFT,
  };

  explicit LedStrip(int pin, int numLeds, SoundPlayer *soundPlayer);
  void reportButtonState(BlockColor, bool);
  int numPixels();
  void tick();
  void reset();

  void digitUp();
  void digitDown();

private:
  void HSVtoRGB(float h, float s, float v, int *r, int *g, int *b);
  void newBlock();
  void reverseDirection();

  SoundPlayer *soundPlayer;

  Adafruit_NeoPixel *strip;
  Mode mode;
  bool buttonState[_BLOCKCOLOR_MAX];
  int buttonCounter;

  int playerPosition;
  int playerBrightness;
  bool playerBrightnessUp;

  int blockPosition;
  BlockColor blockColor;
  int blockCounter;
  Direction direction;
  int speed;

  // digits mode
  unsigned int digits;
  int digitsModeTimeout;

  StartupSequenceStep *startupStep;
};