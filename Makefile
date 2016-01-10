#
#   SEGA SATURN Graphic library make file for GNU
#
# slightly modified for ISO building, COFF toolchain

# specify on command line
# OBJFMT = coff
 OBJFMT = elf

# macro
CC = sh-$(OBJFMT)-gcc
CONV = sh-$(OBJFMT)-objcopy
RM = rm

# directory
SGLDIR = C:/SaturnOrbit/SGL_302j
SGLIDR = $(SGLDIR)/inc
SGLLDR = $(SGLDIR)/lib_elf

CMNDIR = C:/SaturnOrbit/COMMON
OBJECTS = ./objects

# option
#CCFLAGS = -O2 -m2 -g -c -I$(SGLIDR)
# -fomit-frame-pointer -fsort-data 
CCFLAGS = -Os  -m2 -c -I$(SGLIDR) 

# -m2 must be specified in LDFLAGS so the linker will search the SH2 lib dirs
# Specify path of libsgl.a by using "-L" option

LDFLAGS = -m2 -L$(SGLLDR) -Xlinker --format=coff-sh -Xlinker -T$(LDFILE) -Xlinker -Map \
          -Xlinker $(MPFILE) -Xlinker -e -Xlinker ___Start -nostartfiles -LL:/GNUSHv13/sh-elf/sh-elf/lib/m2/libc.a
DFLAGS =
include $(OBJECTS)

TARGET   = root/sl.coff
TARGET1  = root/sl.bin
MPFILE   = $(TARGET:.coff=.maps)
IPFILE   = $(CMNDIR)/IP.BIN
LDFILE   = $(CMNDIR)/SL.lnk
MAKEFILE = makefile


all: $(TARGET) $(TARGET1)

# Use gcc to link so it will automagically find correct libs directory

$(TARGET) : $(SYSOBJS) $(OBJS) $(MAKEFILE) $(LDFILE) #$(OBJECTS)
	$(CC) $(LDFLAGS) $(SYSOBJS) $(OBJS) $(LIBS) -o $@

$(TARGET1) : $(SYSOBJS) $(OBJS) $(MAKEFILE) $(LDFILE)
	$(CONV) -O binary $(TARGET) $(TARGET1)

#$(LDFILE) : $(MAKEFILE)
#	@echo Making $(LDFILE)
#	@echo SECTIONS {		> $@
#	@echo 	SLSTART 0x06004000 : {	>> $@
#	@echo 		___Start = .;	>> $@
#	@echo 		*(SLSTART)	>> $@
#	@echo 	}			>> $@
#	@echo 	.text ALIGN(0x20) :			>> $@
#	@echo 	{			>> $@
#	@echo 		* (.text)			>> $@
#	@echo 		*(.strings)			>> $@
#	@echo 		__etext = .;			>> $@
#	@echo 	}			>> $@
#	@echo 	SLPROG ALIGN(0x20): {	>> $@
#	@echo 		__slprog_start = .;	>> $@
#	@echo 		*(SLPROG)	>> $@
#	@echo 		__slprog_end = .;	>> $@
#	@echo 	}			>> $@
#	@echo 	.tors  ALIGN(0x10) :			>> $@
#	@echo 	{			>> $@
#	@echo 		___ctors = . ;			>> $@
#	@echo 		*(.ctors)			>> $@
#	@echo 		___ctors_end = . ;			>> $@
#	@echo 		___dtors = . ;			>> $@
#	@echo 		*(.dtors)			>> $@
#	@echo 		___dtors_end = . ;			>> $@
#	@echo 	}			>> $@
#	@echo 	.data ALIGN(0x10):			>> $@
#	@echo 	{			>> $@
#	@echo 		* (.data)			>> $@
#	@echo 		_edata = . ;			>> $@
#	@echo 	}			>> $@
#	@echo 	.bss ALIGN(0x10) (NOLOAD):			>> $@
#	@echo 	{			>> $@
#	@echo 		__bstart = . ;			>> $@
#	@echo 		*(.bss)			>> $@
#	@echo 		* ( COMMON )			>> $@
#	@echo 		__bend = . ;			>> $@
#	@echo 	_end = .;			>> $@
#	@echo 	}			>> $@
#	@echo }				>> $@

# suffix
.SUFFIXES: .asm

.c.o:
	$(CC) $< $(DFLAGS) $(CCFLAGS) $(_CCFLAGS) -o $@

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET1) $(TARGET2) $(MPFILE) cd/0.bin



