#include "led-strip.h"

static struct LedStrip::StartupSequenceStep StartupSequenceSteps[] = {
  { 1450, 4, LedStrip::LEDSTRIP_HUE_BLUE },
  { 1450, 5, LedStrip::LEDSTRIP_HUE_BLUE },

  { 1700, 3, LedStrip::LEDSTRIP_HUE_RED },
  { 1700, 6, LedStrip::LEDSTRIP_HUE_RED },

  { 1950, 2, LedStrip::LEDSTRIP_HUE_GREEN },
  { 1950, 7, LedStrip::LEDSTRIP_HUE_GREEN },

  { 2400, 1, LedStrip::LEDSTRIP_HUE_YELLOW },
  { 2400, 8, LedStrip::LEDSTRIP_HUE_YELLOW },

  { 2650, 0, LedStrip::LEDSTRIP_HUE_CYAN },
  { 2650, 9, LedStrip::LEDSTRIP_HUE_CYAN },

  { 6000, 0, LedStrip::LEDSTRIP_HUE_MAGENTA },
  { 6000, 0, LedStrip::LEDSTRIP_HUE_MAGENTA },

  { -1 }
};

LedStrip::LedStrip(int pin, int numLeds, SoundPlayer *sp) {
  soundPlayer = sp;
  mode = LEDSTRIP_MODE_STARTUP;

  buttonState[BLOCKCOLOR_BLUE] = false;
  buttonState[BLOCKCOLOR_RED] = false;
  buttonState[BLOCKCOLOR_GREEN] = false;
  buttonState[BLOCKCOLOR_YELLOW] = false;
  buttonCounter = 0;

  digits = 0;

  startupStep = &StartupSequenceSteps[0];
  strip = new Adafruit_NeoPixel(numLeds, pin, NEO_GRB | NEO_KHZ800);
  strip->begin();
  strip->show();

  reset();
}

int LedStrip::numPixels() {
  return strip->numPixels();
}

void LedStrip::reset() {
  direction = DIR_RIGHT_TO_LEFT;
  speed = 300;

  playerPosition = 0;
  playerBrightness = 0;
  playerBrightnessUp = true;

  newBlock();
}

void LedStrip::reportButtonState(BlockColor color, bool state) {
  buttonState[color] = state;

  if (state)
    buttonCounter++;
  else
    buttonCounter--;
}

void LedStrip::newBlock() {
  if (direction == DIR_LEFT_TO_RIGHT)
    blockPosition = 0;
  else
    blockPosition = numPixels() - 1;

  blockColor = (BlockColor) random(_BLOCKCOLOR_MAX);
  blockCounter = speed;
}

void LedStrip::reverseDirection() {
  if (direction == DIR_LEFT_TO_RIGHT)
    direction = DIR_RIGHT_TO_LEFT;
  else
    direction = DIR_LEFT_TO_RIGHT;

  soundPlayer->play(2, 4);
}

void LedStrip::digitUp() {
  mode = LEDSTRIP_MODE_DIGITS;
  digitsModeTimeout = 1000;

  if (digits < numPixels()) {
    digits++;
    soundPlayer->play(10, digits);
  }
}

void LedStrip::digitDown() {
  mode = LEDSTRIP_MODE_DIGITS;
  digitsModeTimeout = 1000;

  if (digits > 1) {
    digits--;
    soundPlayer->play(10, digits);
  }
}

