all:
	gcc -Wall -g main.c src/bigint.c src/utils.c -I src -o ent