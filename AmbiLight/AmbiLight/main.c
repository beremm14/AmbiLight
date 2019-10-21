#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t led_active = PB0;
uint8_t led1 = PB1;
uint8_t led2 = PB2;
uint8_t led3 = PB3;
uint8_t led4 = PB4;

uint8_t taster = PD2;
uint8_t out = PD3;

uint8_t pwm = OCR2B;

enum States {
    OFF,
    ON,
    T1,
    T2,
    T3,
    T4
};

enum States state;

void initIO(void) {
    DDRB |= (1<<led_active | 1<<led1 | 1<<led2 | 1<<led3 | 1<<led4);
    DDRD |= (1<<out);
    PORTD |= (1<<taster);
    
    OCR2B = 0;
    TCCR2A |= (1 << COM0A1);
    TCCR2A |= (1 << WGM01) | (1 << WGM00);
    TCCR2B |= (1 << CS01);
    
    state = OFF;
}

int main(void) {
	initIO();

	while (1) {
        switch (state) {
            case OFF:
                
            case ON:
                
            case T1:
                
            case T2:
                
            case T3:
                
            case T4:
                
        }
	}
	return 0; // never reached
}
