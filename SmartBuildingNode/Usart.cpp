#include <avr/io.h>
#include <avr/interrupt.h>

#include "Usart.h"


void (*Usart_callback) (char) = nullptr;

void Usart::init(unsigned long baud) {
    UCSR0A = 0 << TXC0 | 0 << U2X0 | 0 << MPCM0;
    UCSR0B = 1 << RXCIE0 | 0 << TXCIE0 | 0 << UDRIE0 | 1 << RXEN0 | 1 << TXEN0 | 0 << UCSZ02 | 0 << TXB80;
    UCSR0C = 0 << UMSEL01 | 0 << UMSEL00 | 0 << UPM01 | 0 << UPM00 | 0 << USBS0 | 1 << UCSZ01 | 1 << UCSZ00 | 0 << UCPOL0;
    UBRR0 = F_CPU / 16 / baud - 1;
}

void Usart::setCallback(void (*callback_a)(char)) {
    Usart_callback = callback_a;
}

void Usart::write(char c) {
    while (0 == (UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void Usart::write(char* s) {
    while (*s) {
        write(*s);
        ++s;
    }
}

ISR(USART_RX_vect) {
    char c = UDR0;
    if (Usart_callback != nullptr) {
        Usart_callback(c);
    }
}