# Author: Jiri Matejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: TODO
# Module: IFJ-MAKEFILE
# Make version: GNU Make 4.1
CFLAGS = -std=c99
CC = gcc
default: IFJ16

IFJ16: main.o error.o scanner.o garbage_collector.o parser.o ial.o structures.o expression.o embedded_functions.o
	$(CC) $(CFLAGS) -o IFJ16 main.o error.o scanner.o garbage_collector.o parser.o ial.o structures.o expression.o

main.o: main.c
	$(CC) -c main.c -o $@
error.o: error.c error.h
	$(CC) -c error.c -o $@
scanner.o: scanner.c scanner.h
	$(CC) -c scanner.c -o $@
garbage_collector.o: garbage_collector.c garbage_collector.h
	$(CC) -c garbage_collector.c -o $@
parser.o: parser.c parser.h
	$(CC) -c parser.c -o $@
ial.o: ial.c ial.o
	$(CC) -c ial.c -o $@
structures.o: structures.c structures.o
	$(CC) -c structures.c -o $@
expression.o: expression.c expression.h
	$(CC) -c expression.c -o $@
embedded_functions.o: embedded_functions.c embedded_functions.h
	$(CC) -c embedded_functions.c -o $@

.PHONY: clean zip test
clean:
	rm *.o IFJ16
zip:
	zip -r xmisov00.zip *.c *.h Makefile dokumentace.pdf rozdeleni rozsireni ../tests/
test:
	chmod +x ./tests/run_test.sh
	./tests/run_test.sh
test-valgrind:
	chmod +x ./tests/valgrind_test.sh
	./tests/valgrind_test.sh
	./tests/valgrind_test.sh check_leaks
