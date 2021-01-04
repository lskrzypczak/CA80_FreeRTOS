#	FreeRTOS.org V4.5.0 - Copyright (C) 2003-2007 Richard Barry.
#
#	This file is part of the FreeRTOS.org distribution.
#
#	FreeRTOS.org is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; either version 2 of the License, or
#	(at your option) any later version.
#
#	FreeRTOS.org is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with FreeRTOS.org; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	A special exception to the GPL can be applied should you wish to distribute
#	a combined work that includes FreeRTOS.org, without being obliged to provide
#	the source code for any proprietary components.  See the licensing section 
#	of http://www.FreeRTOS.org for full details of how and when the exception
#	can be applied.
#
#	***************************************************************************
#	See http://www.FreeRTOS.org for documentation, latest information, license 
#	and contact details.  Please ensure to read the configuration and relevant 
#	port sections of the online documentation.
#	***************************************************************************


CC=sdcc
NO_OPT=--nogcse --noinvariant --noinduction --nojtbound --noloopreverse --nolabelopt --nooverlay --peep-asm
DEBUG=--debug

CFLAGS=-mz80 -I. -I../Common/include -I../include -I../../Source/include \
   --stack-auto  --int-long-reent --float-reent

ASM=as-z80
ASMFLAGS=-xlsp

LNK=link-z80
LNKFLAGS=-p,-u

FREERTOS_DIR = /FreeRTOS
#DEMO_DIR = ../Common
#SOURCE_DIR = ../../Source
#PORT_DIR = ../../Source/portable/SDCC/Z84C15
SOURCE_DIR = $(FREERTOS_DIR)/Source
PORT_DIR = $(FREERTOS_DIR)/Source/portable/SDCC/Z84C15

ROM = 0x2000
RAM = 0xE000
RAMLEN = 0x2000
CODE     = $(ROM)+0x100
HEAP_END = $(RAM)+$(RAMLEN)-0x80
STACK    = $(RAM)+$(RAMLEN)-1

#define C source files
SRC	= main.c\
$(SOURCE_DIR)/tasks.c \
$(SOURCE_DIR)/queue.c \
$(SOURCE_DIR)/list.c \
$(SOURCE_DIR)/portable/MemMang/heap_3.c \
$(PORT_DIR)/port.c

# Define all object files.
OBJ = $(SRC:.c=.o)

#define ASM source files
CRTS = $(PORT_DIR)/crt0.s
CRTO = $(PORT_DIR)/crt0.o

main.hex : crt0.ihx Makefile
	packihx crt0.ihx>main.hex

#main.hex : main.ihx Makefile
#	packihx main.ihx>main.hex

#main.ihx : main.c Makefile ../../Source/portable/SDCC/Z84C15/portmacro.h  $(OBJ) $(CRTO)
crt0.ihx : main.c Makefile ../../Source/portable/SDCC/Z84C15/portmacro.h  $(OBJ) $(CRTO)  z84c15.h  FreeRTOSConfig.h
	echo LINKER
	$(CC) -mz80 --no-std-crt0 $(CRTO) $(OBJ) -Wl-p,-u,-b_HEADER=$(ROM),-b_CODE=$(CODE),-b_DATA=$(RAM),-b_HEAP_END=$(HEAP_END),-g__STACK=$(STACK)

%.o : %.c Makefile ../../Source/portable/SDCC/Z84C15/portmacro.h  z84c15.h FreeRTOSConfig.h
	echo $< $@
	$(CC) -c $(CFLAGS) $< -o $@

$(CRTO) : $(CRTS)
	$(ASM) $(ASMFLAGS) -o $@ $<

