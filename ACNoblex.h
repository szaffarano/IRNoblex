#ifndef ACNoblex_h
#define ACNoblex_h

#include <IRremote.h>

#define ON                true
#define OFF               false

#define MODE_AUTO 0b000
#define MODE_HEAT 0b001
#define MODE_DRY  0b010
#define MODE_COOL 0b100
#define MODE_FAN  0b110

#define FAN_AUTO  0b00
#define FAN_MED   0b01
#define FAN_MIN   0b10
#define FAN_MAX   0b11

class ACNoblex {
public:
  ACNoblex(IRsend* irsend);

  void on();
  void off();
  bool isOn();

  void setLight(bool);
  bool getLight();

  void setSwing(bool);
  bool getSwing();

  void setMode(uint8_t);
  uint8_t getMode();

  void setFanSpeed(uint8_t);
  uint8_t getFanSpeed();

  void setTemp(uint8_t temp);
  uint8_t getTemp();

  void sendCommand();
private:
  IRsend* _irsend;
  uint8_t reverse(uint8_t num);

  uint8_t _mode;
  uint8_t _fan;
  int _temp;
  bool _swing;
  bool _light;
  bool _power;
};

#endif // ACNoblex_h
