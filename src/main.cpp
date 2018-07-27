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

int pixelPos = 0;
bool blackButtonPressed = false;

static void blueButtonChanged(bool state)
{
  if (state) {
    ledStrip->setMode(LedStrip::LEDSTRIP_MODE_ALL);
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_BLUE, 100, 100);

    if (blackButtonPressed)
      player->play(3);
  }
}

static void redButtonChanged(bool state)
{
  if (state) {
    ledStrip->setMode(LedStrip::LEDSTRIP_MODE_ALL);
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_RED, 100, 100);

    if (blackButtonPressed)
      player->play(2);
  }
}

static void greenButtonChanged(bool state)
{
  if (state) {
    ledStrip->setMode(LedStrip::LEDSTRIP_MODE_ALL);
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_GREEN, 100, 100);

    if (blackButtonPressed)
      player->play(4);
  }
}

static void yellowButtonChanged(bool state)
{
  if (state) {
    ledStrip->setMode(LedStrip::LEDSTRIP_MODE_ALL);
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_YELLOW, 100, 100);

    if (blackButtonPressed)
      player->play(6);
  }
}

static void blackButtonChanged(bool state)
{
  ledStrip->setMode(LedStrip::LEDSTRIP_MODE_CHASING);
  blackButtonPressed = state;
}

static void leftRotaryButtonChanged(bool state)
{
  if (state)
    ledStrip->setMode(LedStrip::LEDSTRIP_MODE_RAINBOW);
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
  ledStrip->setMode(LedStrip::LEDSTRIP_MODE_CHASING);
  ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_CYAN, 100, 100);

  player = new SoundPlayer(8, 7);
  if (!player->init()) {
    ledStrip->setMode(LedStrip::LEDSTRIP_MODE_ALL);
    ledStrip->setHSV(LedStrip::LEDSTRIP_HUE_RED, 100, 100);
  }

  player->play(1);

  leftRotary = new RotaryEncoder(11, 12, leftRotaryEvent);
  rightRotary = new RotaryEncoder(9, 10, rightRotaryEvent);
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