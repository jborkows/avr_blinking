MCU=atmega32
F_CPU=16000000UL
CC=avr-gcc
CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
OBJCOPY=avr-objcopy
AVRDUDE=avrdude
PORT=/dev/ttyUSB0
PROGRAMMER=usbasp

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

main.elf: main.c
	$(CC) $(CFLAGS) -o $@ $<

flash: main.hex
	$(AVRDUDE) -c $(PROGRAMMER) -b 57600 -p $(MCU) -P $(PORT) -U flash:w:$<:i

clean:
	@rm -f *.elf *.hex compile_commands.json

