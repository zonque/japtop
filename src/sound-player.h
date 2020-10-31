#pragma once

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

class SoundPlayer {
public:
  explicit SoundPlayer(int rxPin, int txPin);
  bool init();
  void play(int folder, int file);

private:
  SoftwareSerial *_serial;
  DFRobotDFPlayerMini *_player;
};