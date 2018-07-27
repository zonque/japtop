#include <Arduino.h>
#include "analog-button.h"
#include "led-strip.h"
#include "rotary-encoder.h"
#include "sound-player.h"

SoundPlayer *player;
LedStrip *ledStrip;
RotaryEncoder *leftRotary;
RotaryEncoder *rightRotary;
AnalogButton *blueButton;
AnalogButton *redButton;
AnalogButton *greenButton;
AnalogButton *yellowButton;
AnalogButton *blackButton;
AnalogButton *leftRotaryButton;
AnalogButton *rightRotaryButton;

int pixelPos = 5;

static void blueButtonChanged(bool state)
{
  if (state)
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_BLUE, 100, 100);
}

static void redButtonChanged(bool state)
{
  if (state)
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_RED, 100, 100);
}

static void greenButtonChanged(bool state)
{
  if (state)
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_GREEN, 100, 100);
}

static void yellowButtonChanged(bool state)
{
  if (state)
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_YELLOW, 100, 100);
}

static void blackButtonChanged(bool state)
{
  ledStrip->setMode(LedStrip::LEDSTRIP_MODE_ALL);

  if (state)
    ledStrip->setHSV(0, 0, 0);
}

static void leftRotaryButtonChanged(bool state)
{
  if (state)
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_CYAN, 100, 100);
}

static void rightRotaryButtonChanged(bool state)
{
  if (state)
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_MAGENTA, 100, 100);
}

static void leftRotaryEvent(bool cw)
{
  if (cw)
    pixelPos++;
  else
    pixelPos--;

  if (pixelPos >= ledStrip->numPixels())
    pixelPos = ledStrip->numPixels() - 1;

  if (pixelPos < 0)
    pixelPos = 0;

  ledStrip->setMode(LedStrip::LEDSTRIP_MODE_SINGLE);
  ledStrip->setPosition(pixelPos);
}

static void rightRotaryEvent(bool cw)
{
  if (cw)
    ledStrip->increaseHue(1);
  else
    ledStrip->increaseHue(-1);
}

void setup() {
  ledStrip = new LedStrip(5, 10);
  ledStrip->setPosition(pixelPos);

  player = new SoundPlayer(0, 0); // XXX
  leftRotary = new RotaryEncoder(9, 10, leftRotaryEvent);
  rightRotary = new RotaryEncoder(11, 12, rightRotaryEvent);
  blueButton = new AnalogButton(A0, blueButtonChanged);
  redButton = new AnalogButton(A1, redButtonChanged);
  greenButton = new AnalogButton(A2, greenButtonChanged);
  yellowButton = new AnalogButton(A3, yellowButtonChanged);
  blackButton = new AnalogButton(A4, blackButtonChanged);
  leftRotaryButton = new AnalogButton(A5, leftRotaryButtonChanged);
  rightRotaryButton = new AnalogButton(A6, rightRotaryButtonChanged);
}

void loop() {
  ledStrip->tick();
  leftRotary->tick();
  rightRotary->tick();
  blueButton->tick();
  redButton->tick();
  greenButton->tick();
  yellowButton->tick();
  blackButton->tick();
  leftRotaryButton->tick();
  rightRotaryButton->tick();
}