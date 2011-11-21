MCU_TARGET     = atmega2561
#MCU_TARGET     = attiny2313
FREQ		= 1000000
#FREQ		= 7372800
#FREQ		= 12000000
#FREQ           = 3579545 

OPTIMIZE       = -O2

DEFS           = -DF_CPU=$(FREQ) -DBAUD=9600 -DNO_FONT
LIBS           =

OBJS = test.o serial.o font_mini_4x6.o

TARGET = test

CC             = avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS        += -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS       = -g -Wl,-Map,$(TARGET).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump


all: $(TARGET).srec

$(TARGET).srec: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o test $(OBJS)
	$(OBJCOPY) -j .text -j .data -O ihex test test.srec

clean:
	rm -rf *.o $(PRG).elf  
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
	rm -f test *.srec
	rm -f $(OBJS) $(DEPENDENCIES) make.dep

make.dep: 
	$(CC) -M $(CFLAGS) $(LDFLAGS) *.c >make.dep

hwtest:
	avrdude -p $(MCU_TARGET) -c stk200 -U lfuse:r:-:b

outerquartz:
	avrdude -p $(MCU_TARGET) -c stk200 -U lfuse:w:0xed:m

innerrc:
	avrdude -p $(MCU_TARGET) -c stk200 -U lfuse:w:0b1100100:m

verify:
	avrdude -p $(MCU_TARGET) -c stk200 -U flash:v:test.srec

program: test.srec
	avrdude -p $(MCU_TARGET) -c stk200 -U flash:w:test.srec

include make.dep

