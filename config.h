#ifndef CONFIG_H
#define CONFIG_H

#if defined (__AVR_ATmega2561__)

#	define FETPORT PORTA
#	define FETDDR DDRA
#	define LEDPORT PORTC
#	define LEDDDR DDRC

#define	RXC RXC0
#define	RXEN RXEN0
#define	TXEN TXEN0
#define	UBRRH UBRR0H
#define	UBRRL UBRR0L
#define	UCSRA UCSR0A
#define	UCSRB UCSR0B
#define	UCSRC UCSR0C
#define	UCSZ0 UCSZ00
#define	UDR UDR0
#define	UDRE UDRE0
#define	USBS USBS0


#elif defined (__AVR_ATtiny2313__)

//FETPORT means nothing for the tiny2313
//it has so few pins that the fetport needs its own logic
// do not modify LEDPORT 
#	define FETPORT PORTA
#	define FETDDR DDRA
#	define LEDPORT PORTD
#	define LEDDDR DDRD

#else
#    warning "device type not defined"
#endif

#endif /* CONFIG_H */

