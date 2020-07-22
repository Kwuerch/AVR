#include <avr/io.h>
#include <util/delay.h>



/*******************************************PIN_CONFIG*********************************************************
 * LCD-   |         |         |         |         |         |         |         |         |         |         *
 *   EN   |    RS   |   R/W   |   DB7   |   DB6   |   DB5   |   DB4   |   DB3   |   DB2   |   DB1   |   DB0   *
 *------------------------------------------------------------------------------------------------------------*
 * MCU-   |         |         |         |         |         |         |         |         |                   *
 *   PB2  |   PB1   |   PB0   |   PD7   |   PD6   |   PD5   |   PD4   |   PD3   |   PD2   |   PD1   |   PD0   *
 *************************************************************************************************************/

#define DATA_PORT_MASK 0xFF
#define DATA_PORT_DREG DDRD
#define DATA_PORT PORTD

#define CTRL_PORT_MASK 0x07
#define CTRL_PORT_DREG DDRB
#define CTRL_PORT PORTB

#define BUSY_MASK 0x80
#define EN_MASK   0x04

void init_data_ports(){
    DATA_PORT_DREG |= DATA_PORT_MASK;
    CTRL_PORT_DREG |= CTRL_PORT_MASK;

    DATA_PORT &= ~DATA_PORT_MASK;
    CTRL_PORT &= ~CTRL_PORT_MASK;
    _delay_ms(15);
}


void enable_switch(){
    CTRL_PORT |= EN_MASK;
    asm volatile ("nop");
    asm volatile ("nop");
    CTRL_PORT &= ~EN_MASK;
}

void check_busy(){
    DATA_PORT_DREG &= ~DATA_PORT_MASK;

    CTRL_PORT &= ~CTRL_PORT_MASK;
    CTRL_PORT |= 0x01; // RS-LOW R/W-High

    while( BUSY_MASK & DATA_PORT ){
        enable_switch();
    }

    DATA_PORT_DREG |= DATA_PORT_MASK;
}


void run_command( uint8_t data ){

    check_busy();
    DATA_PORT = data;

    CTRL_PORT &= ~CTRL_PORT_MASK;

    enable_switch();

    DATA_PORT &= ~DATA_PORT_MASK;
}

void write_char( uint8_t c ){
    check_busy();

    CTRL_PORT &= ~CTRL_PORT_MASK;
    CTRL_PORT = 0x02;

    DATA_PORT = c;

    enable_switch();
    DATA_PORT = 0;
    _delay_us( 40 );
}

void write_string( char* str ){
    while( *str != '\0' ){
        write_char( *str );
        str++;
    }
}

void init_lcd(){
    run_command( 0x01 );
    _delay_ms(2);

    run_command( 0x38 );
    _delay_us(50);

    run_command( 0x0C );
    _delay_us(50);
}

void mov_curs(int line){
    if( !line ){
        run_command( 0x80 );
    }else{
        run_command( 0xC0 );
    }

    _delay_us( 50 );
}

void shift_up(){
    int to_shift = 8;
    while( to_shift ){
        run_command( 0x18 );
        _delay_us( 50 );
        to_shift--;
    }
}

void clear_screen(){
    run_command( 0x01 );
    _delay_ms( 2 );
}

void power_down(){
    clear_screen();

    // Set Pins to Input
    DATA_PORT_DREG = 0x00;

    // Clear Sleep Register Bits
    SMCR &= ~0x07;

    // Enable Sleep Power Down
    SMCR |= 0x05;

    // Power Down
    asm volatile( "sleep" );

    // Renable Data Pins as Output
    DATA_PORT_DREG = 0xFF;
}



int main( void ){
    init_data_ports();
    init_lcd();

    while( 1 ){
        write_string( "Hello Kyle!" );

        mov_curs( 1 );

        write_string( "How are you?" );

        _delay_ms( 3000 );


        clear_screen();


        write_string( "Good to hear!" );
        _delay_ms( 3000 );

        clear_screen();

        write_string( "Going to Sleep!" );
        _delay_ms( 3000 );

        power_down();
        // Must reenalbe Data pins after this
    }
}
