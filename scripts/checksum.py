#!/usr/bin/env python

casos = [
  "00010000100100000000011000001010010 s00000000000001000000000000001011",
  "10010000100100000000011000001010010s00000000000001000000000000000111",
  "01011000100100000000011000001010010s00000000000001000000000000001111",
  "11010000100100000000011000001010010s00000000000001000000000000000000",
  "00110000001100000000011000001010010s00000000000001000000000000000010",
  "00100000001100000000011000001010010s00000000000001000000000000000011",
  "10011000100100000000011000001010010s00000000000001000000000000000111",
  "10010100100100000000011000001010010s00000000000001000000000000000111",
  "10011100100100000000011000001010010s00000000000001000000000000000111",
  "10010000010100000000011000001010010s00000000000001000000000000001111",
  "01011000010100000000011000001010010s00000000000001000000000000000000",
  "11010000010100000000011000001010010s00000000000001000000000000001000",
  "00110000110100000000011000001010010s00000000000001000000000000001100",
  "00110000010100000000011000001010010s00000000000001000000000000000100",
  "00010010100100000000011000001010010s10000000000001000000000000001011",
  "10010010010100000000011000001010010s10000000000001000000000000001111",
  "01011010010100000000011000001010010s10000000000001000000000000000000",
  "11010010010100000000011000001010010s10000000000001000000000000001000",
  "00110010010100000000011000001010010s10000000000001000000000000000100",
  "00010010100100000000001000001010010s10000000000001000000000000001011",
  "10010010010100000000001000001010010s10000000000001000000000000001111",
  "01011010010100000000001000001010010s10000000000001000000000000000000",
  "11010010010100000000001000001010010s10000000000001000000000000001000",
  "00110010010100000000001000001010010s10000000000001000000000000000100"
]

for caso in casos:
	data = caso[0:64].replace('s', '1')
	crc = caso[64:]
	data_by_byte = [data[i*8:(i+1)*8] for i in range(0, 8)]
	data_by_byte_decimal_inv = [int(x[::-1], 2) for x in data_by_byte]

	crc_calculado = format(sum(data_by_byte_decimal_inv) & 0xF, '04b')[::-1]
	if crc == crc_calculado:
		print "OK!"
	else:
		print "FALLO"

