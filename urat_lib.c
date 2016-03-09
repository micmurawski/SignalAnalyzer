#include <avr/io.h>
#include <stdlib.h>

void USART_Init(uint16_t ubrr){
UBRRH = (uint8_t)(ubrr>>8);
UBRRL = (uint8_t)ubrr;
UCSRB = (1<<TXEN);
}

void USART_Transmit(char data){
while ( !( UCSRA & (1<<UDRE)) );
UDR = data;
}

void USART_Puts(char * s){
	while(*s) USART_Transmit(*s++);
}

void USART_Put_Int(uint16_t liczba, uint8_t radix){
	char buff[255];
	ltoa(liczba,buff,radix);
	USART_Puts(buff);
}
