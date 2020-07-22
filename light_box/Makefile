F_CPU = 8000000

MAIN=led.elf

all: $(MAIN)

$(MAIN): led.c
	avr-gcc -mmcu=attiny167 -DF_CPU=$(F_CPU) -O2 -o $(MAIN) led.c
	avr-objcopy -O ihex $(MAIN) led.hex
	avr-size led.elf

clean:
	rm -rf *.elf *.o *.hex

flash: all
	avrdude -p attiny167 -c stk600 -U flash:w:led.hex 
	#avrdude -p attiny85 -c avrispmkii -U flash:w:test.hex 
	#avrdude -c avrisp -p attiny85 -b 19200 -P /dev/ttyACM0 -U flash:w:test.hex 

