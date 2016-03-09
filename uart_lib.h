#ifndef uart_lib
#define uart_lib

void USART_Init(uint16_t ubrr);
void USART_Transmit(char data);
void USART_Puts(char * s);
void USART_Put_Int(uint16_t liczba, uint8_t radix);

#endif
