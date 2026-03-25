/*
 * motorSpeedController.c
 *
 * Created: 3/20/2025 6:07:34 PM
 * Author : Phoen
  

#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIME 10000

static inline void initTimer0(void)
{
	TCCR0A |= 193;
	TIMSK0 |= 2;
	sei();
}

int main(void)
{
	// Pins 2, 3, and 5 OUTPUT
	// 4 + 8 + 32 = 44
	
	// Set pin modes
	DDRD |= 44;
	
	PORTD &= ~(4)
	PORTD |= 40; 
    
	// TCCR0A 7, 6, 0 = 128 + 64 + 1 = 
	// MGM00 1, MGM01 0
	
	
		     (255 - OCR0A) * 2
		 D = ------------------
			        512
	* /
    while (1) 
    {
		// 100%
		OCR0A = 0;
		_delay_ms(TIME);
		
		// 50%
		OCR0A = 128;
		_delay_ms(TIME);
		
		// 75%
		OCR0A = 64;
		_delay_ms(TIME);
		
		// 25% 
		
		_delay_ms(TIME);
		
		// Repeat!
    }
}
*/

//define global variables
volatile uint8_t brightnessA;
// speed 0 = 100% (255), 1 = 50% (128), 2 = 75% (191), 3 = 25% (64)
volatile uint8_t speed = 1;
volatile uint8_t ovrFloCt = 0;

//setup for timer
static inline void initTimer0(void){
	TCCR0B |= (1 << CS01 | 1 << CS00);
	TIMSK0 |= (1 << OCIE1B);
	TIMSK0 |= (1<< TOIE0);
	sei();
}

ISR(TIMER0_OVF_vect){
	if (speed == 0)
	{
		brightnessA = 255;
	}
	else if (speed == 1)
	{
		brightnessA = 128;
	}
	else if (speed == 2)
	{
		brightnessA = 191;
	}
	else if (speed = 3)
	{
		brightnessA = 64;
	}
	
	OCR0B = brightnessA;
	ovrFloCt++;
	// blink portion
	if(ovrFloCt == 9804){
		if (speed == 3){
			speed = 0;
		} else {
			speed++;
		}
			
		ovrFloCt = 0;
	}
}

ISR(TIMER0_COMPB_vect){
	PORTB &= ~(32);
}

int main(void)
{
	DDRB |= 32;
	initTimer0();
	brightnessA = 255;
	while (1)
	{
	}
}
