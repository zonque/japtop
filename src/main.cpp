  #include <Arduino.h>
#include "analog-button.h"
#include "led-strip.h"
#include "rotary-encoder.h"
#include "sound-player.h"

SoundPlayer *soundPlayer;
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
enum LedStrip::Mode previousLedStripMode;

static void blueButtonChanged(bool state) {
  ledStrip->reportButtonState(LedStrip::BLOCKCOLOR_BLUE, state);
}

static void redButtonChanged(bool state) {
  ledStrip->reportButtonState(LedStrip::BLOCKCOLOR_RED, state);
}

static void greenButtonChanged(bool state) {
  ledStrip->reportButtonState(LedStrip::BLOCKCOLOR_GREEN, state);
}

static void yellowButtonChanged(bool state) {
  ledStrip->reportButtonState(LedStrip::BLOCKCOLOR_YELLOW, state);
}

static void blackButtonChanged(bool state) {
  if (state) {
    ledStrip->reset();
    soundPlayer->play(2, 4);
  }
}

static void leftRotaryButtonChanged(bool state) {
}

static void rightRotaryButtonChanged(bool state) {
}

static void leftRotaryEvent(bool cw) {
  if (cw)
    ledStrip->digitUp();
  else
    ledStrip->digitDown();
}

static void rightRotaryEvent(bool cw) {
}

void setup() {
  randomSeed(analogRead(0));

  soundPlayer = new SoundPlayer(8, 7);
  soundPlayer->init();

  ledStrip = new LedStrip(5, 10, soundPlayer);

  soundPlayer->play(2, 1);

  leftRotary = new RotaryEncoder(11, 12, leftRotaryEvent);
  rightRotary = new RotaryEncoder(9, 10, rightRotaryEvent);
  blueButton = new AnalogButton(A0, blueButtonChanged);
  redButton = new AnalogButton(A1, redButtonChanged);
  greenButton = new AnalogButton(A2, greenButtonChanged);
  yellowButton = new AnalogButton(A3, yellowButtonChanged);
  blackButton = new AnalogButton(A4, blackButtonChanged);
  leftRotaryButton = new AnalogButton(A5, leftRotaryButtonChanged);
  rightRotaryButton = new AnalogButton(A6, rightRotaryButtonChanged);

  Serial.begin(115200);
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