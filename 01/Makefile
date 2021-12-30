main: sh.c getty.c init.c
	gcc -o sh sh.c
	gcc -o getty getty.c
	gcc -o init init.c

.PHONY: clean
clean:
	rm sh
	rm getty
	rm init
