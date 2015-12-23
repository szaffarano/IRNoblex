#ifndef ACNoblex_h
#define ACNoblex_h

#include <IRremote.h>

#define CLOCK_IR  38

typedef enum {
  MODE_AUTO, MODE_COOL, MODE_DRY, MODE_FAN, MODE_HEAT  
} ac_mode;

typedef enum {
  FAN_AUTO, FAN_MIN, FAN_MED, FAN_MAX  
} fan_speed;

typedef enum {
  SWING_ONE, 
  SWING_TWO, 
  SWING_THREE, 
  SWING_FOUR, 
  SWING_FIVE, 
  SWING_SIX, 
  SWING_SEVEN, 
  SWING_EIGHT, 
  SWING_NINE  
} swing_pos;

class ACNoblex {
public:
    ACNoblex(IRsend* irsend);
    void on();
    void off();
    void turnOnLight();
    void turnOffLight();
    
    void setMode(ac_mode mode);
    void setFanSpeed(fan_speed);
    void setTemp(int temp);
    void setSwingPos(swing_pos pos);
    
    void send();

private:
  IRsend* _irsend;
  
  uint32_t frame1;
  uint32_t frame2;

  ac_mode _mode;
  fan_speed _fan;
  int _temp;
  swing_pos _pos;
  bool _light;
};

#endif // ACNoblex_h
