# Author: Jiri Matejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: TODO
# Module: IFJ-MAKEFILE
# Make version: GNU Make 4.1
CFLAGS = -std=c99 -pedantic -Wall -Wextra -lm -g
default: IFJ16

IFJ16: main.o error.o scanner.o garbage_collector.o str_prcs.o parser.o stack_int.o htab.o structures.o
	gcc $(CFLAGS) -o IFJ16 $^

%.o: %.c %.h
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean zip odevzdani
clean:
	rm *.o IFJ16
zip:
	zip xmisov00.zip *.c *.h Makefile dokumentace.pdf rozdeleni

odevzdani:
	cp *.c *.h Makefile odevzdani
	cd odevzdani
	make zip
