#
# Author: Jiri Matejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: 22-4-2016
# Module: IJC-DU2 hash_function.c
# gcc version 4.9.2 (Debian 4.9.2-10)
#
CFLAGS = -std=c99 -pedantic -Wall -Wextra -lm -g
default: IFJ16

IFJ16: main.o error.o scanner.o garbage_collector.o str_prcs.o parser.o stack_int.o htab.o stack.o
	gcc $(CFLAGS) -o IFJ16 $^

%.o: %.c %.h
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm *.o IFJ16