void LedStrip::tick() {
  int r, g, b;

  switch (mode) {
    case LEDSTRIP_MODE_STARTUP:
      if (startupStep->millis < 0) {
        mode = LEDSTRIP_MODE_GAME;
        break;
      }

      if (millis() > (unsigned) startupStep->millis) {
        HSVtoRGB(startupStep->hue, 100, 100, &r, &g, &b);
        strip->setPixelColor(startupStep->pixel, r, g, b);
        startupStep++;
      }

      break;

    case LEDSTRIP_MODE_GAME:
      for (unsigned int i = 0; i < strip->numPixels(); i++)
        strip->setPixelColor(i, 0, 0, 0);

      int blockR, blockG, blockB;
      float blockHue;

      switch (blockColor) {
        case BLOCKCOLOR_BLUE:
          blockHue = LEDSTRIP_HUE_BLUE;
          break;
        case BLOCKCOLOR_RED:
          blockHue = LEDSTRIP_HUE_RED;
          break;
        case BLOCKCOLOR_GREEN:
          blockHue = LEDSTRIP_HUE_GREEN;
          break;
        case BLOCKCOLOR_YELLOW:
          blockHue = LEDSTRIP_HUE_YELLOW;
          break;
        case _BLOCKCOLOR_MAX:
          break;
      }

      if (blockCounter-- == 0) {
        blockCounter = speed;

        switch (direction) {
        case DIR_LEFT_TO_RIGHT:
          if (blockPosition + 1 == playerPosition) {
            if (buttonState[blockColor] && buttonCounter == 1) {
              playerPosition--;
              soundPlayer->play(2, 2);
              speed--;
              if (playerPosition == 0)
                reverseDirection();

              newBlock();
            } else if (playerPosition < numPixels()-1) {
              playerPosition++;
              blockPosition++;
              soundPlayer->play(2, 3);
            }
          } else {
            blockPosition++;
          }
          break;

        case DIR_RIGHT_TO_LEFT:
          if (blockPosition - 1 == playerPosition) {
            if (buttonState[blockColor] && buttonCounter == 1) {
              playerPosition++;
              soundPlayer->play(2, 2);
              speed--;
              if (playerPosition == numPixels() - 1)
                reverseDirection();

              newBlock();
            } else if (playerPosition > 0) {
              playerPosition--;
              blockPosition--;
              soundPlayer->play(2, 3);
            }
          } else {
            blockPosition--;
          }
          break;
        }
      }

      HSVtoRGB(blockHue, 100, 100, &blockR, &blockG, &blockB);
      strip->setPixelColor(blockPosition, blockR, blockG, blockB);

      if (playerBrightnessUp) {
        if (playerBrightness == 255)
          playerBrightnessUp = false;
        else
          playerBrightness++;
      } else {
        if (playerBrightness == 0)
          playerBrightnessUp = true;
        else
          playerBrightness--;
      }

      strip->setPixelColor(playerPosition, playerBrightness, playerBrightness, playerBrightness);
      break;

    case LEDSTRIP_MODE_DIGITS:
      for (unsigned int i = 0; i < strip->numPixels(); i++) {
        if (i < digits)
          strip->setPixelColor(i, 0, 255, 255);
        else
          strip->setPixelColor(i, 0, 0, 0);
      }

      if (digitsModeTimeout-- == 0) {
        mode = LEDSTRIP_MODE_GAME;
        reset();
      }

    default:
      break;
  }

  strip->show();
}

// Taken from https://gist.github.com/hdznrrd/656996
void LedStrip::HSVtoRGB(float h, float s, float v, int *r, int *g, int *b)
{
  int i;
  float f, p, q, t;

  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));

  s /= 100;
  v /= 100;

  if (s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v * 255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));

  switch (i) {
  case 0:
    *r = round(255 * v);
    *g = round(255 * t);
    *b = round(255 * p);
    break;

  case 1:
    *r = round(255 * q);
    *g = round(255 * v);
    *b = round(255 * p);
    break;

  case 2:
    *r = round(255 * p);
    *g = round(255 * v);
    *b = round(255 * t);
    break;

  case 3:
    *r = round(255 * p);
    *g = round(255 * q);
    *b = round(255 * v);
    break;

  case 4:
    *r = round(255 * t);
    *g = round(255 * p);
    *b = round(255 * v);
    break;

  default: // case 5:
    *r = round(255 * v);
    *g = round(255 * p);
    *b = round(255 * q);
    break;
  }
}