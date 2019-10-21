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

uint32_t time = 0;
uint32_t time_off = 0;

enum States {
    OFF,
    ON,
    T1,
    T2,
    T3,
    T4
};

enum States state;

uint8_t switchPressed = 0;

void initIO(void) {
    DDRB |= (1<<led_active | 1<<led1 | 1<<led2 | 1<<led3 | 1<<led4);
    DDRD |= (1<<out);
    PORTD |= (1<<taster);
    
    OCR2B = 0;
    TCCR2A |= (1 << COM0A1);
    TCCR2A |= (1 << WGM01) | (1 << WGM00);
    TCCR2B |= (1 << CS01);
    
    TCCR0 = (1<<CS01); //Prescaler 8
    TIMSK |= (1<<TOIE0);
    
    EIMSK |= (1<<INT0);
    EICRA |= (1<<ISC01);
    
    state = OFF;
    
    sei();
}

int main(void) {
	initIO();

	while (1) {
        if ((PIND & (1<<PIND2)) == 0) {
            switchPressed = 1;
        } else {
            switchPressed = 0;
        }
        
        if (time <= 1000 && time >= 50 && switchPressed == 0) { //under 2sec
            if (state == ON)
                state = T1;
            else if (state == T1)
                state = T2;
            else if (state == T2)
                state = T3;
            else if (state == T3)
                state = T4;
            else if (state == T4)
                state = OFF;
            else if (state == OFF)
                state = ON;
        } else if (time >= 1000 && switchPressed == 1) {
            if (state != OFF && (time % 50) == 0)
                OCR2B--;
        }
        
        switch (state) {
            case OFF:
                PORTB &= ~(1<<led_active | 1<<led1 | 1<<led2 | 1<<led3 | 1<<led4);
                OCR2B = 0;
                time_off = 0;
                break;
            case ON:
                PORTB |= (1<<led_active);
                if (OCR2B == 0)
                    OCR2B = 256;
                time_off = 0;
                break;
            case T1:
                PORTB |= (1<<led1);
                if (time_off > 1800000) {
                    state = OFF;
                }
                break;
            case T2:
                PORTB |= (1<<led2);
                if (time_off > 3600000) {
                    state = OFF;
                }
                break;
            case T3:
                PORTB |= (1<<led3);
                if (time_off > 5400000) {
                    state = OFF;
                }
                break;
            case T4:
                PORTB |= (1<<led4);
                if (time_off > 7200000) {
                    state = OFF;
                }
                break;
        }
        
        if (switchPressed == 0) {
            time = 0;
        }
	}
	return 0; // never reached
}

ISR(TIMER0_OVF_vect) {
    //all 2ms
    if (switchPressed)
        time++;
    
    if (state == T1 | state == T2 | state == T3 | state == T4)
        time_off++;
    
}
