all: zad4 zad5

zad4: zad4.c
	gcc -std=gnu99 -Wall -Wextra zad4.c -o zad4
	
zad5: zad5.c
	gcc -std=gnu99 -Wall -Wextra zad5.c -o zad5
    
clean:
	rm -f zad4 zad5
