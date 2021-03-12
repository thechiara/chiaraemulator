#/*
#Copyright 2021 Elliot-Gaspard COURCHINOUX All rights reserved
#
#---------------The beginning of the  free developement license-----------------
#This source code is licensed  under the  free developement license. An opensource software license. 
#
#
#
#
#See COPYING file to read the free developement license. 
#
#You must agree the license to use this program. 
#
#If you don't agree it, you can't use this software.
#
#
#---------------The end of free developement license-----------------
#
CC=clang 
LD=ld
INCLUDEDIR:= include

CCOPTAFT =     -std=gnu99  -g  -Wall  -c  -I$(INCLUDEDIR) 


SOURCES := main.c emulation/emul.c emulation/chiaracompute.c  
OBJECTS := $(SOURCES:.c=.o)
OBJECTSD := $(SOURCES:.c=.d)




.PHONY: all   clean 



all : chiarav 


%.o: %.c
			
			cc  $<   -c -MD -o $@  $(CCOPTAFT)

			


chiarav: $(OBJECTS)
				cc  -o chiarav $(OBJECTS) -pthread  
				

clean: cleanf



cleanf: $(OBJECTS)
			rm -rf  chiarav
			rm -rf  $(OBJECTS)
			rm -rf  $(OBJECTSD)
