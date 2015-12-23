#include "ACNoblex.h"

static uint8_t checksums[] = {10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15, 0, 8, 4, 12};
static uint8_t temperatures[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7};

ACNoblex::ACNoblex(IRsend* irsend) : _irsend(irsend) {
  _mode = MODE_AUTO;
  _fan = FAN_AUTO;
  _temp = 24;
  _pos = SWING_ONE;
  _light = true;
  frame1 = 2418017802;
  frame2 = 1082392585;
}


void ACNoblex::on() {
  frame1 |= (1L << 28);
  frame2 ^= (1 << 0);
  send();
}

void ACNoblex::off() {
  frame1 &= ~(1L << 28);
  frame2 ^= (1 << 0);
  send();
}

void ACNoblex::turnOnLight() {
  frame1 |= (1 << 10);
  send();
}

void ACNoblex::turnOffLight() {
  frame1 &= ~(1 << 10);
  send();
}
    
void ACNoblex::setMode(ac_mode mode) {
  send();
}

void ACNoblex::setFanSpeed(fan_speed) {
  send();
}

void ACNoblex::setTemp(int temp) {
  send();
}

void ACNoblex::setSwingPos(swing_pos pos) {
  send();
}


void ACNoblex::send() {  
  _irsend->enableIROut(CLOCK_IR);

  // header
  _irsend->mark(8660);
  _irsend->space(4300);

  // first 32 bits
  for (int i = 31; i >= 0; i--) {
    if (frame1 & (1L << i)) {
      _irsend->mark(640);
      _irsend->space(1580);     
    } else {
      _irsend->mark(640);
      _irsend->space(520);    
    }
  }

  // last 3 bits for the first part, always: 010
  _irsend->mark(640);
  _irsend->space(520);
  _irsend->mark(640);
  _irsend->space(1580);
  _irsend->mark(640);
  _irsend->space(520);

  // separator delay
  _irsend->mark(640);
  _irsend->space(19240);
  
  // last 32 bits (included checksum)
  for (int i = 31; i >= 0; i--) {
    if (frame2 & (1L << i)) {
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

