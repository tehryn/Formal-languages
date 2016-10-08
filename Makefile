hello: hello.c
	gcc hello.c -Wall -Wextra -std=c99 -o hello
clean:
	rm hello
