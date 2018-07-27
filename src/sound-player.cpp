#include "sound-player.h"

SoundPlayer::SoundPlayer(int rxPin, int txPin)
{
  _serial = new SoftwareSerial(rxPin, txPin);
  _serial->begin(9600);

  _player = new DFRobotDFPlayerMini();
  if (_player->begin(*_serial)) {
    _player->EQ(DFPLAYER_EQ_NORMAL);
    _player->outputDevice(DFPLAYER_DEVICE_SD);
  } else {
    delete _player;
    _player = NULL;
  }
}

void SoundPlayer::play(int id)
{
  _player->play(id);
}