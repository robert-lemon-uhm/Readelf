#///////////////////////////////////////////////////////////////////////////////
#/// Software Reverse Engineering
#/// ReadELF 
#///
#/// @file Makefile
#/// @version 1.0
#///
#/// @author Robert Lemon <rlemon@hawaii.edu>
#/// @date 05_mar_2021
#///////////////////////////////////////////////////////////////////////////////

#Build a ReadELF program

CC     = gcc
CFLAGS = -g -Wall

TARGET = Readelf

DEPENDENCIES = Readelf.c fileHeader.c sectionHeaders.c hexDump.c utility.c

RUNTIME-ARGS = -h -t -x .rodata
RUNTIME-FILES = /bin/touch /bin/cal

all: $(TARGET)

Readelf: $(DEPENDENCIES)
	$(CC) $(CFLAGS) -o $(TARGET) $(DEPENDENCIES)

clean:
	rm $(TARGET)

test: all
	$(info )
	$(info Default arg string is "$(RUNTIME-ARGS)")
	$(info Default arg files are "$(RUNTIME-FILES)")
	$(info These can be edited in the Makefile, lines 8 and 9)
	$(info )
	./$(TARGET) $(RUNTIME-ARGS) $(RUNTIME-FILES)
