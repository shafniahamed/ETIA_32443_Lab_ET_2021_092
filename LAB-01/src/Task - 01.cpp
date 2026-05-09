#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Set PB0 as output
    DDRB |= (1 << DDB0);

    while (1)
    {
        // Turn ON LED (PB0 high)
        PORTB |= (1 << PB0);
        _delay_ms(1000);

        // Turn OFF LED (PB0 low)
        PORTB &= ~(1 << PB0);
        _delay_ms(1000);
    }

    return 0;
}