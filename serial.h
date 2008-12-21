#include <avr/io.h>

#define MYUBRR (F_CPU/16/BAUD-1)

void USART_Init( unsigned int ubrr);
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
void serial_writestr(char *text);
int serial_readstr(char n,char *buf);

