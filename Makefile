all:
	gcc -Wall -Wextra -Werror -std=c90 -Ofast -s -pedantic main.c -o prog

clean:
	rm prog