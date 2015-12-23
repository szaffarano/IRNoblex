#ifndef UTILS_H
#define UTILS_H

int readNumber() {
  int n = 0;
  char c = 0;
  while(1) {
    if (Serial.available() > 0) {
      c = Serial.read() - 48;
      if (c >= 0 && c <= 9) {
        Serial.print(c, DEC);
        n = c + n * 10;
      } else if (c+48 == 13) {
        break;
      }
    }
  }
  return n;
}

#endif // UTILS_H
