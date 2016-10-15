#
# Author: Jiri mMatejka
# Login: xmatej52
# School: VUT FIT, Brno
# Date: 22-4-2016
# Module: IJC-DU2 hash_function.c
# gcc version 4.9.2 (Debian 4.9.2-10) 
#
CFLAGS = -std=c99 -pedantic -Wall -Wextra -lm -g
default: hello

hello: hello.c hash_function.o htab_clear.o htab_foreach.o htab_free.o htab_init.o 
	gcc $(CFLAGS) -o hello $^

hash_function.o: hash_function.c htable.h
	gcc $(CFLAGS) -c hash_function.c
	
htab_clear.o: htab_clear.c htable.h
	gcc $(CFLAGS) -c htab_clear.c
	
htab_foreach.o: htab_foreach.c htable.h
	gcc $(CFLAGS) -c htab_foreach.c
 	
htab_free.o: htab_free.c htable.h
	gcc $(CFLAGS) -c htab_free.c
	
htab_init.o: htab_init.c htable.h
	gcc $(CFLAGS) -c htab_init.c
	
htab_init2.o: htab_init2.c htable.h 
	gcc $(CFLAGS) -c htab_init2.c
	
htab_lookup_add.o: htab_lookup_add.c htable.h
	gcc $(CFLAGS) -c htab_lookup_add.c

.PHONY: clean
clean:
	rm *.o hello
