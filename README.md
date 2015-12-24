## Implementación IR para controlar A/C Noblex modelo NOS32C13X

Es una versión preliminar, muy primitiva, deducida por ingeniería inversa analizando en osciloscopio y con un receiver TSOP48 en un Arduino.  En un raspberry pi si bien se pueden capturar tramas, los tiempos son muy dispares dada la latencia del sistema operativo.  No obtuve buenos resultados.

## Protocolo

### Nomenclatura

- Cada bit que se envía está conformado por un primer pulso (mark) que es igual siempre, más un segundo pulso (space) que varía según se quiera enviar un uno o un cero.
- Los valores de los pulsos son:
    + mark: 540 usec
    + space (cero): 520 usec
    + space (uno): 1580 usec
- Cada secuencia de bytes que se envía está conformada por:
    + un header, compuesto por un pulso mark de 8660 usec y un pulso space de 4300 usec.
    + 64 bits de información, con sucesivos marks y spaces según lo dicho antes.
    + Un bit "separador" que consiste un pulso mark de 640 y un pulso space de 19240.  Este separador se ubica en la posición 36 de la trama de 64 bits.
    + Cuatro bits que hacen las veces de checksum, calculado como la suma big endian de los 8 bytes precedentes interpretados también como big endian.

        +---+--------------+---+------------+-----+
        | H |   35 bits    |sep|   28 bits  | crc |
        +---+--------------+---+------------+-----+ 

### Modo

El código se codifica con 3 bits en la posición [0:3] de la trama.  El A/C maneja 5 modos distintos:

- Auto: 000
- Cool: 100
- Dry: 010
- Fan: 110
- Heat: 001

### Power

Es el 4 bit de la trama:

- On: 1
- Off: 0

### Fan

Se maneja con 2 bits en la posición [4:5] de la trama:

- Mínimo: 10
- Medio: 01
- Máximo: 11
- Auto: 00

### Swing

Se maneja con 2 bits.  Pueden ser más, falta analizar los distintos casos ya que por lo visto depende el modo en el que esté para que haya valores válidos.  En principió observé que se maneja con 2 bits **separados**, frame[5] y frame[36]

- On: ambos en uno.
- Off: ambos en cero.

### Temperatura

Manejada por 4 bits en la posición [8:11] del frame.  Se calcula como: TEMPERATURA - 16 y se expresa como big endian.

### Luz

Se representa con un bit en frame[21]. 

- On: 1
- Off: 0

### Bits invariantes / sin analizar

Solo una parte de los bits varían según la funcionalidad interpretada, el resto se mantiene invariante:

frame = [
    0b00110010, # modo, power, fan, swing
    0b01010000, # temperatura
    0b00000010, # light
    0b00001010, # siempre igual 
    0b010s1000, # la "s" es el separador, swing
    0b00000000, # siempre igual
    0b01000000, # siempre igual
    0b00000000  # siempre igual
]

