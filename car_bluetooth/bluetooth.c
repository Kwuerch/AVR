#include <avr/io.h>
#include <util/delay.h>

#define GPIO_PIN_0 (0x01u)

void power_down(){
    // Clear sleep enable and sleep mode bits
    MCUCR &= ~(0x38);

    // Enable Power Down mode
    MCUCR |= (0x30);

    // Power Down
    asm volatile( "sleep" );
}



int main( void ){
    // GPIO_PIN_0 as High Output
    PORTB |= GPIO_PIN_0;
    DDRB |= GPIO_PIN_0;

    _delay_ms(5000);


    // Simulate Button Press
    PORTB &= ~(GPIO_PIN_0);
    _delay_ms(200);

    // Put GPIO_PIN_0 in Hi-Z mode
    DDRB &= ~(GPIO_PIN_0);

    while( 1 ){
        power_down();
    }
}
