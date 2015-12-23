#include "ACNoblex.h"
#include "Utils.h"

#include <IRremote.h>

IRsend irsend;
ACNoblex ac(&irsend);
char r;
int temp;

void setup() {
  ac.off();
  Serial.begin(9600);

  while(!Serial);
  
  Serial.println("ACNoblex initialized!");
}

void loop() {
  Serial.print("ACNoblex> ");
  while(Serial.available() <= 0);
  r = Serial.read();
  switch(r) {
    case 't':
      Serial.print("enter new temperature value: ");
      temp = readNumber();
      ac.setTemp(temp);
      Serial.println();
      Serial.print("ACNoblez> new temperature value: ");
      Serial.print(temp, DEC);
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
    case 'h':
      Serial.println("Commands: ");
      Serial.println("\tt <temp>: sets new temperature value");
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
