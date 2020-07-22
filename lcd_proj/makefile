F_CPU = 8000000

MAIN=lcd.elf

all: $(MAIN)

$(MAIN): lcd.c
	avr-gcc -mmcu=attiny88 -DF_CPU=$(F_CPU) -O2 -o $(MAIN) lcd.c
	avr-objcopy -O ihex $(MAIN) lcd.hex
	avr-size lcd.elf

clean:
	rm -rf *.elf *.o *.hex

flash: all
	avrdude -p attiny88 -c stk600 -U flash:w:lcd.hex 

