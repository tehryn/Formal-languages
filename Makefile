# Author: Jiri Matejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: TODO
# Module: IFJ-MAKEFILE
# Make version: GNU Make 4.1
CFLAGS = -std=c99 -pedantic -Wall -Wextra -lm -g
default: IFJ16

IFJ16: main.o error.o scanner.o garbage_collector.o parser.o ial.o structures.o expression.o
	gcc $(CFLAGS) -o IFJ16 $^
	cp IFJ16 testing/

%.o: %.c %.h
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean odevzdani
clean:
	rm *.o IFJ16

odevzdani:
	cp *.c *.h odevzdani
	make -C ./odevzdani zip
	rm odevzdani/*.c odevzdani/*.h
	./odevzdani/is_it_ok.sh ./odevzdani/xmisov00.zip ./odevzdani/test
	make -C ./odevzdani/test
	rm ./odevzdani/test/*
