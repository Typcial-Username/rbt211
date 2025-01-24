/*
 * blink.c
 *
 * Created: 1/23/2025 6:09:13 PM
 * Due: 1/29/25
 * Author : Levi Terry
 */ 

// Constants
#define F_CPU 16000000

// Built-in Libraries
#include <avr/io.h>
#include <util/delay.h>

// Main Loop
int main(void)
{
	DDRB |= 0b00100000; // OR 0x20 OR 32
    
    while (1) 
    {
		PORTB |= 0b00100000;
		_delay_ms(500);
		
		PORTB &= ~(0b00100000);
    }
}

