# Author: Jiri Matejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: TODO
# Module: IFJ-MAKEFILE
# Make version: GNU Make 4.1
CFLAGS = -std=c99 -pedantic -Wall -Wextra -g
default: IFJ16

IFJ16: main.o parser.o expression.o scanner.o ial.o structures.o garbage_collector.o error.o interpret.o embedded_functions.o
	gcc $(CFLAGS) -o IFJ16 $^

%.o: %.c %.h
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean odevzdani test
clean:
	rm *.o IFJ16

zip:
	zip -j xmisov00.zip *.c *.h  odevzdani/dokumentace.pdf odevzdani/Makefile odevzdani/rozdeleni odevzdani/rozsireni
#	zip -ur xmisov00.zip tests
	mv xmisov00.zip odevzdani/

odevzdani:
#	cp *.c *.h odevzdani
	make zip
	./odevzdani/is_it_ok.sh ./odevzdani/xmisov00.zip ./odevzdani/test
#	make -C ./odevzdani/test
	cp ./odevzdani/test/IFJ16 .
	echo "tests will start in 5 seconds"
	sleep 5
	make test
	make test-valgrind
	rm ./odevzdani/test/*
test:
	echo "Test funkcionality"
	chmod +x ./tests/run_test.sh
	./tests/run_test.sh
test-valgrind:
	chmod +x ./tests/valgrind_test.sh
	./tests/valgrind_test.sh
	echo "memory leaks"
	./tests/valgrind_test.sh check_leaks
