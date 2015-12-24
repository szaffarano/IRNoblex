#ifndef ACNoblex_h
#define ACNoblex_h

#include <IRremote.h>

#define CLOCK_IR  38

typedef enum {
  MODE_AUTO = 0b000,
  MODE_HEAT = 0b001,
  MODE_DRY  = 0b010,
  MODE_COOL = 0b100,
  MODE_FAN  = 0b110
} ac_mode;

typedef enum {
  FAN_AUTO  = 0b00,
  FAN_MED   = 0b01,
  FAN_MIN   = 0b10,
  FAN_MAX   = 0b11
} fan_speed;

class ACNoblex {
public:
  ACNoblex(IRsend* irsend);
  void on();
  void off();

  void turnOnLight();
  void turnOffLight();

  void turnOnSwing();
  void turnOffSwing();

  void setMode(ac_mode mode);
  void setFanSpeed(fan_speed);
  void setTemp(int temp);

  void send();

private:
  IRsend* _irsend;

  ac_mode   _mode;
  fan_speed _fan;
  int       _temp;
  bool      _swing;
  bool      _light;
  bool      _power;

  uint8_t reverse(uint8_t num);
};

#endif // ACNoblex_h
