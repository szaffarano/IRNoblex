#include "ACNoblex.h"


#define CHECKSUMS_OFFSET  0
#define LIGHT_OFFSET      10
#define TEMP_OFFSET       21
#define FAN_SPEED_OFFSET  27
#define ON_OFF_OFFSET     28
#define MODE_OFFSET       30
#define SWING_OFFSET      30
#define ON                true
#define OFF               false

ACNoblex::ACNoblex(IRsend* irsend) : _irsend(irsend) {
  _mode   = MODE_COOL;
  _power  = OFF;
  _fan    = FAN_AUTO;
  _temp   = 25;
  _swing    = OFF;
  _light  = ON;
}

void ACNoblex::on() {
  _power = ON;
  send();
}

void ACNoblex::off() {
  _power = OFF;
  send();
}

void ACNoblex::turnOnLight() {
  _light = ON;
  send();
}

void ACNoblex::turnOffLight() {
  _light = OFF;
  send();
}

void ACNoblex::setMode(ac_mode mode) {
  _mode = mode;
  send();
}

void ACNoblex::setFanSpeed(fan_speed fan) {
  _fan = fan;
  send();
}

void ACNoblex::setTemp(int temp) {
  if (temp >= 16 && temp <= 30) {
    _temp = temp;
    send();
  }
}

void ACNoblex::turnOnSwing() {
  _swing = ON;
  send();
}

void ACNoblex::turnOffSwing() {
  _swing = OFF;
  send();
}


void ACNoblex::send() {
  uint8_t frame[8] = {
    0,    // mode[0:2], power[3], fan[4:5], swing[6], zero[7]
    0,    // temp[0:3], zero[4:7]
    0,    // ligth[5], zero[0:4], one[6], zero[7]
    0x0A, // valor fijo
    0x50, // fixed[0:3], separator[3], swing[4]
    0x00, // fixed[0:7]
    0x40, // fixed[0:7]
    0x00  // fixed[0:7]
  };
  uint8_t crc = 0;

  frame[0] |= (_mode  << 5);
  frame[0] |= (_power << 4);
  frame[0] |= (_fan   << 2);
  frame[0] |= (_swing << 1);
  frame[0] |= (0      << 0);

  frame[1] = reverse((_temp - 16) & 0xF);
  frame[2] |= (_light << 2) | (1 << 1);
  frame[4] |= (_swing << 3);

  for (uint8_t i = 0; i < 8; i++) {
    crc += reverse(frame[i]);
  }
  crc = reverse(crc & 0xF) >> 4;

  _irsend->enableIROut(CLOCK_IR);

  // header
  _irsend->mark(8660);
  _irsend->space(4300);

  for (uint8_t j = 0; j < 8; j++) {
    for (int8_t i = 7; i >= 0; i--) {
      if (j == 4 && i == 4) {
        // separator delay
        _irsend->mark(640);
        _irsend->space(19240);
      } else if (frame[j] & (1 << i)) {
        _irsend->mark(640);
        _irsend->space(1580);
      } else {
        _irsend->mark(640);
        _irsend->space(520);
      }
    }
  }

  // send crc
  for (int8_t i = 3; i >= 0; i--) {
    if (crc & (1 << i)) {
      _irsend->mark(640);
      _irsend->space(1580);
    } else {
      _irsend->mark(640);
      _irsend->space(520);
    }
  }

  // footer (LED off)
  _irsend->mark(640);
  _irsend->space(0);
}

uint8_t ACNoblex::reverse(uint8_t num) {
  uint8_t value = 0;
  for (int8_t i = 7; i >= 0; i--) {
    if (num & (1 << (7 - i))) {
      value |= (1 << i);
    }
  }
  return value;
}