# Author: Jiri Matejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: TODO
# Module: IFJ-MAKEFILE
# Make version: GNU Make 4.1
CFLAGS = -std=c99 -pedantic -Wall -Wextra -lm -g
default: IFJ16

<<<<<<< HEAD
IFJ16: main.o error.o scanner.o garbage_collector.o parser.o stack_int.o htab.o stack.o expression.o
	g++ $(CFLAGS) -o IFJ16 $^
=======
IFJ16: main.o error.o scanner.o garbage_collector.o str_prcs.o parser.o stack_int.o htab.o stack.o
	gcc $(CFLAGS) -o IFJ16 $^
>>>>>>> c92e5866aecc1d266543e7cb810c0d1dc38ea537

%.o: %.c %.h
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm *.o IFJ16
