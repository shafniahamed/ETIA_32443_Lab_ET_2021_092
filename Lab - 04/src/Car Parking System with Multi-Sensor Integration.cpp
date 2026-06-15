#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define PIR_SENSOR PD3
#define RESET_BUTTON PD2

#define GREEN_LED PB0
#define YELLOW_LED PB1
#define RED_LED PB2

const uint8_t initial_capacity = 10;

uint8_t vehicles_entered = 0;
uint8_t available_slots = 10;

uint8_t currentState = 0;
uint8_t previousState = 0;

void updateLEDs(void)
{
    PORTB &= ~((1<<GREEN_LED) |
               (1<<YELLOW_LED) |
               (1<<RED_LED));

    if(available_slots > (initial_capacity/2))
    {
        PORTB |= (1<<GREEN_LED);
    }
    else if(available_slots > 0)
    {
        PORTB |= (1<<YELLOW_LED);
    }
    else
    {
        PORTB |= (1<<RED_LED);
    }
}

int main(void)
{
    DDRB |= (1<<GREEN_LED);
    DDRB |= (1<<YELLOW_LED);
    DDRB |= (1<<RED_LED);

    DDRD &= ~(1<<PIR_SENSOR);

    DDRD &= ~(1<<RESET_BUTTON);

    PORTD |= (1<<RESET_BUTTON);

    updateLEDs();

    while(1)
    {
        if(PIND & (1<<PIR_SENSOR))
        {
            currentState = 1;
        }
        else
        {
            currentState = 0;
        }

        if(currentState == 1 &&
           previousState == 0 &&
           vehicles_entered < initial_capacity)
        {
            vehicles_entered++;
        }

        previousState = currentState;

        available_slots =
            initial_capacity - vehicles_entered;

        // Update LEDs
        updateLEDs();

        if(!(PIND & (1<<RESET_BUTTON)))
        {
            _delay_ms(50);

            if(!(PIND & (1<<RESET_BUTTON)))
            {
                vehicles_entered = 0;
                available_slots = initial_capacity;

                updateLEDs();

                while(!(PIND & (1<<RESET_BUTTON)));
            }
        }

        _delay_ms(100);
    }
}