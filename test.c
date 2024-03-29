#include "config.h"
#include <util/delay.h>
#include <avr/io.h>
#include "font.h"
#include "serial.h"

void setled(i,j)
{
#if defined (__AVR_ATtiny2313__)
	if(i<3)
	{
		PORTA=PORTA&(0xf8|(1<<i));
	} else {
		PORTD=PORTD&(0x83|(1<<(i-1)));
	}
#else /* __AVR_ATtiny2313__ */
	FETPORT=(1<<i)^0x04;
#endif /* __AVR_ATtiny2313__ */
	LEDPORT=1<<j;
}
void initleds()
{
#if defined (__AVR_ATtiny2313__)
	/* FET side bottom up is A0,A1,A2,D2,D3,D4,D5,D6*/
	DDRA=0x07;
	DDRD=0x7c;
	PORTA=PORTA&0xf8;
	PORTD=PORTD&0x83;
#else /* __AVR_ATtiny2313__ */
	FETPORT=0x00;
	FETDDR=0x0ff;
#endif /* __AVR_ATtiny2313__ */
	LEDPORT=0x00;
	LEDDDR=0x0ff;
}
void clearleds()
{
#if defined (__AVR_ATtiny2313__)
	PORTA=PORTA&0xf8;
	PORTD=PORTD&0x83;
#else /* __AVR_ATtiny2313__ */
	FETPORT=0x00;
#endif /* __AVR_ATtiny2313__ */
	LEDPORT=0x00;
}

char outbuffer[]="b x y\r\n";
void ackcmd(char c,int i, int j)
{
	outbuffer[0]=c;
	outbuffer[2]=(char) i+'0';
	outbuffer[4]=(char) j+'0';
	serial_writestr(outbuffer);
}

void blinkled(int i, int j)
{
	int k;
	for(k=0;k<1;k++)
	{
		setled(i,j);
		_delay_ms(100);
		clearleds();	
		_delay_ms(50);
		
	}
}

unsigned char videomem[8];

void initvga()
{
	int i;
	for(i=0;i<8;i++)
	{
		videomem[i]=0;
	}
}

extern inline void displayonserial(char * ptr)
{
	int i;
	int j;
	char c;
	USART_Transmit('');
	for(i=7;i>-0;i--)
	{
		c=pgm_read_byte(ptr+i);
		//c=*(ptr+i);
		USART_Transmit((c>>4)+'0');
		USART_Transmit((c%16)+'0');
		for(j=0;j<8;j++)
		{
			if((1<<j)&c)
			{
				USART_Transmit('X');
			} else {
				USART_Transmit('.');
			}
		}
		USART_Transmit('\r');
		USART_Transmit('\n');
	}
}
	
void displayvga()
{
	/*
	* displays the videomem.
        * loops on the FET side to minimize load on led side ports.
        * displays rows on the led side
        */
	int i;
	int j;
	//displayonserial(videomem);
	for(j=0;j<10;j++){
		for(i=0;i<8;i++)
		{
#if defined (__AVR_ATtiny2313__)
			if(i<3)
			{
				PORTA=PORTA&(0xf8|(1<<i));
			} else {
				PORTD=PORTD&(0x83|(1<<(i-1)));
			}
#else /* __AVR_ATtiny2313__ */
				FETPORT=(1<<i)^0x04;
#endif /* __AVR_ATtiny2313__ */
			LEDPORT=videomem[i];
			_delay_ms(1);
		}
	}
}

void vgashiftin(unsigned char col)
{
/* shifts a column from right into video memory
   each rows are shifted left, and bits from col are added.
   row 0 is bit 0
*/
	int i;
	for(i=0;i<8;i++)
	{
		videomem[i]=(videomem[i])<<1;
		if(col&(1<<i))
		{
			(videomem[i])++;
		}
	}
}

unsigned char font_col(char letter,int col)
{
/*
	returns nth column of a letter, suitable for use by vgashiftin
	the letter is encoded in FONTARRAY by FONTHEIGHT bytes as the letter-th letter
	colums are in high->low order!
*/
	unsigned char c=0;
	unsigned char row;
	int i;
	for(i=0;i<FONTHEIGHT;i++)
	{
		c=c<<1;
		row=GETFONTROW(letter,i);
		if(row&(1<<(7-col)))
		{
			c++;
		}
	}
	return c;
}

void vga_putc(char c)
{
/*
	scrolls in a char into the display
	for each column shifts it in, then adds an empty one.
	does a displayvga after each shift
	the font have FONTWIDTH columns.
*/
	int i;
	unsigned char col;
	//displayonserial(FONTARRAY);//+c*FONTHEIGHT);
	USART_Transmit(c);
	for(i=0;i<FONTWIDTH;i++)
	{
		col=font_col(c,i);
		vgashiftin(col);
		displayvga();
	}
	//col=0;
	//vgashiftin(col);
	//displayvga();
}

void scrolltext(char *str)
{
/*
	scrolls a text in the display
*/
	char * ptr;
	for(ptr=str;(*ptr);ptr++)
	{
		vga_putc(*ptr);
	}
}


char buffer[4];
char hello[]=".oO Hello Juli! Oo.";
char sorminta[]="//I";
main()
{
	int x,y,i,j;
	USART_Init(MYUBRR);
//	DDRB=0xff;
//	DDRD=0xff;
	while(1)
	{
//		PORTB=0xff;
//		PORTD=0xff;
		_delay_ms(500);
		serial_writestr("\r\nmatrix>");
//		PORTB=0x0;
//		PORTD=0x0;
		_delay_ms(500);
	}

	initleds();
	while(1)
	{
	serial_writestr("\r\nmatrix>");
	}

	for(y=0;y<3;y++)
	{
		scrolltext(hello);
	}
	while(1)
	{
		serial_writestr("\r\nmatrix>");
		serial_readstr(4,buffer);
		x=buffer[1]-'0';
		y=buffer[2]-'0';
		ackcmd(*buffer,x, y);
		switch(*buffer)
		{
			case 'b':
				blinkled(x,y);
				break;
			case 's':
				setled(x,y);
				break;
			case 'c':
				serial_writestr("clear\r\n");
				initleds();
				break;
			case 'k':
				LEDPORT=x+y*16;
				break;
			case 'l':
				for(i=0;i<8;i++)
				{
					for(j=0;j<8;j++)
					{
						blinkled(i,j);
					}
				}
				break;
			case 'h':
				for(y=0;y<x;y++)
				{
					scrolltext(hello);
				}
				break;
			case 'H':
				for(y=0;y<x;y++)
				{
					scrolltext(sorminta);
				}
				break;
			default:
				serial_writestr("unknown command\r\n");
		}
	}
}
