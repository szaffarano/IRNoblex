#include "ACNoblex.h"

// Redefine values from ir_NEC.cpp
// and add "SEPARATOR", a new value
// for my cheap AC "Noblex"
#define NEC_HDR_MARK      9024
#define NEC_HDR_SPACE     4512
#define NEC_BIT_MARK      564
#define NEC_ONE_SPACE     1692
#define NEC_ZERO_SPACE    564
#define NEC_SEPARATOR     19176
#define NEC_RPT_SPACE     2250

#define NEC_CARRIER_CLOCK 38

ACNoblex::ACNoblex(IRsend* irsend) : _irsend(irsend) {
  _mode = MODE_COOL;
  _power = OFF;
  _fan = FAN_AUTO;
  _temp = 25;
  _swing = OFF;
  _light = ON;
}

void ACNoblex::on() {
  _power = ON;
}
void ACNoblex::off() {
  _power = OFF;
}
bool ACNoblex::isOn() {
  return _power == ON;
}

void ACNoblex::setLight(bool light) {
  _light = light ? ON : OFF;
}

bool ACNoblex::getLight() {
  return _light;
}

void ACNoblex::setSwing(bool swing) {
  _swing = swing ? ON : OFF;
}

bool ACNoblex::getSwing() {
  return _swing;
}

void ACNoblex::setMode(uint8_t mode) {
  if (mode == MODE_AUTO || mode == MODE_HEAT || mode == MODE_DRY ||
      mode == MODE_COOL || mode == MODE_FAN) {
    _mode = mode;
  }
}

uint8_t ACNoblex::getMode() {
  return _mode;
}

void ACNoblex::setFanSpeed(uint8_t fan) {
  if ( fan ==  FAN_AUTO || fan == FAN_MED ||
       fan ==  FAN_MIN || fan ==  FAN_MAX  ) {
    _fan = fan;
  }
}

uint8_t ACNoblex::getFanSpeed() {
  return _fan;
}

void ACNoblex::setTemp(uint8_t new_temp) {
  if (new_temp >= 16 && new_temp <= 30) {
    _temp = new_temp;
  }
}

uint8_t ACNoblex::getTemp() {
  return _temp;
}

void ACNoblex::sendCommand() {
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

  _irsend->enableIROut(NEC_CARRIER_CLOCK);

  // header
  _irsend->mark(NEC_HDR_MARK);
  _irsend->space(NEC_HDR_SPACE);

  for (uint8_t j = 0; j < 8; j++) {
    for (int8_t i = 7; i >= 0; i--) {
      if (j == 4 && i == 4) {
        // separator delay
        _irsend->mark(NEC_BIT_MARK);
        _irsend->space(NEC_SEPARATOR);
      } else if (frame[j] & (1 << i)) {
        _irsend->mark(NEC_BIT_MARK);
        _irsend->space(NEC_ONE_SPACE);
      } else {
        _irsend->mark(NEC_BIT_MARK);
        _irsend->space(NEC_ZERO_SPACE);
      }
    }
  }

  // send crc
  for (int8_t i = 3; i >= 0; i--) {
    if (crc & (1 << i)) {
      _irsend->mark(NEC_BIT_MARK);
      _irsend->space(NEC_ONE_SPACE);
    } else {
      _irsend->mark(NEC_BIT_MARK);
      _irsend->space(NEC_ZERO_SPACE);
    }
  }

  // footer (LED off)
  _irsend->mark(NEC_BIT_MARK);
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