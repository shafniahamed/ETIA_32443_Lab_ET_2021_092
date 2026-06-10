#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#define PASSWORD_LENGTH 4


uint8_t password[PASSWORD_LENGTH] = {1, 2, 3, 4};


uint8_t entered[PASSWORD_LENGTH];


uint8_t indexPos = 0;


void buttonDelay();

void setup()
{

    DDRB |= (1 << PB0) | (1 << PB1);


    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);


    DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5));


    PORTD |= (1 << PD2) | (1 << PD3) | (1 << PD4) | (1 << PD5);
}

void loop()
{
  
    if (!(PIND & (1 << PD2)))
    {
        entered[indexPos] = 1;
        indexPos++;

        buttonDelay();
    }

 
    if (!(PIND & (1 << PD3)))
    {
        entered[indexPos] = 2;
        indexPos++;

        buttonDelay();
    }

    if (!(PIND & (1 << PD4)))
    {
        entered[indexPos] = 3;
        indexPos++;

        buttonDelay();
    }

  
    if (!(PIND & (1 << PD5)))
    {
        entered[indexPos] = 4;
        indexPos++;

        buttonDelay();
    }


    if (indexPos >= PASSWORD_LENGTH)
    {
        uint8_t correct = 1;

        for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
        {
            if (entered[i] != password[i])
            {
                correct = 0;
                break;
            }
        }


        if (correct)
        {
            PORTB |= (1 << PB0);   
            PORTB &= ~(1 << PB1);
        }


        else
        {
            PORTB |= (1 << PB1);
            PORTB &= ~(1 << PB0);
        }

        
        _delay_ms(3000);

     
        PORTB &= ~(1 << PB0);
        PORTB &= ~(1 << PB1);

        indexPos = 0;
    }
}


void buttonDelay()
{
    _delay_ms(300);
}