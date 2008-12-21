#include <avr/io.h>
#include "serial.h"

void USART_Init( unsigned int ubrr){
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
} // USART_Init

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void serial_writestr(char *text)
{
        char *ptr;
        for (ptr=text;*ptr!=0;ptr++)
        {
                USART_Transmit(*ptr);
        }
}


int serial_readstr(char n,char *buf)
{
        unsigned char i;
        for(i=0;i<n;i++)
        {
        buf[i]=USART_Receive();
                if(buf[i]=='\r')
                {
                        break;
                }
        }
        return i;
}

