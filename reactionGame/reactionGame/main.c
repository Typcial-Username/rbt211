/*
 * reactionGame.c
 *
 * Created: 4/25/2025 11:06:40 AM
 * Author : Phoen
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#define CPU_FREQ 16000000L

// How long to stay on the current led (ms)
volatile int delay = 250;
volatile char direction = 'L';
volatile uint8_t curPin = 0;

uint16_t ms_to_ticks(uint16_t ms)
{
	uint16_t ticks = ((uint32_t)(ms * (CPU_FREQ / 1000))) / 1024;

	if (ticks > 65535L)
	{
		ticks = 65535L;
	} else if (ticks < 0)
	{
		ticks = 0;
	}

	return (uint16_t)(65535 - ticks);
}

static inline void initPCInterrupt0(void)
{
	// Enable PCINT0
	PCICR |= (1 << PCIE1);
	// Trigger when button changes
	PCMSK1 |= (1 << PCINT8);
	// Set interrupt enable bit
	sei();
}

// Configure Timer 2 for PWM
static inline void initTimer2(void)
{
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS21);
} 

static inline void initTimer1(void)
{
	// Set start count
	TCNT1 = ms_to_ticks(delay);
	
	// Set prescaler
	// Normal mode
	TCCR1A = 0;
	// 1024 prescaler
	TCCR1B = (1 << CS12) | (1 << CS10);
	
	TIMSK1 = 1;
}

ISR(PCINT1_vect)
{
	// Check if button is pressed
	if (curPin == 3)
	{
		// Enable PWM on PD3
		TCCR2A |= (1 << COM2B1);
		
		// Set to 50% duty cycle
		OCR2B = 128;

		// Decrease delay
		delay -= delay * 0.1;
	} else
	{
		// Disable PWM on PD3
		TCCR2A &= ~((1 << COM2B1) | (1 << COM2B0));
		// Turn off LED
		//PORTB &= ~(1 << PB3);
		
	}
}

ISR(TIMER1_OVF_vect)
{	
	if (direction == 'R')
	{
		// Enable current LED
		PORTD |= (1 << curPin);
		
		if (curPin > 0 && curPin < 6)
		{
			// Turn off last LED
			PORTD &= ~(1 << (curPin + 1));
		}
		
		// Reverse Direction
		if (curPin == 0)
		{
			direction = 'L';
			curPin = 1;
		} else
		{
			curPin--;
		}
		
	} else if (direction == 'L')
	{
		// Enable current LED
		PORTD |= (1 << curPin);
		
		if (curPin > 0 && curPin < 6)
		{
			// Turn off last LED
			PORTD &= ~(1 << (curPin - 1));
		}
		
		// Reverse Direction
		if (curPin == 6)
		{
			direction = 'R';
			curPin = 5;
		} else
		{
			curPin++;
		}
	}

	TCNT1 = ms_to_ticks(delay);
}

int main(void)
{
	// Set all of PORTD to output
	DDRD |= 0xff;
	DDRC &= ~(1 << PC0);
	PORTC |= (1 << PC0);
	// Configure PB0 for pullup resistor
	PORTB |= (1 << PB0);

	initTimer2();
	initPCInterrupt0();
	initTimer1();
	
	while (1) {}
}

