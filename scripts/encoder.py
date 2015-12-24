#!/usr/bin/env python

import sys

def usage():
	print """Modo de uso: %s <modo> <power> <fan> <swing> <temp> <light>
	
	modo: [auto|cool|dry|fan|heat
	power: [1|0]
	fan: [min|med|max|auto]
	swing: [1|0]
	temp: <16..30>
	light: [1|0]""" % sys.argv[0]

def reverse(num):
	value = 0
	for i in list(reversed(range(8))):
		if (num & (1 << 7-i)):
			value |= (1 << i)
	return value 

def safe_set(value, values):
	if value not in values:
		print "%s: valor invalido" % value
		usage()
		sys.exit(2)
	else:
		return value

modes = {
	'auto': 0b000,
	'cool': 0b100,
	'dry': 0b010,
	'fan': 0b110,
	'heat': 0b001
}

fans = {
	'min': 0b10,
	'med': 0b01,
	'max': 0b11,
	'auto': 0b00
}

if len(sys.argv) != 7:
	usage()
	sys.exit(1)

mode = safe_set(sys.argv[1], modes)
power = int(safe_set(sys.argv[2], ['0', '1']))
fan = safe_set(sys.argv[3], fans)
swing = int(safe_set(sys.argv[4], ['0', '1']))
temp = int(safe_set(sys.argv[5], [str(x) for x in range(16, 31)]))
light = int(safe_set(sys.argv[6], ['0', '1']))


# Ejemplo:
# 	MODE:  	auto	
# 	POWER: 	on	
# 	FAN:	auto	
# 	SWING: 	off
# 	TEMP: 	N/A	
# 	LIGHT: 	on
# 0001000010010000000001100000101001010000000000000100000000000000 1011
frame = [
	0,    # mode[0:2], power[3], fan[4:5], swing[6], zero[7]
	0,    # temp[0:3], zero[4:7]
	0,    # ligth[5], zero[0:4], one[6], zero[7]
	0x0A, # valor fijo
	0x50, # fixed[0:3], separator[3], swing[4]
	0x00, # fixed[0:7]
	0x40, # fixed[0:7]
	0x00  # fixed[0:7]
]

frame[0] |= (modes[mode] 	<< 5)
frame[0] |= (power 			<< 4)
frame[0] |= (fans[fan] 		<< 2)
frame[0] |= (swing 			<< 1)
frame[0] |= (0 				<< 0)

frame[1] = reverse((temp-16) & 0xF)

frame[2] |= (light << 2) | (1 << 1)

frame[4] |= (swing << 3)

command = "".join([format(n, '08b') for n in frame])
crc = format(reverse(sum([reverse(b) for b in frame]) & 0xF) >> 4, '04b')

print ("Parametros:\n\tmode: <%s>\n\tpower: <%s>\n\tfan: <%s>\n\tswing: <%s>\n\ttemp: <%s>\n\tlight: <%s>" %
	(mode, power, fan, swing, temp, light))
print "Comando: %s" % command
print "CRC: %s" % crc