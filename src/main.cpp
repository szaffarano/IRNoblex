#include "ACNoblex.h"
#include "Utils.h"

#include <IRremote.h>

IRsend irsend;
ACNoblex ac(&irsend);
char r;
int temp;

void setup() {
  //ac.off();
  Serial.begin(9600);

  while (!Serial);

  Serial.println("ACNoblex initialized!");
}

void loop() {
  Serial.print("ACNoblex> ");
  while (Serial.available() <= 0);
  r = Serial.read();
  switch (r) {
  case 't':
    Serial.print("enter new temperature value: ");
    temp = readNumber();
    ac.setTemp(temp);
    Serial.println();
    Serial.print("ACNoblez> new temperature value: ");
    Serial.print(temp, DEC);
    break;
  case 'c':
    Serial.print("AC state sent!");
    ac.send();
    break;
  case '1':
    Serial.print("AC turned on!");
    ac.on();
    break;
  case '0':
    Serial.print("AC turned off!");
    ac.off();
    break;
  case 'l':
    Serial.print("light turned off!");
    ac.turnOffLight();
    break;
  case 'L':
    Serial.print("light turned on!");
    ac.turnOnLight();
    break;  
  case 'S':
    Serial.print("swing turned on!");
    ac.turnOnSwing();
    break;
  case 's':
    Serial.print("swing turned off!");
    ac.turnOffSwing();
    break;
  case 'm':
    Serial.print("Select mode [A/b/c/d] ");
    while (r != 13 && (r < 'a' || r > 'e')) {
      r = Serial.read();
    }
    switch(r) {
      case 'm':
      case 'a':
        ac.setMode(MODE_AUTO);
        Serial.println(" AUTO");
        break;
      case 'b':
        ac.setMode(MODE_COOL);
        Serial.println(" COOL");
        break;
      case 'c':
        ac.setMode(MODE_DRY);
        Serial.println(" DRY");
        break;
      case 'd':
        ac.setMode(MODE_FAN);
        Serial.println(" FAN");
        break;
      case 'e':
        ac.setMode(MODE_HEAT);
        Serial.println(" HEAT");
        break;
    }
    break;
  case 'h':
    Serial.println("Commands: ");
    Serial.println("\tt <temp>: sets new temperature value");
    Serial.println("\tb <mode>: sets new mode operation");
    Serial.println("\t1: turns AC on");
    Serial.println("\t0: turns AC off");
    Serial.println("\tL: turns light on");
    Serial.println("\tl: turns light off");
    break;
  default:
    Serial.print(" command invalid, press 'h' for help!");
  }
  Serial.println();
}
