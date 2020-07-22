F_CPU = 1000000

MAIN=bluetooth.elf

all: $(MAIN)

$(MAIN): bluetooth.c
	avr-gcc -mmcu=attiny85 -DF_CPU=$(F_CPU) -O2 -o $(MAIN) bluetooth.c
	avr-objcopy -O ihex $(MAIN) bluetooth.hex
	avr-size bluetooth.hex

clean:
	rm -rf *.elf *.o *.hex

flash: all
	avrdude -p attiny85 -c stk600 -U flash:w:bluetooth.hex 
