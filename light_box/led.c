#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define NUM_LEDS 3
#define NUM_COLORS 3
#define LED1 0
#define LED2 1
#define LED3 2
#define RED 0
#define GRN 1
#define BLU 2

#define GPIO_PIN0 ((uint8_t)0x01u)
#define GPIO_PIN1 ((uint8_t)0x02u)
#define GPIO_PIN2 ((uint8_t)0x04u)
#define GPIO_PIN3 ((uint8_t)0x08u)
#define GPIO_PIN4 ((uint8_t)0x10u)
#define GPIO_PIN5 ((uint8_t)0x20u)
#define GPIO_PIN6 ((uint8_t)0x40u)
#define GPIO_PIN7 ((uint8_t)0x80u)


// Could potentially use OC0A for the timer
/**
 *
    PA2 LED3B - 0A
    PB5 LED3G - 1BW
    PB6 LED3R - 1AX
**/

uint8_t count[3] = {0, 170, 85};
int count_inc[3] = {1,   1,  -1};


int main(void){
    /** Initialize As Outputs **/
    DDRA = _BV(2);
    DDRB = _BV(5) | _BV(6);
    PORTA = 0x00;
    PORTB = 0x00;

    // Clear on compare match, set at bottom 0x80
    // Fast PWM 0x03
    TCCR0A = 0x83u;

    // /64 Prescaler 0x04
    TCCR0B = 0x04u;


    // Clear on compare match for Channel A and B, set at bottom 0x80
    // Fast PWM 8-bit 0x01
    TCCR1A = 0xA1u;

    // /64 Prescaler 0x03
    // Fast PWM 8-bit 0x08
    TCCR1B = 0x0Bu;

    // Disable Force Output Compare
    TCCR1C = 0x00u;

    // Enable Output Compare for B(X,W,V,U) and A(X,W,V,U)
    TCCR1D = 0xFFu;


    while(1){
        // Red
        OCR1AH = 0x00;
        OCR1AL = count[0];

        //Green
        OCR1BH = 0x00;
        OCR1BL = count[1];

        // Blue
        OCR0A = 255;

        for (int i = 0; i < 3; i++){
            if (count[i] == 255) {
                count_inc[i] = -1;
            } else if (count[i] == 0) {
                count_inc[i] = +1;
            }

            count[i] += count_inc[i];
        }

        _delay_ms(100);
    }
}
