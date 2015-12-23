#!/usr/bin/env python

import re
import os
import sys

HEADER1 = 8600
HEADER2 = 4400
LOW = 600
HIGH = 1500
SEPARATOR = 19200
PAUSE = 38700
MARGIN = 150
SEP = ','

def is_type(field, value):
  return True if abs(value-field) < MARGIN else False

def translate(value1, value2):
  value1 = int(value1)
  value2 = int(value2)
  if is_type(HEADER1, value1) and is_type(HEADER2, value2):
    return 'h'
  elif is_type(LOW, value1) and is_type(LOW, value2):
    return '0'
  elif is_type(LOW, value1) and is_type(HIGH, value2):
    return '1'
  elif is_type(LOW, value1) and is_type(SEPARATOR, value2):
    return 's'
  elif is_type(LOW, value1) and is_type(PAUSE, value2):
    return 'p'
  return 'e[%s:%s:%s:%s]' % (value1,value2, is_type(LOW, value1), is_type(PAUSE, value2))

if __name__ == '__main__':
  if len(sys.argv) != 2:
    print("Modo de uso: %s <input>" % sys.argv[0])
    sys.exit(1)

  file_name = sys.argv[1]
  if not os.path.isfile(file_name):
    print("%s: no existe el archivo" % file_name)

  commands = []
  for line in open(file_name).readlines():
    i = 1
    codes = line.split(SEP)
    decoded = []
    
    while i < len(codes)-1:
      v1 = codes[i]
      v2 = codes[i+1]
      decoded.append(translate(v1, v2))
      i += 2
    commands.append(decoded)


print "Se parsearon %d comandos" % len(commands)
for c in commands:
  print "".join(c)
