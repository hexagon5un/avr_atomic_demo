/* 
 * Code that demos the non-atomicity of 16-bit assignment on AVRs
 * shared_variable is updated in an ISR
 * copied over to local_variable
 * but the copying can get interrupted, resulting in a bad value
 * and blinking the LED
 *
 * Code by Elliot Williams, put into public domain.  
 * There ain't much to it. Enjoy.
 *
 * */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#define LED      PB5        /* pin 13 on Arduino, the LED */
#define LED_DDR  DDRB
#define LED_PORT PORTB

volatile uint16_t shared_variable; 

/* Simply toggle between two 16-bit values */
ISR(TIMER0_OVF_vect){
	if (shared_variable == 0x2222){
		shared_variable  = 0x7777;
	} else {
		shared_variable  = 0x2222;
	}
}

void init_timer0(void){
	TCCR0B |= (1 << CS02);  /* divide clock by 1/256     */
	TIMSK0 |= (1 << TOIE0); /* enable overflow interrupt */
	sei();                  /* enable interrupts         */
}

int main(void) 
{
	uint16_t local_variable = 0;
	LED_DDR |= (1 << LED);  /* set up output on LED      */
	init_timer0();

	while (1) {
		/* Make a local copy of the variable 
		 * if protected by atomic block, should not blink LED */
		/* ATOMIC_BLOCK(ATOMIC_RESTORESTATE) */
		{
			local_variable = shared_variable;
		}

		/* Test for Franken-values */
		if ((local_variable == 0x7722) | (local_variable == 0x2277)){
			LED_PORT |= (1 << LED);
			_delay_ms(100);
		} else {
			LED_PORT &= ~(1 << LED);
		}

		/* Optional delay. 
		 * Change to see different blinking patterns as the 
		 * time it takes to run the mainloop and the timer come
		 * in and out of phase. */
		_delay_us(3);

	}         /* End event loop             */
	return 0; /* This line is never reached */
}
