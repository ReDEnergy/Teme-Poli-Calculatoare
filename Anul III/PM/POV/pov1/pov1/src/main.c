/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>

char led[2][16] = {
	{0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 6, 7, 1, 2},
	{3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0}
};

unsigned char volatile* ports[] = {&PORTA, &PORTB, &PORTC, &PORTD};
unsigned char pins[] = {0, 1, 2, 3, 4};

char F[16][24] = {
	"111111111111111111111111",
	"111111111111111111111111",
	"111111111111111111111111",
	"111111111111111111111111",
	"111111100000000000000000",
	"111111100000000000000000",
	"111111100000000000000000",
	"111111111111111111000000",
	"111111111111111111000000",
	"111111111111111111000000",
	"111111100000000000000000",
	"111111100000000000000000",
	"111111100000000000000000",
	"111111100000000000000000",
	"111111100000000000000000",
	"111111100000000000000000",
	};


int step = 0;

ISR(TIMER1_COMPA_vect)
{

	PORTC ^= ((1 << 6) - 1);	// LED 
	PORTD ^= ((1 << 6) - 1);
/*
	for (int i=0; i<16; i++) {
		if (F[i][step] == 0) {
			switch(led[1][i]) {
				case 3 :
					PORTD &= ~(1 << led[0][i]);
					break;
				case 0 :
					PORTA &= ~(1 << led[0][i]);
					break;
				case 2 :
					PORTC &= ~(1 << led[0][i]);
					break;
			}
		}

		else {
			switch(led[1][i]) {
				case 3 :
				PORTD |= 1 << led[0][i];
				break;
				case 0 :
				PORTA |= 1 << led[0][i];
				break;
				case 2 :
				PORTC |= 1 << led[0][i];
				break;
			}
		}
	}

	step++;
	if (step == 25)
		step = 0;
*/
}

int main (void)
{
	sei();
	board_init();

	// BUTON PD6
	DDRD &= ~(1 << PD6);  // BUTON - input
	PORTD |= (1 << PD6);  // activam rezistenta de pull-up
  
	// LED ROSU PD7
	DDRD |= (1 << PD7);   

	// LEDURI PORTD
	DDRD |= (1 << 6) - 1;		// PIN LEDURI output
	PORTD &= ~((1 << 6) - 1);	// LED OFF
	
	// LEDURI PORTC
	DDRC |= (1 << 8) - 1;		// PIN LEDURI output
	PORTC &= ~((1 << 8) - 1);	// LED OFF

	// LEDURI PORTA
	DDRA |= (1 << PA7);			
	DDRA |= (1 << PA6);
	PORTA &= ~(1 << PA6);
	PORTA &= ~(1 << PA7);
  
	// Timer
	TCCR1B |= (1 << WGM12);		// modul CTC - capitolul 16 din datasheet,
//	TCCR1A |= (1 << COM1B0);	// toggle pe OC1B la fiecare compare match
	TCCR1B |= (1 << CS12);		// prescaler 256 - capitolul 16
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 5000;				// 16000000 / 2 / 256 = 31250



	while (1)
	{
		if (!(PIND & (1 << PD6))) {
			// daca bitul corespunzator PD6 este 0
			// PORTD |= (1 << PD7);		// LED OFF
			PORTD &= ~((1 << 6) - 1);	// LED OFF
			PORTC &= ~((1 << 8) - 1);	// LED OFF
			PORTA &= ~(1 << PA6);		// LED OFF
			PORTA &= ~(1 << PA7);		// LED OFF
		}
		else {
			// PORTD &= ~(1 << PD7);		// LED ON
			PORTD |= ((1 << 6) - 1);	// LED ON
			PORTC |= (1 << 8) - 1;		// LED ON
			PORTA |= (1 << PA6);		// LED ON
			PORTA |= (1 << PA7);		// LED ON
		}


	}
	return 0;

	// Insert application code here, after the board has been initialized.
}
