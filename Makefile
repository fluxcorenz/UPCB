# MPLAB IDE generated this makefile for use with Microsoft `nmake'.
# Project: UPCB.mcp
# Date: Sat Apr 25 08:15:42 2015

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = del
CCFLAGS = -p=18F4550 -pa=5
MLIB = c:\Program Files (x86)\Microchip\mplabc18\v3.47\h
OUTPUT = _output
OBJ = obj

"create_dirs": 
	@if not exist $(OUTPUT) mkdir $(OUTPUT)
	@if not exist $(OBJ) mkdir $(OBJ)

all: "create_dirs" "$(OUTPUT)\UPCB.cof"

"$(OUTPUT)\UPCB.cof" : "$(OBJ)\main.o" "$(OBJ)\neogeo.o" "$(OBJ)\psx.o" "$(OBJ)\saturn.o" "$(OBJ)\genesis.o" "$(OBJ)\tg16.o" "$(OBJ)\snes.o" "$(OBJ)\nes.o" "$(OBJ)\jaguar.o" "$(OBJ)\x68000.o" "$(OBJ)\3do.o" "$(OBJ)\program.o" "$(OBJ)\gc.o" "$(OBJ)\piggy.o" "$(OBJ)\upcb.o" "$(OBJ)\cd32.o" "$(OBJ)\wii.o" "$(OBJ)\inttest.o" "$(OBJ)\allusb.o" "$(OBJ)\ps3usb.o" "$(OBJ)\xboxusb.o"
	$(LD) /p18F4550 "rm18f4550.lkr" "$(OBJ)\main.o" "$(OBJ)\neogeo.o" "$(OBJ)\psx.o" "$(OBJ)\saturn.o" "$(OBJ)\genesis.o" "$(OBJ)\tg16.o" "$(OBJ)\snes.o" "$(OBJ)\nes.o" "$(OBJ)\jaguar.o" "$(OBJ)\x68000.o" "$(OBJ)\3do.o" "$(OBJ)\program.o" "$(OBJ)\gc.o" "$(OBJ)\piggy.o" "$(OBJ)\upcb.o" "$(OBJ)\cd32.o" "$(OBJ)\wii.o" "$(OBJ)\inttest.o" "$(OBJ)\allusb.o" "$(OBJ)\ps3usb.o" "$(OBJ)\xboxusb.o" /u_CRUNTIME /z__MPLAB_BUILD=1 /m"$(OUTPUT)\UPCB.map" /o"$(OUTPUT)\UPCB.cof"

"$(OBJ)\main.o" : "main.c" "$(MLIB)\delays.h" "upcb.h" "nes.h" "snes.h" "gc.h" "piggy.h" "main.c" "c:\Program Files (x86)\Microchip\mplabc18\v3.47\h\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h" "neogeo.h" "psx.h" "saturn.h" "genesis.h" "tg16.h" "jaguar.h" "x68000.h" "3do.h" "cd32.h" "wii.h" "inttest.h" "allusb.h" "ps3usb.h" "xboxusb.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "main.c" -fo="$(OBJ)\main.o"

"$(OBJ)\neogeo.o" : "neogeo.c" "upcb.h" "$(MLIB)\timers.h" "neogeo.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "neogeo.h" "config.h" "program.h" "$(MLIB)\pconfig.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "neogeo.c" -fo="$(OBJ)\neogeo.o

"$(OBJ)\psx.o" : "psx.c" "upcb.h" "$(MLIB)\spi.h" "$(MLIB)\delays.h" "psx.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "psx.h" "config.h" "program.h" "$(MLIB)\pconfig.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "psx.c" -fo="$(OBJ)\psx.o

"$(OBJ)\saturn.o" : "saturn.c" "upcb.h" "saturn.c" "saturn.h" "config.h" "program.h" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "inttest.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "saturn.c" -fo="$(OBJ)\saturn.o

"$(OBJ)\genesis.o" : "genesis.c" "upcb.h" "$(MLIB)\timers.h" "genesis.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "genesis.h" "config.h" "program.h" "$(MLIB)\pconfig.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "genesis.c" -fo="$(OBJ)\genesis.o

"$(OBJ)\tg16.o" : "tg16.c" "upcb.h" "tg16.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "tg16.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "tg16.c" -fo="$(OBJ)\tg16.o

"$(OBJ)\snes.o" : "snes.c" "snes.h" "upcb.h" "snes.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "snes.c" -fo="$(OBJ)\snes.o

"$(OBJ)\nes.o" : "nes.c" "nes.h" "upcb.h" "$(MLIB)\delays.h" "nes.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "nes.c" -fo="$(OBJ)\nes.o

"$(OBJ)\jaguar.o" : "jaguar.c" "upcb.h" "jaguar.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "jaguar.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "jaguar.c" -fo="$(OBJ)\jaguar.o

"$(OBJ)\x68000.o" : "x68000.c" "upcb.h" "x68000.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "x68000.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "x68000.c" -fo="$(OBJ)\x68000.o

"$(OBJ)\3do.o" : "3do.c" "upcb.h" "3do.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "3DO.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "3do.c" -fo="$(OBJ)\3do.o

"$(OBJ)\program.o" : "program.c" "upcb.h" "program.c" "program.h" "config.h" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "program.c" -fo="$(OBJ)\program.o

"$(OBJ)\gc.o" : "gc.c" "gc.h" "upcb.h" "$(MLIB)\delays.h" "gc.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "gc.c" -fo="$(OBJ)\gc.o

"$(OBJ)\piggy.o" : "piggy.c" "upcb.h" "piggy.h" "$(MLIB)\delays.h" "piggy.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "piggy.c" -fo="$(OBJ)\piggy.o

"$(OBJ)\upcb.o" : "upcb.c" "upcb.h" "upcb.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "upcb.c" -fo="$(OBJ)\upcb.o

"$(OBJ)\cd32.o" : "cd32.c" "upcb.h" "cd32.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "cd32.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "cd32.c" -fo="$(OBJ)\cd32.o

"$(OBJ)\wii.o" : "wii.c" "upcb.h" "$(MLIB)\i2c.h" "wii.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "wii.h" "config.h" "program.h" "$(MLIB)\pconfig.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "wii.c" -fo="$(OBJ)\wii.o

"$(OBJ)\inttest.o" : "inttest.c" "upcb.h" "inttest.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "inttest.h" "config.h" "program.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "inttest.c" -fo="$(OBJ)\inttest.o

"$(OBJ)\allusb.o" : "allusb.c" "upcb.h" "allusb.c" "allusb.h" "config.h" "program.h" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "allusb.c" -fo="$(OBJ)\allusb.o

"$(OBJ)\ps3usb.o" : "ps3usb.c" "upcb.h" "$(MLIB)\timers.h" "ps3usb.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "ps3usb.h" "allusb.h" "config.h" "program.h" "$(MLIB)\pconfig.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "ps3usb.c" -fo="$(OBJ)\ps3usb.o

"$(OBJ)\xboxusb.o" : "xboxusb.c" "upcb.h" "$(MLIB)\timers.h" "xboxusb.c" "$(MLIB)\p18cxxx.h" "$(MLIB)\p18f4550.h" "config.h" "program.h" "allusb.h" "$(MLIB)\pconfig.h" "xboxusb.h"
	$(CC) $(CCFLAGS) /i"$(MLIB)" "xboxusb.c" -fo="$(OBJ)\xboxusb.o

"clean" : 
	@if exist $(OUTPUT) rmdir /S /Q $(OUTPUT)
	@if exist $(OBJ) rmdir /S /Q $(OBJ)

