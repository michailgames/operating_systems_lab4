all: zad4

zad4: zad4.c
	gcc -std=gnu99 -Wall -Wextra zad4.c -o zad4
    
clean:
	rm -f zad4
