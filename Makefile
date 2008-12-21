MCU_TARGET     = atmega2561
OPTIMIZE       = -O2

DEFS           = -DF_CPU=1000000 -DBAUD=9600
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
	avrdude -p atmega2561 -c stk200

verify:
	avrdude -p atmega2561 -c stk200 -U flash:v:test.srec

program: test.srec
	avrdude -p atmega2561 -c stk200 -U flash:w:test.srec

include make.dep

