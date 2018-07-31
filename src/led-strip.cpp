#include "led-strip.h"

static const struct LedStrip::StartupSequenceStep StartupSequenceSteps[] = {
  { 1450, 3, LedStrip::LEDSTRIP_HUE_BLUE },
  { 1700, 4, LedStrip::LEDSTRIP_HUE_RED },
  { 1950, 5, LedStrip::LEDSTRIP_HUE_GREEN },
  { 2400, 6, LedStrip::LEDSTRIP_HUE_YELLOW },
  { 2650, 7, LedStrip::LEDSTRIP_HUE_CYAN },
  { 6000, 0, LedStrip::LEDSTRIP_HUE_MAGENTA },
  { -1 }
};

LedStrip::LedStrip(int pin, int numLeds)
{
  _counter = 0;
  _h = _s = _v = 0;
  _mode = LEDSTRIP_MODE_STARTUP;
  _step = &StartupSequenceSteps[0];
  _strip = new Adafruit_NeoPixel(numLeds, pin, NEO_GRB | NEO_KHZ800);
  _strip->begin();
  _strip->show();
}

int LedStrip::numPixels()
{
  return _strip->numPixels();
}

void LedStrip::tick()
{
  int r, g, b;

  switch (_mode) {
    case LEDSTRIP_MODE_STARTUP:
      if (_step->millis < 0) {
        _mode = LEDSTRIP_MODE_CHASING;
        break;
      }

      if (millis() > (unsigned) _step->millis) {
        HSVtoRGB(_step->hue, 100, 100, &r, &g, &b);
        _strip->setPixelColor(_step->pixel, r, g, b);
        _step++;
      }

      break;

    case LEDSTRIP_MODE_ALL:
      HSVtoRGB(_h, _s, _v, &r, &g, &b);

      for (unsigned int i = 0; i < _strip->numPixels(); i++)
        _strip->setPixelColor(i, r, g, b);

      break;

    case LEDSTRIP_MODE_CHASING:
      _counter++;
      if (_counter > 50) {
        _position++;
        if (_position > 20)
          _position = 0;

        _counter = 0;
      }
      // fall through

    case LEDSTRIP_MODE_SINGLE:
      HSVtoRGB(_h, _s, _v, &r, &g, &b);

      for (unsigned int i = 0; i < _strip->numPixels(); i++)
        if (i == _position)
          _strip->setPixelColor(i, r, g, b);
        else
          _strip->setPixelColor(i, 0, 0, 0);

      break;

    case LEDSTRIP_MODE_RAINBOW:
      for (unsigned int i = 0; i < _strip->numPixels(); i++) {
        HSVtoRGB((_h + (i*20)) % 360, _s, _v, &r, &g, &b);
        _strip->setPixelColor(i, r, g, b);
      }

      _counter++;
      if (_counter > 1) {
        _counter = 0;
        _h++;
        _h %= 360;
      }

      break;

    default:
      break;
  }

  _strip->show();
}

void LedStrip::setHSV(int h, int s, int v)
{
  _h = h;
  _s = s;
  _v = v;
}

void LedStrip::increaseHue(int delta)
{
  _h += 360 + delta;
  _h %= 360;
}

void LedStrip::setMode(LedStrip::Mode mode)
{
  _mode = mode;
}

void LedStrip::setPosition(unsigned int position)
{
  _position = position;
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