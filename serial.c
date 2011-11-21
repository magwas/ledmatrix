#include "config.h"
#include <avr/io.h>
#include "serial.h"

void USART_Init( unsigned int ubrr){
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (0<<USBS)|(3<<UCSZ0);
} // USART_Init

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
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

