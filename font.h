
/* Hand composed "Miniscule" 4x6 font, with binary data generated using
 * Perl stub.
 *
 * Use 'perl -x mini_4x6.c < mini_4x6.c > new_version.c' to regenerate
 * binary data.
 *
 * Created by Kenneth Albanowski.
 * No rights reserved, released to the public domain.
 *
 * Version 1.0
 */

#include <avr/pgmspace.h>

#define FONTDATAMAX 1536
#define FONTHEIGHT 6
#define FONTWIDTH 4
#define FONTARRAY fontdata_mini_4x6
//#define GETFONTROW(letter,i) (FONTARRAY[letter*FONTHEIGHT+i])
#ifdef SMALL_MEM
#define GETFONTROW(letter,i) pgm_read_byte(&(FONTARRAY[(letter-32)*FONTHEIGHT+i]))
#else /*SMALL_MEM*/
#define GETFONTROW(letter,i) pgm_read_byte(&(FONTARRAY[letter*FONTHEIGHT+i]))
#endif /*SMALL_MEM*/

unsigned char fontdata_mini_4x6[];
//unsigned char fontdata_mini_4x6[FONTDATAMAX];

