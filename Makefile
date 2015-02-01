all: zad3 zad4 zad5 zad6

zad3: zad3.c
	gcc -std=gnu99 -Wall -Wextra zad3.c -o zad3

zad4: zad4.c
	gcc -std=gnu99 -Wall -Wextra zad4.c -o zad4
	
zad5: zad5.c
	gcc -std=gnu99 -Wall -Wextra zad5.c -o zad5
	
zad6: zad6.c
	gcc -std=gnu99 -Wall -Wextra zad6.c -o zad6
    
clean:
	rm -f zad3 zad4 zad5 zad6
