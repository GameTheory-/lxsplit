# GNU Makefile for building lxsplit
# $Id: Makefile,v 1.5 2008/05/31 08:03:10 sezero Exp $

#
# FIXME: This makefile is NOT suitable for cross-compiling,
# because we are generating uoff_t.h during compilation..
#

####### Compiler, tools and options

CC	?=	gcc
ifeq ($(CC),)
CC	:=	gcc
endif
INCPATH :=	.
CFLAGS	?=	-O2
CFLAGS	:=	$(CFLAGS) -Wall -W -DNO_DEBUG
LDLIBS	:=	
LDFLAGS	:=	

# 64 bit file offsets even on 32 bit
# platforms (LFS, if supported by the
# operating system): yes or no.
USE_LARGEFILE:=	yes

INSTALL_PATH :=	/usr/local/bin

ifeq ($(USE_LARGEFILE),yes)
# these flags work for linux / glibc.
# change for others, if necessary.
FILESIZE_DEFS:=	-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
else
FILESIZE_DEFS:=	
endif

####### Files

BINARY	:=	lxsplit
HEADERS :=	config.h func.h merge.h split.h
SOURCES :=	func.c merge.c split.c main.c mkuoff_t.c
OBJECTS :=	func.o merge.o split.o main.o

####### Implicit rules

.SUFFIXES: .c

.c.o:
	$(CC) -c $(CFLAGS) $(FILESIZE_DEFS) -I$(INCPATH) -o $@ $<

####### Build rules

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(BINARY) $(OBJECTS) $(LDLIBS)

uoff_t.h: mkuoff_t.c
	$(CC) $(CFLAGS) $(FILESIZE_DEFS) -o mkuoff_t mkuoff_t.c
	@echo "Generating uoff_t.h"
	@./mkuoff_t

clean:
	-rm -f core *.o mkuoff_t uoff_t.h
#	-rm -f $(BINARY)

install:
	cp $(BINARY) $(INSTALL_PATH)

uninstall:
	rm $(INSTALL_PATH)/$(BINARY)

####### Dependencies

func.o: uoff_t.h func.c func.h config.h
main.o: uoff_t.h main.c merge.h split.h config.h
merge.o: uoff_t.h merge.c func.h merge.h config.h
split.o: uoff_t.h split.c split.h func.h config.h

