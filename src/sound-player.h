#pragma once

#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

class SoundPlayer {
public:
  explicit SoundPlayer(int rxPin, int txPin);
  void play(int id);

private:
  SoftwareSerial *_serial;
  DFRobotDFPlayerMini *_player;
};