#include "sound-player.h"

SoundPlayer::SoundPlayer(int rxPin, int txPin) {
  _serial = new SoftwareSerial(rxPin, txPin);
  _serial->begin(9600);
  _player = new DFRobotDFPlayerMini();
}

bool SoundPlayer::init() {
  if (_player->begin(*_serial)) {
    _player->setTimeOut(500);
    _player->EQ(DFPLAYER_EQ_NORMAL);
    _player->outputDevice(DFPLAYER_DEVICE_SD);
    _player->volume(5);
    return true;
  } else {
    delete _player;
    _player = NULL;
    return false;
  }
}

void SoundPlayer::play(int folder, int file) {
  if (_player) {
    _player->volume(22);
    _player->stop();
    _player->playFolder(folder, file);
  }
}